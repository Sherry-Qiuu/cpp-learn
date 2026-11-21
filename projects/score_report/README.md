# 项目 1 · 学生成绩报表（多文件 C++ 工程）

**这是什么**：把 notes/06（vector）、notes/07（struct）、notes/08（多级排序）、notes/03（iomanip）学到的东西，拼成一个**真正的多文件项目**：两个头文件、三个编译单元、一张 Makefile。

重点不是「算成绩」——那点儿逻辑 20 行就写完了。重点是**工程化**：

- 接口（.h）和实现（.cpp）怎么分，为什么分了之后改一行不用全量重编
- 依赖方向为什么必须是 `main → report → student`，不许反过来
- 多关键字排序、并列名次、对齐表格这些「看起来简单、写起来全是细节」的东西
- 用 `assert` 把「我觉得对了」变成「机器证明它对了」

数据是**内置的样例**（8 个人 × 4 门课），程序不读键盘也不读文件，`make run` 非交互、几秒钟跑完。

---

## 目录结构

```
projects/score_report/
├── student.h         数据模型的接口：struct Student、科目常量、纯计算函数声明
├── student.cpp       数据模型的实现：总分/平均分/最值/挂科数 + 内置样例数据
├── report.h          报表的接口：比较规则、名次规则、列宽常量
├── report.cpp        报表的实现：排序、并列名次、逐科统计、iomanip 排版
├── main.cpp          使用方 + 测试驱动：跑样例 + 一整套 assert
├── Makefile          分离编译规则：.cpp -> .o -> .exe
└── build/            产物目录（.o / .d / .exe），已被 .gitignore 忽略，不要提交
```

---

## 怎么用

```bash
# 在仓库根目录
make p1                     # 编译 + 运行（推荐，这条就是验收命令）
make clean                  # 连项目产物一起清（会调本目录的 make clean）

# 或者在项目目录里
cd projects/score_report
make            # = make all，只编译不运行
make run        # 编译并运行
make clean      # 删掉 build/ 里的东西
make rebuild    # clean + all
make help       # 列出所有目标
```

Windows 上如果 `make` 不认，用 `mingw32-make`（同一个程序，见下面报错速查）。

第一次跑 `make p1`，你会看到四行构建输出 —— **这四行就是本项目要你学的东西**：

```
  [CXX]  student.cpp -> build/student.o
  [CXX]  report.cpp  -> build/report.o
  [CXX]  main.cpp    -> build/main.o
  [LINK] build/student.o build/report.o build/main.o -> build/score_report.exe
```

---

## 每个文件负责什么

| 文件 | 角色 | 里面有什么 |
|---|---|---|
| `student.h` | **合同**：这张表长什么样 | include 守卫、`struct Student`、`kSubjectCount`、纯计算函数**声明** |
| `student.cpp` | 数据模型的实现 | 总分/平均分/最高/最低/最高分科目/挂科数 + `sampleStudents()`。**一行 cout 都没有** |
| `report.h` | 合同：报表的三条规则 | `beforeInReport`、`computeRanks`、`reportRow`、列宽常量 |
| `report.cpp` | 规则的实现 | `sort`、并列名次、逐科统计、`iomanip` 排版。全项目**唯一**碰 iostream 的文件 |
| `main.cpp` | 使用方 + 测试 | 只 include 两个 `.h`；4 组测试函数、64 处 `assert`，最后打印报表 |
| `Makefile` | 构建规则 | 每个 `.cpp` 单独出 `.o`，再链接；`-MMD -MP` 自动生成头文件依赖 |

依赖方向单向：`main.cpp → report.h → student.h`。`student.cpp` 不知道 `report` 存在，
所以你可以把 `student.*` 原样搬进另一个项目，只换报表。

---

## 关键代码逐段讲解

### 1. 头文件守卫，以及 `<>` 与 `""`

```cpp
#ifndef STUDENT_H
#define STUDENT_H
...
#endif
```

