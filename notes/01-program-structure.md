# 01 · 程序结构与运行

> 对应练习：`ex01_types_io.cpp`
> 学完你应该能：解释一个 C++ 程序从上到下每一行的作用，独立编译运行，并看得懂常见报错。

---

## 1. 一个程序的四层结构

```cpp
#include <iostream>          // ① 预处理指令：把头文件内容贴进来
using namespace std;         // ② using 声明：省略 std:: 前缀
int main() {                 // ③ 函数定义：唯一的执行入口
    cout << "Hello, C++!" << endl;   // ④ 语句：真正干活的一行行代码
    return 0;
}
```

| 层 | 是什么 | 什么时候起作用 | 漏了会怎样 |
|---|---|---|---|
| `#include` | 预处理指令，编译前把文件内容原样插入 | **编译期**，不占运行时间 | 编译器不认识 `cout`，报 `‘cout’ was not declared` |
| `using namespace std;` | 名字空间声明 | 编译期的名字查找 | 得写 `std::cout`，否则同样报未声明 |
| `int main()` | 函数，操作系统约定从这里开始执行 | **运行期** | 链接时报 `undefined reference to 'main'` |
| `return 0;` | 返回值交给操作系统，0 表示正常结束 | 运行期结束时 | 可省略，`main` 的 `}` 会隐式返回 0 |

`main` 有固定签名，只有两种合法写法：`int main()` 和 `int main(int argc, char** argv)`（第二个参数是命令行参数，入门阶段用不到）。**不要**写 `void main()`——某些老教材这么写，标准不允许，新编译器会直接拒绝。

---

## 2. 从源码到运行：四个阶段

```
hello.cpp  ──①预处理──>  展开后的代码  ──②编译──>  汇编  ──③汇编──>  hello.o  ──④链接──>  hello.exe
              #include            翻译为机器指令      机器码        拼上库里的 cout 实现
              宏替换
              去掉注释
```

想亲眼看见每一步，在本仓库里可以：

```bash
g++ -std=c++17 -E hello.cpp -o build/hello.i    # 只看预处理结果（会膨胀到上万行）
g++ -std=c++17 -S hello.cpp -o build/hello.s    # 看生成的汇编
g++ -std=c++17 -c hello.cpp -o build/hello.o    # 只看编译，不链接
g++ -std=c++17 hello.cpp -o build/hello.exe     # 完整走一遍
./build/hello.exe
```

或者用仓库现成的：

```bash
make asm/ex01
```

**为什么要懂这个**：因为报错分两类。"error: ..." 出现在②之前，是语法问题；"undefined reference to ..." 出现在④，是链接问题（比如你把函数声明了却没写实现，或者少链接了某个 `.cpp` 文件）。**看到 undefined reference 就该想到"少了实现/少了文件"，而不是"语法写错了"**。

---

## 3. 语句、块与作用域

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 3;              // 声明语句
    a = a + 1;              // 表达式语句，分号是语句的一部分
    if (a > 3) {            // 复合语句（块）：一对花括号 = 一条语句
        cout << "big\n";    // 这里的 a 看得见
        int b = a * 2;      // b 只在这对花括号里活着
        cout << b << "\n";
    }
    // cout << b;           // 错：b 已经离开作用域被销毁
    return 0;
}
```

三条规矩：

1. **分号不能省**。`int a = 3` 后面漏分号，报错常常指向**下一行**——这是初学者最困惑的地方，记住"报错行不一定是真正的出错行，往前一行找"。
2. **花括号必须成对**。`if (x) a = 1; b = 2;` 里只有 `a = 1` 受 `if` 管，`b = 2` 总会执行。哪怕块里只有一条语句，也建议写上花括号。
3. **变量从声明处开始生效**，到所在块结束销毁。所以 `int a = 1; ... a = a + 1;` 里的后一个 `a` 必须是已经声明过的。

---

## 4. 注释与可读性

```cpp
// 单行注释：从 // 到行尾
/* 块注释：可以跨行，但不能嵌套 */
```

注释写"为什么"，不写"是什么"。`i++; // i 加一` 是噪音；`i++; // 跳过表头行` 才有价值。

