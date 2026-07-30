/*
 * C++11 常用语法示例
 * 覆盖: auto/decltype, 范围for, nullptr, override/final,
 *       lambda, 智能指针, 移动语义, 列表初始化, constexpr,
 *       enum class, static_assert, delete/default, using别名,
 *       std::function/bind, tuple, unordered容器, array,
 *       可变参数模板, 完美转发, chrono, thread_local 等
 */

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <array>
#include <tuple>
#include <string>
#include <memory>
#include <functional>
#include <chrono>
#include <thread>
#include <type_traits>
#include <initializer_list>

using namespace std;

// ==================== 1. auto 类型推导 ====================
void test_auto()
{
    auto i = 42;           // int
    auto d = 3.14;         // double
    auto s = string("hi"); // string
    auto v = vector<int>{1, 2, 3};

    // 遍历时 auto 省去冗长类型
    for (auto it = v.begin(); it != v.end(); ++it)
        cout << *it << " ";
    cout << endl;

    cout << "auto done" << endl;
}

// ==================== 2. decltype 类型推导 ====================
void test_decltype()
{
    int x = 10;
    decltype(x) y = 20; // y 的类型是 int

    vector<int> v{1, 2, 3};
    decltype(v.size()) sz = v.size(); // size_t

    // 配合 auto 用于返回类型后置 (trailing return type)
    auto add = [](int a, int b) -> decltype(a + b) {
        return a + b;
    };
    cout << add(1, 2) << endl;

    cout << "decltype done" << endl;
}

// ==================== 3. 范围 for 循环 ====================
void test_range_for()
{
    vector<int> v{1, 2, 3, 4, 5};

    // 只读
    for (auto val : v)
        cout << val << " ";
    cout << endl;

    // 修改（用引用）
    for (auto& val : v)
        val *= 2;

    for (auto val : v)
        cout << val << " ";
    cout << endl;

    // 避免拷贝（用 const 引用）
    for (const auto& val : v)
        cout << val << " ";
    cout << endl;

    cout << "range_for done" << endl;
}

// ==================== 4. nullptr ====================
void test_nullptr_val(int*)    { cout << "pointer" << endl; }
void test_nullptr_val(int)     { cout << "int" << endl; }

void test_nullptr()
{
    int* p = nullptr;        // 明确的空指针，不再是 NULL 宏
    // int* p2 = NULL;       // 老写法，容易二义性
    // int* p3 = 0;          // 老写法，容易二义性

    test_nullptr_val(nullptr); // 调用指针版本
    test_nullptr_val(0);       // 调用 int 版本

    // 判断空指针
    if (p == nullptr)
        cout << "p is nullptr" << endl;

    cout << "nullptr done" << endl;
}

// ==================== 5. override / final ====================
class Base
{
public:
    virtual void foo() { cout << "Base::foo" << endl; }
    virtual void bar() { cout << "Base::bar" << endl; }
};

class Derived : public Base
{
public:
    void foo() override { cout << "Derived::foo" << endl; }   // 明确标记重写
    // void foo(int) override { } // 编译错误: 基类没有虚函数 foo(int)

    void bar() final { cout << "Derived::bar(final)" << endl; } // 禁止再被重写
};

void test_override_final()
{
    Derived d;
    d.foo();
    d.bar();
    cout << "override/final done" << endl;
}

// ==================== 6. Lambda 表达式 ====================
void test_lambda()
{
    int n = 10;

    // 基础: [捕获列表](参数) -> 返回类型 { 函数体 }
    auto f1 = []() { cout << "hello lambda" << endl; };
    f1();

    // 值捕获 (只读)
    auto f2 = [n]() { cout << "captured: " << n << endl; };
    f2();

    // 引用捕获 (可修改)
    auto f3 = [&n]() { n += 5; };
    f3();
    cout << "n after ref lambda: " << n << endl;

    // mutable 允许修改值捕获的副本
    auto f4 = [n]() mutable { n += 10; return n; };
    cout << f4() << endl; // 25 (修改的是副本)
    cout << "original n: " << n << endl; // 15 (原值不变)

    // 隐式值捕获 [=] , 隐式引用捕获 [&]
    int a = 1, b = 2;
    auto f5 = [=]() { return a + b; }; // 值捕获所有
    auto f6 = [&]() { a = 10; b = 20; }; // 引用捕获所有
    f6();
    cout << "a=" << a << " b=" << b << endl;

    // 混合捕获
    auto f7 = [=, &b]() mutable { b = 30; return a; }; // a 值捕获, b 引用捕获

    // 直接传参
    auto add = [](int x, int y) -> int { return x + y; };
    cout << "add: " << add(3, 4) << endl;

    cout << "lambda done" << endl;
}