`#include` 是**原样把文件内容贴进来**。`report.cpp` 贴了 `report.h`，而 `report.h` 又贴了 `student.h`；
`main.cpp` 两个都 include，`struct Student` 就会被贴两遍 → `redefinition of 'struct Student'`。
守卫的作用：第二次进来宏已经定义了，整段跳过。`#pragma once` 效果一样，但它不是标准的东西，
本仓库统一用 `#ifndef`（嵌入式 IDE 里躺着上古编译器的概率不低）。

`#include <iostream>` 只去系统头文件目录找；`#include "student.h"` **先在当前目录找**，找不到才退到系统目录。
写反了通常也能编过，但这两个符号同时是给人看的记号：`""` = 我项目里的文件，`<>` = 语言/库自带的。

还有两件事**没做**，都是有意的：

- 头文件里没有函数定义（带函数体的那种）。一旦写了，每个 include 它的 `.cpp` 都生成一份副本，
  链接时立刻 `multiple definition`。
- 头文件里不写 `using namespace std;`。它会顺着 `#include` 污染所有包含者，你关不掉。
  所以 `student.h` / `report.h` 里一律写 `std::string`，`.cpp` 里才用 `using namespace std;`。

### 2. 多级比较函数：为什么是「逐层 if，最后一个 return」

```cpp
bool beforeInReport(const Student& a, const Student& b)
{
    const int ta = totalOf(a), tb = totalOf(b);
    if (ta != tb) return ta > tb;                   // 第一关键字：总分降序
    if (a.name != b.name) return a.name < b.name;   // 第二关键字：姓名字典升序
    return a.id < b.id;                             // 第三关键字：学号升序
}
```

三个关键字写**两个** if，最后一个直接 return。每一层的职责只有一句话：
**「这一项分出胜负了吗？没有就把决定权交给下一层。」**

- 里面**只有 `<` 和 `>`，一次 `<=` 都没有**。写成 `ta >= tb` 时 `cmp(a, a)` 返回 true，
  等于告诉 `sort`「a 该排在 a 自己前面」，严格弱序被破坏 ——
  8 个人的数据看着完全正常，8 万个人直接 RE。`main.cpp` 里那条
  `assert(!beforeInReport(s, s))` 就是把这条铁律钉成可执行的检查。
- 参数是 `const Student&`。比较函数被调 n log n 次，传值等于每次拷两个 `string` 加两个 `vector`，
  `sort` 里就是几百万次堆分配。结构体里有 `string`/`vector`，参数一律 `const T&`，没有例外。
- 第三个关键字不是凑数：样例里躺着**两个同名同总分的 Kevin**（2024007 / 2024008），
  只有学号能稳定分开他们。少写这一层，`sort` 给出的顺序由算法实现决定，换台机器就变。

### 3. 名次为什么要并列（1,2,2,4 而不是 1,2,3,4）

```cpp
if (i > 0 && totalOf(sorted[i]) == totalOf(sorted[i - 1]))
    ranks[i] = ranks[i - 1];    // 同分：并列，名次不前进
else
    ranks[i] = i + 1;           // 分高了：名次 = 位置号
```

排第 2 的有两个人，第三个人就该是第 **4** 名 —— 「第 3 名」这个人压根不存在，
把他叫第 3 名等于凭空少了一名。这就是考试排名、竞赛榜单的通用做法。

代码上它只是一句 `ranks[i] = ranks[i-1]`，但**前提是数组已经按总分排好且并列的人相邻**，
所以名次和排序规则是绑死的：改了比较函数必须回头看 `computeRanks`。
`main.cpp` 里三条断言在盯这件事：名次不后退、不比自己的位置更差、同分必须同名次。

另外注意：并列比的是**总分**，不是「整个对象一样」。两个 Kevin 分数分布完全不同，
总分一样就并列 —— 名次回答「你排第几」，不回答「你是谁」。

### 4. 统计为什么按行遍历