**调试小技巧**：`/* */` 可以整段包住注释，但里面**不能已有 `*/`**。想临时屏蔽大段代码，用编辑器快捷键（VS Code 是 `Ctrl+/`）逐行加 `//` 更安全。

---

## 5. 编译命令与常用选项

本仓库统一用：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 文件名.cpp -o build/文件名.exe
```

| 选项 | 作用 | 什么时候改 |
|---|---|---|
| `-std=c++17` | 语言标准，决定能用哪些语法 | 固定不动。C++11/14 的老写法基本都能编 |
| `-Wall -Wextra` | 打开绝大多数警告 | **一直开着**。警告是免费的 bug 提示 |
| `-Wpedantic` | 严格按标准，编译器扩展也报 | 交作业/换环境时有用 |
| `-g` | 生成调试信息，gdb/VS Code 才能断点 | 提交前可以去掉，不影响运行 |
| `-O0` | 不优化，代码和源码一一对应 | **调试用 -O0，交题用 -O2**。优化后单步会乱跳 |
| `-fsanitize=address,undefined` | 运行时抓越界、空指针、有符号溢出 | 程序行为诡异时立刻加上重跑 |
| `-lxxx` | 链接某个库 | 入门阶段用不到 |

`-o` 后面的名字：Windows 下带 `.exe`，Linux/macOS 下不带。课堂在线 IDE 一般只需点"运行"，它在背后跑的就是 `g++ -std=c++17 ...`。

---

## 6. 报错怎么读

一条真实的 g++ 报错长这样：

```
ex01.cpp:12:10: error: 'cout' was not declared in this scope
   12 |     cout << "hi" << endl;
      |          ^~~~
```

拆开看：`文件:行:列: 级别: 说明`，然后**把你的代码原样贴出来并在出错位置画波浪线**。读报错的顺序：

1. **看级别**：`error` 必须改；`warning` 也建议改（本仓库故意用 `-Wall -Wextra -Wpedantic` 把警告全暴露出来）。
2. **看第一个 error**：编译器会在一个错误后努力继续，往往一次报几十个。**只改第一个**，改完重新编译，其余常常一起消失。
3. **看波浪线指的那个符号**，而不是行号本身——列号才是精确位置。

常见错误对照表：

| 报错关键词 | 真实原因 | 修法 |
|---|---|---|
| `'cout' was not declared` | 少 `#include <iostream>` 或少 `using namespace std;` | 补上，或写 `std::cout` |
| `expected ';' before ...` | **上一行**漏分号 | 往上找一行 |
| `expected '}' at end of input` | 花括号没配对 | 用编辑器的括号高亮逐个核对 |
| `no match for 'operator<<'` | 类型不匹配，比如 `cout << 数组名` | 改用循环输出，或输出 `.size()` |
| `undefined reference to 'main'` | `main` 拼错（如 `mian`）或整个文件没有 main | 检查拼写 |
| `undefined reference to 'add(int,int)'` | 声明了函数但没写实现 | 补实现或把 `.cpp` 一起编译 |
| `'m' was not declared in this scope` | 变量没声明就用；或作用域已过 | 在使用前声明 |
| `expected primary-expression before ')'` | 括号里表达式不完整，常是多余逗号 | 看波浪线列号 |

**运行期**的问题（编译通过但结果不对/程序崩）见 `14-coding-practices.md`。

---

## 7. 本仓库怎么用

每个练习都是一个**独立完整程序**，做完一个跑一个：

```bash
make run/ex01          # 编译 + 运行
make san/ex01          # 带 ASan/UBSan 运行，抓越界
```

Windows 上如果刚编译完立刻运行报 `Permission denied`，是 Defender 实时扫描短暂锁住了新生成的 `.exe`，等一秒再跑即可，不是代码问题。

---

## 8. 自检三问

1. `#include <iostream>` 和 `#include "myheader.h"` 的尖括号/引号区别是什么？（前者在系统头文件目录找，后者先在当前目录找）
2. 为什么 `return 0;` 可以不写，但 `int main` 的 `int` 不能省？
3. 编译报 `undefined reference`，是语法错了还是少了东西？

三问答不上来，重读本文件的第 1、2、6 节。