// ==================== 7. 智能指针 ====================
struct Obj
{
    int id;
    Obj(int i) : id(i) { cout << "Obj(" << id << ") created" << endl; }
    ~Obj() { cout << "Obj(" << id << ") destroyed" << endl; }
    void say() { cout << "I am Obj(" << id << ")" << endl; }
};

void test_smart_ptr()
{
    // unique_ptr: 独占所有权，不可拷贝，可移动
    cout << "--- unique_ptr ---" << endl;
    {
        unique_ptr<Obj> p1(new Obj(1));     // C++11 写法
        p1->say();

        auto p2 = move(p1); // 移动所有权
        if (!p1) cout << "p1 is null after move" << endl;
        p2->say();
        // 离开作用域自动 delete，Obj(1) destroyed
    }

    // shared_ptr: 共享所有权，引用计数
    cout << "--- shared_ptr ---" << endl;
    {
        auto p1 = make_shared<Obj>(2);
        cout << "use_count: " << p1.use_count() << endl;
        {
            auto p2 = p1;   // 拷贝，引用计数+1
            cout << "use_count: " << p1.use_count() << endl;
            {
                auto p3 = p1;
                cout << "use_count: " << p1.use_count() << endl;
            }
            cout << "use_count after p3 gone: " << p1.use_count() << endl;
        }
        cout << "use_count after p2 gone: " << p1.use_count() << endl;
    }
    // 最后一个 shared_ptr 析构时 delete Obj

    // weak_ptr: 不增加引用计数，解决循环引用
    cout << "--- weak_ptr ---" << endl;
    {
        auto shared = make_shared<Obj>(3);
        weak_ptr<Obj> weak = shared;
        cout << "use_count: " << shared.use_count() << endl; // 还是1

        if (auto locked = weak.lock())   // 尝试提升为 shared_ptr
        {
            cout << "weak_ptr locked, use_count: " << shared.use_count() << endl;
            locked->say();
        }
    }
    // shared 析构后 weak 自动过期

    cout << "smart_ptr done" << endl;
}

// ==================== 8. 移动语义 & 右值引用 ====================
class MoveObj
{
public:
    int* data;
    size_t size;

    // 构造函数
    MoveObj(size_t sz) : size(sz)
    {
        data = new int[sz];
        cout << "ctor: alloc " << sz << endl;
    }

    // 拷贝构造 (深拷贝)
    MoveObj(const MoveObj& other) : size(other.size)
    {
        data = new int[size];
        copy(other.data, other.data + size, data);
        cout << "copy ctor" << endl;
    }

    // 移动构造: 偷走资源
    MoveObj(MoveObj&& other) noexcept : data(other.data), size(other.size)
    {
        other.data = nullptr; // 将源对象置空
        other.size = 0;
        cout << "move ctor" << endl;
    }

    // 拷贝赋值
    MoveObj& operator=(const MoveObj& other)
    {
        if (this != &other)
        {
            delete[] data;
            size = other.size;
            data = new int[size];
            copy(other.data, other.data + size, data);
            cout << "copy assign" << endl;
        }
        return *this;
    }

    // 移动赋值
    MoveObj& operator=(MoveObj&& other) noexcept
    {
        if (this != &other)
        {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            cout << "move assign" << endl;
        }
        return *this;
    }

    ~MoveObj() { delete[] data; cout << "dtor" << endl; }
};

MoveObj create_obj() { return MoveObj(5); }

void test_move()
{
    cout << "--- 拷贝 vs 移动 ---" << endl;
    MoveObj a(10);
    MoveObj b = a;            // 拷贝构造
    MoveObj c = move(a);      // 移动构造 (a 的资源被偷走)

    MoveObj d(3);
    d = b;                    // 拷贝赋值
    d = MoveObj(8);           // 移动赋值（临时对象是右值）

    cout << "move done" << endl;
}