```cpp
for (const Student& s : v)                     // 外层：人（一行）
    for (int j = 0; j < ...; j++) sum[j] += s.scores[j];   // 内层：往各科的累加器上加
```

`vector<Student>` 在内存里是一个学生紧挨着下一个学生排的。按行走是**顺序扫内存**，
CPU 缓存一条直线命中；反过来「外层科目、内层学生」每次都要跳过一整行，
在内存里来回折返跑，而且同一份数据要读 `kSubjectCount` 遍。

这里只有 8 行，两种写法测不出差别。但「一次行遍历 + 把每列的累加器摊在一个数组上」
本身就是标准写法：数据只过一遍，将来加一列统计（最高分、及格人数）只是多开一个数组。

### 5. 表格怎么做到「行尾零空格」

```cpp
os << " " << setw(kTotalWidth) << totalOf(s)
   << " " << fixed << setprecision(2) << setw(kAvgWidth) << avgOf(s);
```

`setw` **只对紧跟的那一项生效**（`setfill` / `left` / `right` / `fixed` 会一直生效）。
所以循环里每一格都得重写 `setw`。

行尾空格的成因永远是「最后一格用了左对齐」—— `left << setw(10) << name` 会在名字右边补空格补满 10 列。
**把最后一列设成右对齐**，空格就全补在数字左边，行尾一个都不剩。
`main.cpp` 里把每行拼成 `string` 就是为了能检查这件事：

```cpp
assert(line[line.size() - 1] != ' ');   // 最后一个字符不能是空格
assert(line.size() == rowLen);          // 而且每行等长 = 列真的对齐了
```

打印出去的东西收不回来，拼成字符串才审得掉。这是本文件里唯一一处「为了可测试性而改变设计」的地方。

实测输出（`make run` 的第 4 节，8 行都是 72 字符）：

```
Rank        ID  Name     Chinese    Math English Science   Total     Avg
------------------------------------------------------------------------
   1   2024004  David         95      95      95      95     380   95.00
   2   2024002  Bob           95      78      88      91     352   88.00
   2   2024006  Fiona         90      88      94      80     352   88.00
   4   2024001  Alice         88      92      79      85     344   86.00
   5   2024007  Kevin         85      85      84      80     334   83.50
   5   2024008  Kevin         70      80      90      94     334   83.50
   7   2024003  Cindy         72      99      65      80     316   79.00
   8   2024005  Eric          60      55      78      90     283   70.75
```

名字为什么用英文不用中文：`setw` 按**字节**算宽度，一个汉字 3 字节（notes/03 第 3 节），
用中文名列立刻歪成锯齿。

### 6. Makefile 里那三条规则

```make
OBJS := $(addprefix build/,$(SRCS:.cpp=.o))

build/%.o: %.cpp | build
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@        # 编译：一个 .cpp 出一个 .o

score_report.exe: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@               # 链接：把 .o 拼成 exe
```

- `-c` 只编译不链接。编译 `main.cpp` 时，`totalOf` 只是一个「声明」，
  生成的是「跳转去符号 totalOf」的指令，地址留给链接器填。
  自己验证：`nm build/main.o | grep totalOf` 看到的是 `U`（未定义，要别人给），
  `nm build/student.o` 里是 `T`（本文件已定义）。链接器干的就是撮合 U 和 T 这件事。
- `| build` 是 order-only 前提：只要求目录存在，目录时间戳变了不触发重编。
- `-MMD -MP` 让 g++ 顺手生成 `build/xxx.d`，里面写着「这个 .o 依赖哪些头文件」，
  `-include $(DEPS)` 读进来之后，**改头文件就会自动重编对应的 .o**，不用手写依赖。
  实测：`touch student.h && make` 重编三个 `.o`；`touch report.h` 只重编 `report.o` 和 `main.o`；
  `touch student.cpp` 只重编 `student.o`。项目大了这就是几分钟和几秒钟的差别。

---

## 验收标准

```bash
make p1        # 在仓库根目录
```

