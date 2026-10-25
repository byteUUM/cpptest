// 编译: g++ -std=c++17 es_demo.cpp -lcurl -o es_demo
// 依赖: sudo apt install libcurl4-openssl-dev nlohmann-json3-dev
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

class ES {
    std::string base_;
    static size_t write_cb(char* p, size_t s, size_t n, void* u) {
        static_cast<std::string*>(u)->append(p, s * n);
        return s * n;
    }

public:
    explicit ES(std::string base = "http://127.0.0.1:9200") : base_(std::move(base)) {}

    // 通用 REST 请求, 返回解析后的 JSON
    json req(const std::string& method, const std::string& path,
             const std::string& body = "", const char* ctype = "application/json") {
        CURL* c = curl_easy_init();
        std::string out;
        curl_slist* h = curl_slist_append(nullptr, (std::string("Content-Type: ") + ctype).c_str());
        curl_easy_setopt(c, CURLOPT_URL, (base_ + path).c_str());
        curl_easy_setopt(c, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(c, CURLOPT_HTTPHEADER, h);
        if (!body.empty()) curl_easy_setopt(c, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(c, CURLOPT_WRITEDATA, &out);
        CURLcode rc = curl_easy_perform(c);
        curl_slist_free_all(h);
        curl_easy_cleanup(c);
        if (rc != CURLE_OK) throw std::runtime_error(curl_easy_strerror(rc));
        return json::parse(out, nullptr, false);
    }

    json create_index(const std::string& idx, const json& body) { return req("PUT", "/" + idx, body.dump()); }
    json drop_index(const std::string& idx) { return req("DELETE", "/" + idx); }
    json put_doc(const std::string& idx, const std::string& id, const json& doc) {
        return req("PUT", "/" + idx + "/_doc/" + id, doc.dump());
    }
    json get_doc(const std::string& idx, const std::string& id) { return req("GET", "/" + idx + "/_doc/" + id); }
    json search(const std::string& idx, const json& q) { return req("POST", "/" + idx + "/_search", q.dump()); }
    json bulk(const std::string& idx, const std::string& ndjson) {
        return req("POST", "/" + idx + "/_bulk?refresh=true", ndjson, "application/x-ndjson");
    }
};

static void print_hits(const json& r) {
    std::cout << "命中 " << r["hits"]["total"]["value"] << " 条\n";
    for (auto& h : r["hits"]["hits"]) {
        std::cout << "  [" << h["_score"] << "] " << h["_source"]["title"].get<std::string>()
                  << " (" << h["_source"]["author"].get<std::string>()
                  << ", views=" << h["_source"]["views"] << ")\n";
        if (h.contains("highlight"))
            std::cout << "      高亮: " << h["highlight"]["title"][0].get<std::string>() << "\n";
    }
}

int main() {
    curl_global_init(CURL_GLOBAL_ALL);
    try {
        ES es;
        const std::string idx = "blog";

        // 1. 重建索引 (mapping: text 分词, keyword 精确匹配)
        es.drop_index(idx);
        es.create_index(idx, {
            {"settings", {{"number_of_shards", 1}, {"number_of_replicas", 0}}},
            {"mappings", {{"properties", {
                {"title",  {{"type", "text"}}},  // 中文生产环境请换 {"analyzer","ik_max_word"}
                {"author", {{"type", "keyword"}}},
                {"views",  {{"type", "integer"}}}
            }}}}
        });

        // 2. 单文档写入 + 读取
        es.put_doc(idx, "1", {{"title", "C++ memory pool design"}, {"author", "conwy"}, {"views", 120}});
        std::cout << "GET /1 -> " << es.get_doc(idx, "1")["_source"].dump() << "\n";

        // 3. Bulk 批量写入 (NDJSON: 每行一个 JSON, 末尾必须换行)
        std::string nd;
        auto add = [&](int id, const std::string& t, const std::string& a, int v) {
            nd += json{{"index", {{"_id", std::to_string(id)}}}}.dump() + "\n";
            nd += json{{"title", t}, {"author", a}, {"views", v}}.dump() + "\n";
        };
        add(2, "Elasticsearch inverted index explained", "conwy", 300);
        add(3, "C++ thread pool and lock free queue", "alice", 80);
        add(4, "Redis vs Elasticsearch for search", "bob", 500);
        es.bulk(idx, nd);

        // 4. 全文检索 + 高亮 (BM25 打分)
        std::cout << "\n== match: elasticsearch ==\n";
        print_hits(es.search(idx, {
            {"query", {{"match", {{"title", "elasticsearch"}}}}},
            {"highlight", {{"fields", {{"title", json::object()}}}}}
        }));

        // 5. bool 查询: 全文 must + 精确过滤 filter (filter 不算分, 可缓存)
        std::cout << "\n== bool: title~C++ AND author=conwy AND views>=100 ==\n";
        print_hits(es.search(idx, {
            {"query", {{"bool", {
                {"must",   {{"match", {{"title", "C++"}}}}},
                {"filter", json::array({
                    {{"term",  {{"author", "conwy"}}}},
                    {{"range", {{"views", {{"gte", 100}}}}}}
                })}
            }}}}
        }));

        // 6. 聚合: 按作者统计文章数与总浏览量 (类似 GROUP BY)
        std::cout << "\n== aggs: group by author ==\n";
        auto r = es.search(idx, {
            {"size", 0},
            {"aggs", {{"by_author", {
                {"terms", {{"field", "author"}}},
                {"aggs", {{"total_views", {{"sum", {{"field", "views"}}}}}}}
            }}}}
        });
        for (auto& b : r["aggregations"]["by_author"]["buckets"])
            std::cout << "  " << b["key"].get<std::string>() << ": " << b["doc_count"]
                      << " 篇, views=" << b["total_views"]["value"] << "\n";

    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
    curl_global_cleanup();
    return 0;
}