// ==================== 9. 统一初始化 (列表初始化) ====================
void test_init_list()
{
    // 统一的 {} 初始化语法
    int a{42};
    int b{};                 // 零初始化，b=0
    double arr[]{1.1, 2.2, 3.3};

    vector<int> v{1, 2, 3, 4, 5};
    map<string, int> m{{"a", 1}, {"b", 2}};

    // 防止窄化转换 (编译报错)
    // int x{3.14};          // error: 窄化转换

    // initializer_list 作为函数参数
    auto sum = [](initializer_list<int> list) -> int {
        int total = 0;
        for (auto x : list) total += x;
        return total;
    };
    cout << sum({1, 2, 3, 4, 5}) << endl;

    for (auto& kv : m)
        cout << kv.first << "=" << kv.second << " ";
    cout << endl;

    cout << "init_list done" << endl;
}

// ==================== 10. constexpr ====================
constexpr int square(int n) { return n * n; }
constexpr int factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }

void test_constexpr()
{
    constexpr int val = square(10);  // 编译期计算
    int arr[val];                    // 可用作数组大小
    cout << "square(10) = " << val << endl;
    cout << "factorial(5) = " << factorial(5) << endl;

    // 也可以运行时调用
    int n;
    cout << "input n: ";
    cin >> n;
    cout << "square(" << n << ") = " << square(n) << endl;

    cout << "constexpr done" << endl;
}

// ==================== 11. enum class ====================
enum class Color { Red, Green, Blue };
enum class Traffic { Red, Yellow, Green }; // 作用域隔离，不冲突

void test_enum_class()
{
    Color c = Color::Red; // 必须带作用域

    // if (c == 0)  // error: 不能隐式转换为 int
    if (c == Color::Red)
        cout << "Red" << endl;

    // 显式转换
    cout << "Color::Red = " << static_cast<int>(Color::Red) << endl;

    // 可指定底层类型
    enum class Perm : unsigned char { Read = 1, Write = 2, Exec = 4 };
    cout << "sizeof(Perm) = " << sizeof(Perm) << endl;

    cout << "enum_class done" << endl;
}

// ==================== 12. static_assert ====================
template <typename T>
T safe_abs(T val)
{
    static_assert(is_arithmetic<T>::value, "T must be numeric");
    return val >= 0 ? val : -val;
}

void test_static_assert()
{
    // 编译期断言
    static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
    static_assert(sizeof(void*) == 8, "64-bit platform expected");

    cout << safe_abs(-10) << endl;
    cout << safe_abs(3.14f) << endl;
    // safe_abs("hello"); // 编译错误: T must be numeric

    cout << "static_assert done" << endl;
}

// ==================== 13. delete & default ====================
class NonCopyable
{
public:
    NonCopyable() = default;                          // 显式要求编译器生成默认构造
    NonCopyable(const NonCopyable&) = delete;          // 禁止拷贝构造
    NonCopyable& operator=(const NonCopyable&) = delete; // 禁止拷贝赋值
};

void test_delete_default()
{
    NonCopyable a;
    // NonCopyable b = a;  // 编译错误: 拷贝构造被 delete
    // auto c = a;         // 同上

    cout << "delete/default done" << endl;
}

// 模板别名 (typedef 做不到的) — 必须在命名空间/类作用域
template <typename T>
using Vec = vector<T>;

// ==================== 14. using 类型别名 ====================
void test_using()
{
    // using 比 typedef 更直观
    using StringMap = map<string, string>;
    using IntVector = vector<int>;
    using FuncPtr = void(*)(int);

    Vec<int> vi{1, 2, 3};
    Vec<string> vs{"a", "b", "c"};  // Vec<T> 是上面的模板别名

    StringMap m{{"key1", "val1"}};
    cout << m["key1"] << endl;

    cout << "using done" << endl;
}

// ==================== 15. std::function & std::bind ====================
int add3(int a, int b, int c)
{
    return a + b + c;
}

void test_function_bind()
{
    // function: 可存储任意可调用对象
    function<int(int, int)> f1 = [](int a, int b) { return a + b; };
    cout << "f1: " << f1(3, 4) << endl;

    // bind: 参数绑定 / 占位
    using namespace placeholders; // _1, _2, ...

    auto f2 = bind(add3, _1, _2, 10); // add3(a, b, 10)
    cout << "f2: " << f2(1, 2) << endl;           // 1+2+10 = 13

    auto f3 = bind(add3, _2, _1, 0);  // add3(第二个参数, 第一个参数, 0)
    cout << "f3: " << f3(1, 2) << endl;           // 2+1+0 = 3

    // 绑定成员函数
    Obj obj(99);
    auto f4 = bind(&Obj::say, &obj);  // &obj 是 object_ptr
    f4();

    cout << "function/bind done" << endl;
}

