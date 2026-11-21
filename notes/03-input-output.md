# 03 · 输入与输出

> 对应练习：`ex01_types_io.cpp`
> 目标：把 `cin` / `cout` 用熟，会格式化输出，知道大数据量时怎么加速。

---

## 1. 最小可用的两套 I/O

**方式 A：C++ 流**（本仓库默认，类型安全，写 `string` 方便）

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    int a; double b; string s;
    cin >> a >> b >> s;     // 从键盘读，自动按空白切分
    cout << "a=" << a << " b=" << b << " s=" << s << "\n";
    return 0;
}
```

**方式 B：C 风格 `printf`/`scanf`**（更快一点，但要自己写格式符）

```cpp
#include <cstdio>

int main() {
    int a; double b;
    scanf("%d%lf", &a, &b);
    printf("a=%d b=%.2f\n", a, b);
    return 0;
}
```

两套**不要混用**（`cin` 和 `scanf` 交错读同一输入流会出现顺序错乱，除非你先 `ios::sync_with_stdio(false)` 并坚持只用一套）。

格式符速查：

| 类型 | `scanf` | `printf` |
|---|---|---|
| `int` | `%d` | `%d` |
| `long long` | `%lld` | `%lld` |
| `double` | `%lf` | `%f` 或 `%.2f` |
| `char` | ` %c`（前面空格跳空白） | `%c` |
| 字符串 | `%s`（不含空格） | `%s` |
| 一行文本 | `gets` 已废弃，用 `fgets` | — |

---

## 2. `cin >>` 到底怎么切数据

`cin >> x` 的行为：

1. **跳过前面所有空白**（空格、Tab、换行）；
2. 一直读到下一个空白为止；
3. 按 `x` 的类型解释这段文本。

所以：

```
输入写成一行：   3 5
输入写成两行：   3
                5
两者对 cin 来说完全一样。
```

这解释了课堂作业里"2m 个整数故意写在同一行"为什么要担心——**其实不用担心**，`cin >> l >> r` 循环读就行，换行和空格等价。

**读不到东西时的行为**：如果类型不匹配（比如该读 int 却来了个字母），`cin` 会进入失败状态，**后续所有 `cin` 都直接返回 0/空**，程序不会崩，而是拿着 0 一路算下去——这是最阴的 bug。防御写法：

```cpp
while (cin >> x) { ... }     // 读不到就退出循环，这是"读到文件末尾"的标准写法
```

洛谷/牛客的多组数据常用 `while (cin >> n)`，OJ 上用 `Ctrl+Z`（Windows）/`Ctrl+D`（Linux）结束输入。

---

## 3. 读整行：`getline`

```cpp
#include <iostream>
#include <string>
using namespace std;
int main() {
    int n;
    cin >> n;
    cin.ignore();                 // ★ 关键：吃掉 >> 留下的换行符
    string line;
    getline(cin, line);           // 读一整行，含空格，不含换行
    cout << "n=" << n << " line=[" << line << "]\n";
}
```

**为什么要 `cin.ignore()`**：`cin >> n` 读完数字后，那个换行符还留在缓冲区里，紧接着的 `getline` 会读到空字符串。这是新手在"先读 n 再读 n 行字符串"题型上最常见的翻车点。

**读中文**：`string` 按 UTF-8 存，`s.size()` 是**字节数**不是字数（一个汉字 3 字节），`s[i]` 拿到的是单个字节不是整个汉字。要按字符处理中文得用更复杂的编码处理——入门阶段记住"中文只用整体输出和 `find`，别按下标遍历"。

---

## 4. 格式化输出

```cpp
#include <iostream>
#include <iomanip>
using namespace std;
int main() {
    double v = 3.14159;
    cout << v << "\n";                              // 3.14159   默认 6 位有效数字
    cout << fixed << setprecision(2) << v << "\n";  // 3.14      固定 2 位小数
    cout << setw(8) << setfill('*') << 42 << "\n";  // ******42  宽度 8，右对齐
    cout << left << setw(8) << 42 << "|\n";         // 42******| 左对齐
    return 0;
}
```

| 操作符 | 作用 | 生效范围 |
|---|---|---|
| `fixed` | 小数记法（非科学计数） | **之后一直生效** |
| `setprecision(n)` | `fixed` 下表示小数位数；否则表示有效数字 | 之后一直生效 |
| `setw(n)` | 宽度 n | **只对紧跟的那一项生效** |
| `setfill(c)` | 填充字符 | 之后一直生效 |
| `left` / `right` | 对齐方向 | 之后一直生效 |
| `hex` / `dec` / `oct` | 进制 | 之后一直生效 |
| `showpoint` / `noshowpoint` | 强制显示小数点 | 之后一直生效 |

**`setw` 只作用一次**，循环里对齐要每次写 `setw`。

---

## 5. `endl` vs `'\n'`

```cpp
cout << "\n";      // 只写一个换行字符
cout << endl;      // 写换行 + 立即 flush（把缓冲区刷到屏幕/管道）
```

功能一样，性能差很多：输出 100 万行时，`endl` 会 flush 100 万次，可能慢好几倍。**循环里一律用 `'\n'`**，只在需要"立刻看到输出"（比如交互题）时用 `endl` 或 `cout.flush()`。

---

## 6. 大输入量的加速

```cpp
#include <iostream>
using namespace std;

int main() {
    ios::sync_with_stdio(false);   // 关掉与 C stdio 的同步
    cin.tie(nullptr);              // 取消 cin 强制刷新 cout
    long long sum = 0;
    int x;
    while (cin >> x) sum += x;     // 读到 EOF 为止：之后这样读就够快了
    cout << sum << "\n";
    return 0;
}
```

这两行能把 `cin` 的速度提升到接近 `scanf`。**代价**：加了之后就不能再和 `printf`/`scanf` 混用。

规模参考（本机实测：读 100 万个整数）：

| 方式 | 量级 |
|---|---|
| `cin` 默认（同步开着） | 慢，1e6 大约 100+ ms |
| `ios::sync_with_stdio(false)` + `cin` | 约 30 ms |
| `scanf("%d")` | 约 40 ms |
| 手写快速读入（`getchar` 版） | 约 20 ms，但没必要 |

结论：`n ≤ 1e6` 时加那两行就够；`n > 1e6` 且时限很紧才考虑手写读入。**输出**用 `cout` 加那两行同样快，或者攒进 `string` 一次输出。

---

## 7. 输出格式：OJ 上的隐形失分点

三种最常见的格式要求：

```cpp
// ① 数之间空格隔开
for (int i = 0; i < n; i++) {
    if (i) cout << ' ';
    cout << a[i];
}
cout << '\n';

// ② 每个后面都有空格（多数 OJ 也接受行尾多余空格）
for (int i = 0; i < n; i++) cout << a[i] << ' ';
cout << '\n';

// ③ 严格禁止行尾空格 → 用 ①，或者先拼 string
```

现代 OJ 一般**忽略行尾多余空格和末尾空行**，但"多打一行提示文字"（比如把 `请输入n：` 一起输出到 stdout）会直接 WA。**交题时 stdout 里只能有答案**，调试信息走 `cerr`（见 `14-coding-practices.md`）。

---

## 8. 自检三问

1. `cin >> n;` 之后紧接 `getline(cin, s);`，`s` 为什么是空的？怎么修？
2. `cout << setw(6) << a << b;`，`b` 会按宽度 6 输出吗？
3. 循环里输出 1e6 行，为什么不该用 `endl`？
