#include <bits/stdc++.h>
using namespace std;
using V = vector<int>;

// ---------- O(n^2) ----------
void bubble(V& a) {
    for (int i = (int)a.size() - 1; i > 0; --i) {
        bool swp = false;
        for (int j = 0; j < i; ++j)
            if (a[j] > a[j + 1]) swap(a[j], a[j + 1]), swp = true;
        if (!swp) break;  // 已有序提前退出
    }
}

void selection(V& a) {
    for (size_t i = 0; i + 1 < a.size(); ++i) {
        size_t m = i;
        for (size_t j = i + 1; j < a.size(); ++j)
            if (a[j] < a[m]) m = j;
        swap(a[i], a[m]);
    }
}

void insertion(V& a) {
    for (size_t i = 1; i < a.size(); ++i) {
        int k = a[i], j = (int)i - 1;
        for (; j >= 0 && a[j] > k; --j) a[j + 1] = a[j];
        a[j + 1] = k;
    }
}

void shell(V& a) {
    int n = a.size();
    for (int g = n / 2; g > 0; g /= 2)
        for (int i = g; i < n; ++i) {
            int k = a[i], j = i - g;
            for (; j >= 0 && a[j] > k; j -= g) a[j + g] = a[j];
            a[j + g] = k;
        }
}

// ---------- O(n log n) ----------
void msort(V& a, V& t, int l, int r) {  // [l, r)
    if (r - l < 2) return;
    int m = (l + r) / 2;
    msort(a, t, l, m), msort(a, t, m, r);
    int i = l, j = m, k = l;
    while (i < m && j < r) t[k++] = a[i] <= a[j] ? a[i++] : a[j++];  // <= 保证稳定
    while (i < m) t[k++] = a[i++];
    while (j < r) t[k++] = a[j++];
    copy(t.begin() + l, t.begin() + r, a.begin() + l);
}
void merge_sort(V& a) { V t(a.size()); msort(a, t, 0, a.size()); }

void qsort3(V& a, int l, int r) {  // [l, r]，随机基准 + 三路划分，重复元素多也不退化
    if (l >= r) return;
    int p = a[l + rand() % (r - l + 1)], lt = l, i = l, gt = r;
    while (i <= gt)
        if (a[i] < p) swap(a[lt++], a[i++]);
        else if (a[i] > p) swap(a[i], a[gt--]);
        else ++i;
    qsort3(a, l, lt - 1), qsort3(a, gt + 1, r);
}
void quick(V& a) { qsort3(a, 0, (int)a.size() - 1); }

void down(V& a, int n, int i) {
    for (int c; (c = 2 * i + 1) < n; i = c) {
        if (c + 1 < n && a[c + 1] > a[c]) ++c;
        if (a[i] >= a[c]) break;
        swap(a[i], a[c]);
    }
}
void heap(V& a) {
    int n = a.size();
    for (int i = n / 2 - 1; i >= 0; --i) down(a, n, i);  // 建大顶堆
    for (int i = n - 1; i > 0; --i) swap(a[0], a[i]), down(a, i, 0);
}

// ---------- 非比较 ----------
void counting(V& a) {  // 稳定，支持负数
    if (a.empty()) return;
    int lo = *min_element(a.begin(), a.end()), hi = *max_element(a.begin(), a.end());
    V c(hi - lo + 2), r(a.size());
    for (int x : a) ++c[x - lo + 1];
    partial_sum(c.begin(), c.end(), c.begin());  // c[v] = 值 v 的起始下标
    for (int x : a) r[c[x - lo]++] = x;
    a = move(r);
}

void bucket(V& a) {
    if (a.size() < 2) return;
    long long lo = *min_element(a.begin(), a.end()), hi = *max_element(a.begin(), a.end());
    long long n = a.size(), span = hi - lo + 1;
    vector<V> b(n);
    for (int x : a) b[(x - lo) * n / span].push_back(x);
    a.clear();
    for (auto& v : b) insertion(v), a.insert(a.end(), v.begin(), v.end());
}

void radix(V& a) {  // LSD，十进制，支持负数（减去最小值偏移）
    if (a.empty()) return;
    long long lo = *min_element(a.begin(), a.end());
    long long mx = *max_element(a.begin(), a.end()) - lo;
    V t(a.size());
    for (long long e = 1; mx / e > 0; e *= 10) {
        auto d = [&](int x) { return (x - lo) / e % 10; };
        int c[11] = {};
        for (int x : a) ++c[d(x) + 1];
        for (int i = 0; i < 10; ++i) c[i + 1] += c[i];
        for (int x : a) t[c[d(x)]++] = x;
        a.swap(t);
    }
}

// ---------- 测试 ----------
int main() {
    vector<pair<const char*, void (*)(V&)>> algos = {
        {"bubble", bubble},   {"selection", selection}, {"insertion", insertion},
        {"shell", shell},     {"merge", merge_sort},    {"quick", quick},
        {"heap", heap},       {"counting", counting},   {"bucket", bucket},
        {"radix", radix}};
    mt19937 rng(42);
    for (auto& [name, f] : algos) {
        bool ok = true;
        for (int t = 0; t < 300 && ok; ++t) {
            int n = rng() % 200, range = t % 3 == 0 ? 5 : 100000;  // 小范围 => 大量重复
            V a(n);
            for (int& x : a) x = (int)(rng() % (2 * range + 1)) - range;  // 含负数
            V b = a;
            f(a), sort(b.begin(), b.end());
            ok = a == b;
        }
        printf("%-10s %s\n", name, ok ? "OK" : "FAIL");
    }
}