// ==================== 16. std::tuple ====================
void test_tuple()
{
    // 创建
    auto t1 = make_tuple(42, 3.14, string("hello"));

    // 访问
    cout << get<0>(t1) << endl;
    cout << get<1>(t1) << endl;
    cout << get<2>(t1) << endl;

    // 解包
    int i;
    double d;
    string s;
    tie(i, d, s) = t1;
    cout << "tie: " << i << ", " << d << ", " << s << endl;

    // 忽略某些值
    tie(i, ignore, s) = t1;

    // 拼接
    auto t2 = tuple_cat(t1, make_tuple('x', true));
    cout << "t2 size: " << tuple_size<decltype(t2)>::value << endl;

    cout << "tuple done" << endl;
}

// ==================== 17. unordered 容器 ====================
void test_unordered()
{
    unordered_map<string, int> um{{"apple", 5}, {"banana", 3}, {"cherry", 8}};
    um["durian"] = 2;

    for (const auto& kv : um)
        cout << kv.first << "=" << kv.second << " ";
    cout << endl;

    // 查找 (O(1) 均摊)
    auto it = um.find("banana");
    if (it != um.end())
        cout << "found banana: " << it->second << endl;

    // bucket 信息
    cout << "bucket_count: " << um.bucket_count() << endl;
    cout << "load_factor: " << um.load_factor() << endl;

    cout << "unordered done" << endl;
}

// ==================== 18. std::array ====================
void test_array()
{
    array<int, 5> arr{1, 2, 3, 4, 5};

    cout << "size: " << arr.size() << endl;

    // 支持迭代器，与普通数组一样连续存储
    for (auto it = arr.begin(); it != arr.end(); ++it)
        cout << *it << " ";
    cout << endl;

    // 支持范围 for
    for (auto x : arr) cout << x << " ";
    cout << endl;

    // at() 带边界检查
    // arr.at(10); // 抛 out_of_range 异常

    // 填充
    arr.fill(0);
    cout << "after fill: ";
    for (auto x : arr) cout << x << " ";
    cout << endl;

    // 零开销，和 C 数组一样快
    cout << "sizeof(array) == sizeof(int[5]): "
         << (sizeof(arr) == sizeof(int[5])) << endl;

    cout << "array done" << endl;
}

// ==================== 19. 可变参数模板 ====================
// 递归终止
void print_all() { cout << endl; }

template <typename T, typename... Args>
void print_all(T first, Args... rest)
{
    cout << first << " ";
    print_all(rest...);
}

// 同样用 sizeof...(Args) 可获取参数个数
template <typename... Args>
int count_args(Args... args)
{
    return sizeof...(args);
}

void test_variadic()
{
    print_all(1, 2.5, "hello", 'c');
    cout << "arg count: " << count_args(1, 2, 3, 4, 5) << endl;
    cout << "variadic done" << endl;
}

// ==================== 20. 完美转发 ====================
void process(int& x)  { cout << "lvalue ref" << endl; }
void process(int&& x) { cout << "rvalue ref" << endl; }

template <typename T>
void forwarder(T&& arg)
{
    // forward 保持值类别
    process(forward<T>(arg));
}

void test_perfect_forward()
{
    int a = 42;
    forwarder(a);       // a 是左值 → lvalue ref
    forwarder(100);     // 100 是右值 → rvalue ref
    forwarder(move(a)); // move(a) 是右值 → rvalue ref

    cout << "perfect_forward done" << endl;
}

// ==================== 21. chrono 时间库 ====================
void test_chrono()
{
    using namespace chrono;

    // 时间点
    auto start = steady_clock::now();

    // 做点事
    this_thread::sleep_for(milliseconds(100));

    auto end = steady_clock::now();

    // 时长 (duration)
    auto elapsed = duration_cast<microseconds>(end - start);
    cout << "elapsed: " << elapsed.count() << " us" << endl;

    // 各种时长定义
    hours h(1);
    minutes m(30);
    seconds s(45);
    cout << "total seconds: " << (h + m + s).count() << endl;

    // system_clock → 时间戳
    auto now = system_clock::now();
    auto t = system_clock::to_time_t(now);
    cout << "current time: " << ctime(&t); // 打印当前时间

    cout << "chrono done" << endl;
}

// ==================== 22. thread_local ====================
thread_local int tls_val = 0;