要求同时满足三件事：**零警告**（`-Wall -Wextra -Wpedantic` 全开）、**退出码 0**、
最后一行打印 `>>> score_report 全部通过`。少任何一条都算没做完。

`assert` 失败时长这样，文件号和行号直接给你：

```
Assertion failed: ranks[i] == wantRank[i], file main.cpp, line 205
```

**这就是本仓库的验收方式**：不靠你肉眼觉得「差不多对」，靠机器停在错的那一行。
（顺带一条规矩：`assert` 里的表达式不能有副作用 —— 加了 `-DNDEBUG` 整条语句会消失。
所以本文件一律先算进变量，再 assert 那个变量。）

---

## 你要动手做的 5 件事

按顺序做，每做完一件跑一次 `make p1`。**别改完三个再编译**，那时你分不清是哪一处改坏的。

### 任务 1：加第 5 门课

给每个学生补一门 "History" 成绩，让 `kSubjectCount` 变成 5，表格和统计跟着自动多一列。

- **期望**：表头多一个 `History` 列，分隔线跟着变长，8 行仍然等长、行尾仍无空格；
  `David` 那行总分从 380 变成 `380 + 他的历史分`；`main.cpp` 里 `assert(byStudent == 2695)`
  会先炸掉 —— **它就是用来提醒你「数据变了，所有期望值都要重算」的**，把它改成新实测值。
- **提示**：先 `grep -n "88, 92, 79, 85" student.cpp`，再想想哪些地方写死了 4。
  如果你的代码里有哪一处改一门课就要动三个地方，说明抽象没做好。

### 任务 2：改成「同分不跳名」

把 `computeRanks` 改成密集名次（dense ranking）：并列之后名次连续，1,2,2,**3** 而不是 1,2,2,4。

- **期望**：报表的名次列变成 `1 2 2 3 4 4 5 6`；`main.cpp` 里三人同分那组断言变成 `1,1,1,2`；
  「名次不比自己的位置更差」那条断言依然成立。
- **提示**：名次不能再取 `i + 1` 了，它应该等于「比你分数更高的人有几个 + 1」，
  或者等价地：遇到不同分就让计数器 +1，而不是跳到位置号。

### 任务 3：加一列挂科警告

在表格最后加一列：有科目低于 60 就印 `WARN`，否则印 `OK`（`failedCount` 已经在 `student.h` 里了）。

- **期望**：Eric 那行行尾是 `WARN`，其余 7 行是 `OK`；`reportRow` 的长度断言从 72 变成新值，
  并且 `assert(line[line.size() - 1] != ' ')` **必须继续通过** ——
  这列是字符串，左对齐就会在行尾留空格，你想办法处理。
- **提示**：要么把这列当最后一格时用右对齐，要么先拼再裁。别急着怪 `setw`。

### 任务 4：换成「排下标」，原数据不许被改动

新增 `std::vector<int> orderForReport(const std::vector<Student>& v)`，
返回顺序下标（`order[0]` 是该排第一的人在原表里的下标），让 `printReport` 用它打印；
`main.cpp` 里断言打印完之后的 `cls` 仍然和 `sampleStudents()` 一模一样。

- **期望**：屏幕上表格一模一样，但多了一条 `assert(cls[i].id == wantId[i])`（**按学号**的顺序）通过 ——
  证明源数据没被排过序。
- **提示**：`notes/08` 第 5 节的 `iota`（在 `<numeric>` 里）+ 传下标进 `sort`；
  比较器里从 `[i]`、`[j]` 取人。想想为什么「同一份数据要按分数排一次、按学号再排一次」时这个写法更省。

### 任务 5：亲手把稳定性搞坏一次

把 `sortForReport` 里的 `sort` 换成 `stable_sort`，比较函数只留总分一层（把名字和学号那两个判断删掉），
然后把 `sampleStudents()` 里两个 Kevin 的书写顺序调换（2024008 写在前面）。

