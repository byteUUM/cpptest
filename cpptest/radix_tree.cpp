#include <string>
#include <map>
#include <iostream>
using namespace std;

// 基数树(压缩前缀树)节点：每条边存一段子串，而非单字符
struct Node {
    string edge;            // 从父节点到本节点边上的子串
    map<char, Node*> next;  // 按边首字符索引的子节点
    bool end = false;       // 是否是某个单词的结尾
};

class RadixTree {
    Node* root = new Node;

    // 求两字符串公共前缀长度
    static int lcp(const string& a, const string& b) {
        int i = 0;
        while (i < (int)a.size() && i < (int)b.size() && a[i] == b[i]) i++;
        return i;
    }

    // 递归插入 key 到 node 之下
    void insert(Node* node, const string& key) {
        if (key.empty()) { node->end = true; return; }

        char c = key[0];
        auto it = node->next.find(c);
        if (it == node->next.end()) {            // 无相同首字符的子节点，直接挂新叶
            node->next[c] = new Node{key, {}, true};
            return;
        }

        Node* child = it->second;
        int l = lcp(key, child->edge);
        if (l == (int)child->edge.size()) {      // 边被完全覆盖，继续向下
            insert(child, key.substr(l));
            return;
        }

        // 分裂边：拆出一个中间节点
        Node* mid = new Node{child->edge.substr(0, l)};
        child->edge = child->edge.substr(l);
        mid->next[child->edge[0]] = child;
        node->next[c] = mid;

        if (l == (int)key.size()) mid->end = true;             // key 恰好是前缀
        else mid->next[key[l]] = new Node{key.substr(l), {}, true};
    }

    // 递归查找
    bool find(Node* node, const string& key) const {
        if (key.empty()) return node->end;

        auto it = node->next.find(key[0]);
        if (it == node->next.end()) return false;

        Node* child = it->second;
        if (key.size() < child->edge.size() || key.compare(0, child->edge.size(), child->edge) != 0)
            return false;
        return find(child, key.substr(child->edge.size()));
    }

public:
    void insert(const string& key) { insert(root, key); }
    bool find(const string& key) const { return find(root, key); }
};

int main() {
    RadixTree t;
    for (string s : {"romane", "romanus", "romulus", "rubens", "ruber", "rubicon", "rubicundus"})
        t.insert(s);

    for (string s : {"romane", "romanus", "romulus", "rubens", "ruber", "rom", "rub", "x", "rubicon"})
        cout << s << " -> " << (t.find(s) ? "found" : "not found") << endl;
}