void tls_task(int id)
{
    tls_val = id * 100;
    this_thread::sleep_for(chrono::milliseconds(10));
    cout << "thread " << id << ": tls_val=" << tls_val << endl;
}

void test_thread_local()
{
    thread t1(tls_task, 1);
    thread t2(tls_task, 2);
    t1.join();
    t2.join();
    // 每个线程有自己独立的 tls_val 副本

    cout << "thread_local done" << endl;
}

// ==================== 23. noexcept ====================
void might_throw() {}
void never_throws() noexcept {}

void test_noexcept()
{
    cout << boolalpha;
    cout << "might_throw noexcept: " << noexcept(might_throw()) << endl;
    cout << "never_throws noexcept: " << noexcept(never_throws()) << endl;

    // 移动构造标记 noexcept 有助于 STL 容器优化
    // (如 vector 扩容时优先移动而非拷贝)
    cout << "MoveObj move ctor noexcept: "
         << is_nothrow_move_constructible<MoveObj>::value << endl;

    cout << "noexcept done" << endl;
}

// ==================== 24. 委托构造 & 继承构造 ====================
class MyWidget
{
    int x, y;
public:
    MyWidget() : MyWidget(0, 0)             // 委托构造
    {
        cout << "default ctor (delegating)" << endl;
    }
    MyWidget(int x_, int y_) : x(x_), y(y_)
    {
        cout << "main ctor: (" << x << ", " << y << ")" << endl;
    }
};

void test_delegating_ctor()
{
    MyWidget w1;                 // 调用默认构造 → 委托到 (0,0)
    MyWidget w2(10, 20);         // 直接调用
    cout << "delegating_ctor done" << endl;
}

// ==================== 25. 右值引用的成员函数限定 ====================
class Buffer
{
    string data;
public:
    Buffer(string s) : data(move(s)) {}

    // 左值: 返回拷贝
    string value() const & { cout << "lvalue call" << endl; return data; }

    // 右值: 返回移动 (避免拷贝)
    string value() && { cout << "rvalue call" << endl; return move(data); }
};

void test_ref_qualifier()
{
    Buffer buf("hello world");
    auto s1 = buf.value();            // lvalue 版本
    auto s2 = Buffer("temp").value(); // rvalue 版本 (临时对象)

    cout << "ref_qualifier done" << endl;
}

// ==================== 26. long long, char16/32_t, raw string ====================
void test_misc_types()
{
    long long ll = 9223372036854775807LL;
    unsigned long long ull = 18446744073709551615ULL;
    cout << "long long: " << ll << endl;

    // 原始字符串字面量
    const char* raw = R"(C:\path\to\file)" R"(
    )";
    cout << "raw string: " << raw << endl;

    cout << "misc_types done" << endl;
}

// ==================== main ====================
int main()
{
    test_auto();
    cout << "--------------------------------" << endl;
    test_decltype();
    cout << "--------------------------------" << endl;
    test_range_for();
    cout << "--------------------------------" << endl;
    test_nullptr();
    cout << "--------------------------------" << endl;
    test_override_final();
    cout << "--------------------------------" << endl;
    test_lambda();
    cout << "--------------------------------" << endl;
    test_smart_ptr();
    cout << "--------------------------------" << endl;
    test_move();
    cout << "--------------------------------" << endl;
    test_init_list();
    cout << "--------------------------------" << endl;
    test_constexpr();
    cout << "--------------------------------" << endl;
    test_enum_class();
    cout << "--------------------------------" << endl;
    test_static_assert();
    cout << "--------------------------------" << endl;
    test_delete_default();
    cout << "--------------------------------" << endl;
    test_using();
    cout << "--------------------------------" << endl;
    test_function_bind();
    cout << "--------------------------------" << endl;
    test_tuple();
    cout << "--------------------------------" << endl;
    test_unordered();
    cout << "--------------------------------" << endl;
    test_array();
    cout << "--------------------------------" << endl;
    test_variadic();
    cout << "--------------------------------" << endl;
    test_perfect_forward();
    cout << "--------------------------------" << endl;
    test_chrono();
    cout << "--------------------------------" << endl;
    test_thread_local();
    cout << "--------------------------------" << endl;
    test_noexcept();
    cout << "--------------------------------" << endl;
    test_delegating_ctor();
    cout << "--------------------------------" << endl;
    test_ref_qualifier();
    cout << "--------------------------------" << endl;
    test_misc_types();

    return 0;
}