- **期望**：`stable_sort` 版输出的第一个 Kevin 是 `2024008`，
  而改回 `sort` 之后永远是 `2024007`（第三关键字学号升序）——
  屏幕上的输出**换了算法就会变**，这就是「依赖稳定性」的代价。
- **提示**：先预测再动手：`sort` 那版第一个 Kevin 是谁？跑完对照一下。
  结论记牢：**凡是「同分要按输入顺序」的题，把序号写进比较函数比换 `stable_sort` 更稳**
  （notes/08 第 6 节末尾）。跑完记得把代码改回来。

---

## 常见报错速查

| 现象 | 真实原因 | 怎么办 |
|---|---|---|
| `Makefile:23: *** missing separator. Stop.` | Makefile 被存成了 **CRLF**，或者命令行用了**空格**缩进 | 编辑器右下角把换行切成 `LF`；recipe 行首必须是 Tab（`.gitattributes` 里已经写了 `* text=auto eol=lf`） |
| `Permission denied` / `cannot execute binary file`，而且**是刚编译完第一件事** | Windows Defender 实时扫描临时锁住新生成的 `.exe` | 等 1 秒再跑一次，不是代码问题 |
| `'make' 不是内部或外部命令` | 这台机器上只有 `mingw32-make` | 用 `mingw32-make p1`，它和 `make` 是**同一个 GNU Make**，只是 MinGW 为了不和 MSVC 的 `nmake` 撞名改了叫法。嫌麻烦就 `alias make=mingw32-make`（写进 `~/.bashrc`） |
| `undefined reference to 'totalOf(Student const&)'` | 少链接了 `student.cpp`（`SRCS` 里没写），或者你只声明了函数没写实现 | 先看 `SRCS`，再看 `.cpp` 里的函数**签名**是否和 `.h` 里逐字符一致（少个 `const` 就是另一个函数） |
| `multiple definition of 'xxx'` | 把函数/变量的**定义**写进了头文件 | 头文件只留声明，定义搬回 `.cpp` |
| `redefinition of 'struct Student'` | include 守卫漏了，或两个头文件用了**同名**的宏（都叫 `HEADER_H`） | 宏名跟着文件名走：`STUDENT_H` / `REPORT_H` |
| `'Student' was not declared in this scope` | `report.h` 忘了 `#include "student.h"` | 头文件要**自包含**：用到谁就 include 谁，别指望调用方帮你包 |
| 表格歪了、列对不齐 | 用了中文列名（`setw` 按字节算），或者循环里漏写 `setw` | `setw` 只对紧跟的那一项生效，一格一次 |
| `84.125` 印成 `84.12` | `setprecision(2)` 的正常舍入（IEEE 的 round-half-even），不是 bug | 别把打印值拿去和字符串比；要比大小就用 `<cmath>` 的 `fabs(a - b) < 1e-9` |
| 加了 `-w`／`-w no` 想把警告关掉 | 你在藏 bug，不是在解决 bug | **不许关警告**，本项目要求 `-Wall -Wextra -Wpedantic` 输出为空。看不懂的警告就逐字读它给的第一句 |
| `make run` 什么都不做，但程序明显有问题 | 目标没重建（时间戳没变） | `make rebuild` |

---

## 做完之后，自己想三个问题

1. `reportRow` 返回 `string`、`subjectAverages` 返回 `vector<double>`，
   每次都新建一个对象返回，为什么不算「低效」？（提示：返回值优化 / 移动，以及「先保证能测，再谈快」）
2. 如果这张表要显示在 STM32 的 128×64 屏上（只能显示 16 列 ASCII），
   现在 `report.*` 里哪些东西必须重写？`student.*` 要动吗？（这就是把「数据」和「展示」分开的意义）
3. 现在数据是 `sampleStudents()` 硬编码的。如果改成从文件读，第一个要处理的边界是什么？
   （提示：`main.cpp` 里那些「空表也要给明确答案」的断言，不是在凑数）
