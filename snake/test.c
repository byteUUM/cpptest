// #include <stdio.h>

// #ifdef _WIN32
//     #include <stdlib.h>
//     #define SET_WINDOW_SIZE(w,h) system("mode con cols=" #w " lines=" #h)
// #else
//     // Linux/macOS 使用 ANSI 转义序列
//     #define SET_WINDOW_SIZE(w,h) \
//         printf("\e[8;" #h ";" #w "t")  // 设置终端窗口大小
// #endif

// int main() {
//     SET_WINDOW_SIZE(100, 35);
//     return 0;
// }