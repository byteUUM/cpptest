// gdb_demo.cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <memory>
#include <cstdio>
using namespace std;

int g_count = 0;                         // 练习 watch

struct Student {
    string name;
    int score;
};

int factorial(int n) {                   // 练习 step / bt / finish
    g_count++;
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int sum(const vector<int>& v) {          // 练习 display / watch
    int total = 0;
    for (size_t i = 0; i < v.size(); ++i)
        total += v[i];
    return total;
}

double average(const vector<Student>& stu) {
    int total = 0;
    for (const auto& s : stu) total += s.score;
    return (double)total / stu.size();   // stu 为空时结果是 nan/inf，可用 p 观察
}

void check(int score) {                  // 练习 catch throw
    if (score < 0) throw invalid_argument("score < 0");
}

void crash() {                           // 练习段错误 + bt
    int* p = nullptr;
    *p = 42;
}

int main() {
    vector<int> nums = {1, 2, 3, 4, 5};
    cout << "sum = " << sum(nums) << endl;

    cout << "5! = " << factorial(5) << endl;

    vector<Student> stu = {{"Tom", 80}, {"Amy", 95}, {"Bob", 70}};
    cout << "avg = " << average(stu) << endl;

    auto closer = [](FILE* f) { if (f) fclose(f); };
    unique_ptr<FILE, decltype(closer)> fp(fopen("gdb_demo.cpp", "r"), closer);
    cout << "file opened: " << (fp ? "yes" : "no") << endl;

    try {
        check(-1);
    } catch (const exception& e) {
        cout << "caught: " << e.what() << endl;
    }

    // crash();   // 练习段错误时，取消这行注释
    return 0;
}