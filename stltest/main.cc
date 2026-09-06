#include <iostream>
#include "String.hpp"
#include "Vector.hpp"
using namespace std;

// 测试一个 vector<int> 的打印
template <class T>
void print_vec(const byte::vector<T>& v, const char* name) {
    cout << name << ": size=" << v.size() << " cap=" << v.capacity() << " [";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i + 1 != v.size()) cout << ", ";
    }
    cout << "]\n";
}

void string_test1() { // 构造 / 遍历 / 拼接
    byte::string s("hello");
    cout << s << " size=" << s.size() << " cap=" << s.capacity() << endl;

    byte::string t = s;                 // 拷贝构造
    byte::string m = std::move(t);      // 移动构造
    cout << "copy+move: " << m << endl;

    s += " world";
    s.push_back('!');
    cout << "concat: " << s << endl;

    for (char c : s) cout << c << ' ';  // 范围 for
    cout << endl;
}

void string_test2() { // reserve / resize / 扩容
    byte::string s("abc");
    cout << "cap=" << s.capacity() << endl;
    s.reserve(100);
    cout << "after reserve(100): cap=" << s.capacity() << endl;

    s.resize(8, 'x');
    cout << "after resize(8,'x'): " << s << " size=" << s.size() << endl;
    s.resize(3);
    cout << "after resize(3): " << s << " size=" << s.size() << endl;
}

void string_test3() { // insert / erase / find / substr
    byte::string s("abcdefg");
    s.insert(0, '0');
    cout << "insert(0,'0'): " << s << endl;
    s.erase(2, 3);
    cout << "erase(2,3): " << s << endl;

    cout << "find('d')=" << s.find('d') << " find(\"ef\")=" << s.find("ef") << endl;
    cout << "substr(1,3)=" << s.substr(1, 3) << endl;
}

void string_test4() { // 比较 / 输入
    byte::string a("apple"), b("banana");
    cout << boolalpha;
    cout << "apple < banana : " << (a < b) << endl;
    cout << "apple == apple: " << (a == byte::string("apple")) << endl;
    cout << "apple == \"apple\": " << (a == "apple") << endl;

    byte::string in;
    cout << "input a word: ";
    cin >> in;
    cout << "you typed: " << in << endl;
}

void vector_test1() { // 构造 / push_back / 扩容
    byte::vector<int> v;
    for (int i = 0; i < 10; ++i) v.push_back(i);
    print_vec(v, "push_back");

    byte::vector<int> w(3, 7);          // 3 个 7
    print_vec(w, "n*val");

    byte::vector<int> u{1, 2, 3, 4};    // 初始化列表
    print_vec(u, "init-list");
}

void vector_test2() { // insert / erase
    byte::vector<int> v{1, 2, 3, 4, 5};
    v.insert(v.begin() + 2, 99);
    print_vec(v, "insert(2,99)");

    v.erase(v.begin());
    print_vec(v, "erase(begin)");

    v.erase(v.begin() + 1, v.begin() + 3); // 区间删除
    print_vec(v, "erase[1,3)");
}

void vector_test3() { // resize / reserve / 元素访问
    byte::vector<int> v{1, 2, 3};
    v.resize(6, 0);
    print_vec(v, "resize(6,0)");
    v.resize(2);
    print_vec(v, "resize(2)");

    v.reserve(100);
    cout << "after reserve(100): cap=" << v.capacity() << endl;

    cout << "front=" << v.front() << " back=" << v.back() << endl;
    v[0] = 42;
    cout << "v[0]=" << v[0] << " at(1)=" << v.at(1) << endl;
}

// 用自定义类型验证"只按需构造、移动语义正确"
struct Widget {
    int id;
    static int live;                 // 存活对象计数
    Widget(int i = 0) : id(i) { ++live; }
    Widget(const Widget& w) : id(w.id) { ++live; }
    Widget(Widget&& w) noexcept : id(w.id) { w.id = -1; ++live; }
    ~Widget() { --live; }
    friend ostream& operator<<(ostream& o, const Widget& w) { return o << w.id; }
};
int Widget::live = 0;

void vector_test4() { // emplace_back / 移动 / 析构正确性
    {
        byte::vector<Widget> v;
        v.emplace_back(1);            // 直接原地构造
        v.emplace_back(2);
        v.push_back(Widget(3));       // 移动入参
        print_vec(v, "widgets");

        byte::vector<Widget> w = std::move(v);
        cout << "after move, v.size=" << v.size() << " w.size=" << w.size() << endl;
        cout << "live=" << Widget::live << endl;
    }
    cout << "after scope, live=" << Widget::live << " (应为 0)" << endl;
}

int main() {
    string_test1();
    string_test2();
    string_test3();
    // string_test4(); // 需要输入，按需打开
    cout << "----------------\n";
    vector_test1();
    vector_test2();
    vector_test3();
    vector_test4();
    return 0;
}
