# C++ 4 天冲刺 · 竞赛入门 + 嵌入式（STM32）前置

> 你的环境**已经全部就绪，本包不需要安装任何东西**：

| 工具 | 位置 / 版本 |
|---|---|
| g++ | `C:\Users\lenovo\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\g++.exe`（MinGW-W64 16.1.0，ucrt-posix-seh） |
| gdb / cmake / ninja / mingw32-make | 同上目录 |
| VS Code | 1.147.0，已装 `ms-vscode.cpptools` 扩展包（1.35.6） |
| 课堂在线 IDE | GCC 13.1.0 / `-std=c++17`（本包所有代码两边都能编过） |

这份仓库把「C++ 基础 7 板块 + 函数与递归 + 排序与自定义比较 + 基础算法思路 + 复杂度分析 + DFS/BFS + STL 容器 + DP 入门 + 竞赛代码习惯」全部讲透，并配了 **14 个带自测断言的练习**和 **2 个多文件项目**。所有代码都在你这台机器上真编译、真运行过——练习答案块零警告、断言全过，两个项目 `make p1` / `make p2` 零警告退出 0。

---

## 目录结构

```
cpp-learn/
├── README.md              本文件：日程表 + 规矩 + 常见问题
├── Makefile               一键编译 / 运行 / 严格模式 / 看汇编
├── notes/                 15 篇讲义，每篇都指向对应练习
│   ├── 01-program-structure.md    程序结构、编译四步、作用域
│   ├── 02-variables-types-operators.md  类型、转换、整数除法、溢出
│   ├── 03-input-output.md         cin/cout、格式化、加速
│   ├── 04-conditions-loops.md     if/for/while、break/continue、模拟
│   ├── 05-functions-recursion.md  三种传参、递归三要素、记忆化
│   ├── 06-arrays-strings-vectors.md  数组退化、二维数组、string、vector
│   ├── 07-structs.md              struct、聚合初始化、字段默认值
│   ├── 08-sorting-comparators.md  sort、比较函数、严格弱序、lambda
│   ├── 09-algorithm-patterns.md   模拟/枚举/排序/前缀和/差分/二分/贪心
│   ├── 10-complexity.md           O(?) 能不能过，怎么估算
│   ├── 11-search-dfs-bfs.md       状态、搜索空间、DFS/BFS、剪枝
│   ├── 12-stl-containers.md       stack/queue/deque/set/map/priority_queue
│   ├── 13-dp-intro.md             状态定义、转移、初始化、四种经典模型
│   ├── 14-coding-practices.md     边界、调试、对拍、竞赛代码习惯
│   └── cheatsheet_cpp.md          全仓速查（最后一天用）
├── exercises/             14 个练习，每个 TODO + assert + 末尾参考答案
│   ├── ex01_types_io.cpp          类型 · 转换 · 整数除法 · 格式化
│   ├── ex02_control_flow.cpp      条件与循环 · 模拟 · off-by-one
│   ├── ex03_functions.cpp         三种传参 · 重载 · 拆函数
│   ├── ex04_recursion.cpp         递归三要素 · 汉诺塔 · 记忆化
│   ├── ex05_arrays_vectors.cpp    数组/二维/vector · 越界陷阱
│   ├── ex06_strings.cpp           string · 分词 · 解析
│   ├── ex07_structs_sorting.cpp   struct · 多级排序 · 严格弱序
│   ├── ex08_prefix_sum_diff.cpp   一维/二维前缀和 · 差分
│   ├── ex09_binary_search.cpp     手写二分 · lower_bound · 二分答案
│   ├── ex10_enum_greedy.cpp       枚举 · 双指针 · 贪心与反例
│   ├── ex11_dfs_backtrack.cpp     全排列 · N 皇后 · 剪枝对比
│   ├── ex12_bfs_containers.cpp    BFS 层序 · STL 六种容器
│   ├── ex13_dp.cpp                爬楼梯/路径/背包/LIS/Kadane/LCS
│   └── ex14_contest_drill.cpp     综合演练 · 边界 · 对拍 · 输出习惯
├── projects/
│   ├── score_report/      项目 1：多文件成绩报表（练工程化、分离编译）
│   └── maze_runner/       项目 2：迷宫求解（DFS + BFS + 连通块收官）
├── build/                 编译产物（可随时整个删掉，不进仓库）
└── .vscode/               编译 / 调试 / 严格模式任务，已配好复用现成 g++
```

---

## 怎么用

在 **Git Bash** 里。注意本机 `make` 的真名是 `mingw32-make`（见下面常见问题），下面统一写 `make`：

```bash
cd "/d/ProgramData/work Buddy/CPU/cpp-learn"

make list                # 列出所有练习
make run/ex01            # 编译并运行 ex01（短名、全名都认：run/ex01_types_io 也行）
make all-ex              # 只编译全部练习，看有没有语法错
make run-all             # 编译并依次跑完全部练习（答案写完之后用这个验收）
make strict/ex05         # 严格模式：-Wconversion -Wsign-conversion -Wshadow -fanalyzer
make asm/ex05            # 生成 build/ex05.s，看编译器怎么理解你的循环
make san/ex05            # ASan+UBSan（本机 MinGW 缺库，会明确告诉你，见常见问题）
make p1                  # 编译并运行 projects/score_report
make p2                  # 编译并运行 projects/maze_runner
make clean               # 清空 build/ 和两个项目的产物，源码不动
```

不想用 make，直接一条命令也行：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex01_types_io.cpp -o build/ex01.exe && ./build/ex01.exe
```

VS Code 里：打开 `cpp-learn` 文件夹，`Ctrl+Shift+B` 编译当前文件，`F5` 用 gdb 单步调试，`Ctrl+Shift+P → Tasks: Run Task` 里能选「严格模式编译」和「看汇编」。

---

## 4 天日程（按 8~9 小时/天 排）

### Day 1 — 语法与工具链：把「能跑」变成「明白为什么能跑」

| 时段 | 时长 | 内容 | 对应文件 |
|---|---|---|---|
| ① | 1.5h | 读 notes 01–03，把里面每个完整程序敲一遍（不许复制粘贴） | `notes/01`~`03` |
| ② | 2h | 类型、转换、整数除法、格式化输出，6 个 TODO | `ex01` |
| ③ | 1.5h | 条件与循环、模拟、off-by-one 与无符号下溢 | `notes/04` + `ex02` |
| ④ | 2h | 三种传参方式、重载、把长 main 拆成函数 | `notes/05` 前半 + `ex03` |
| ⑤ | 1.5h | `make strict/ex01`、`make asm/ex01`，把每条警告和一段汇编看懂 | — |

### Day 2 — 数据与内存：递归、数组、字符串、结构体

| 时段 | 时长 | 内容 | 对应文件 |
|---|---|---|---|
| ① | 2h | 递归三要素、调用栈、记忆化；顺带理解为什么会爆栈 | `notes/05` 后半 + `ex04` |
| ② | 2.5h | 数组退化、二维遍历顺序、vector、越界与 `size()` 无符号陷阱 | `notes/06` + `ex05` |
| ③ | 2h | string 全家族：分词、解析、字典序、回文 | `ex06` |
| ④ | 1h | struct、聚合初始化、字段默认值 | `notes/07` + `ex07` 前半 |
| ⑤ | 1.5h | 补 Day1 里没吃透的警告；把 ex01–ex04 的断言重跑一遍 | — |

### Day 3 — 算法思路与复杂度：把「写得出」升级成「算得过来」

| 时段 | 时长 | 内容 | 对应文件 |
|---|---|---|---|
| ① | 2h | sort 与比较函数、严格弱序、lambda、多级排序 | `notes/08` + `ex07` 收尾 |
| ② | 2.5h | 前缀和、二维前缀和、差分——把 O(nm) 干成 O(n+m) | `notes/09` + `ex08` |
| ③ | 2h | 手写二分、lower_bound 语义、二分答案（make-it-check-it） | `ex09` |
| ④ | 1h | 复杂度估算表：什么规模该用什么复杂度 | `notes/10` |
| ⑤ | 1.5h | 枚举 + 贪心，包括贪心不成立的反例 | `ex10` |

### Day 4 — 搜索、容器、DP 与项目：把知识点串成解题能力

| 时段 | 时长 | 内容 | 对应文件 |
|---|---|---|---|
| ① | 2h | DFS、回溯、剪枝；N 皇后与访问次数对比 | `notes/11` + `ex11` |
| ② | 2h | BFS 与最短步数、六种 STL 容器的选型 | `notes/12` + `ex12` |
| ③ | 2h | DP 四步（状态/转移/初始化/答案），01 背包为什么倒序 | `notes/13` + `ex13` |
| ④ | 1h | 边界清单、调试手法、对拍模板 | `notes/14` + `ex14` |
| ⑤ | 2h | 项目 1 + 项目 2，各自跑通并做完 README 里的进阶任务 | `projects/*` |

只有 2 天的话：Day 1 走完本表 Day 1–2 的 ①②③④，Day 2 走完 Day 3–4 的 ①②③⑤；`ex06`、`ex10`、`ex14` 和两个项目的进阶任务留到后面补。**别跳过 `ex08`、`ex09`、`ex13`**——这三章是竞赛里真正拿分的地方。

---

## 规矩（很重要）

1. **每个练习末尾有自测断言，全部 assert 通过才算做完。** 失败会打印 `Assertion failed: ... file ..., line ...`，照着行号看，它比你的直觉可靠。
2. **卡住先自己想 20 分钟**，再翻文件末尾的 `参考答案` 注释块。答案块里除了代码还有几段「为什么要这样写」，那部分才是价值。
3. 每写完一个练习，`make run/exNN` 必须**零警告**通过。**警告一个都别放过**——`-Wall -Wextra -Wpedantic` 报出来的每一条，在竞赛里是 WA，在单片机上是死机。**不要用 `-w` 把警告关掉。**
4. 想再看紧一点就用 `make strict/exNN`（多开 `-Wconversion -Wsign-conversion -Wshadow -Wcast-qual -fanalyzer`）。它会报一堆 `int`↔`size_t` 的转换警告，这是**信号不是错误**，逐条判断哪些要改、哪些是教学代码故意留的。
5. 所有代码用 `-std=c++17`。课堂在线 IDE 是 GCC 13.1.0，本机是 16.1.0，这个标准两边一致。
6. 没实现 TODO 之前会有一堆 `unused parameter` 警告。**这是故意的信号**，别用 `(void)x` 把它按下去。写实现的格式：把「TODO 在这里写实现」注释行和它下面那行占位 `return` 一起替换掉。
7. 每个练习的 `cout` 观察题：**先在纸上写答案，再跑**。跑出来的和你猜的不一样，那一条才是你今天真正学到的。

---

## 两个项目（Day 4 ⑤）

- `projects/score_report/` —— 多文件成绩报表：头文件守卫、声明与定义分离、分离编译、`.d` 依赖、Makefile、结构体多级排序、并列名次、对齐表格。`make p1` 跑通 + 64 条 assert。
- `projects/maze_runner/` —— 迷宫求解：DFS 判可达、BFS 最短步数 + 前驱表还原路径、连通块计数、多源 BFS；156 条 assert，里面有一个「逐步校验路径合法性」的函数，是本项目最值得读的一段。

两个项目的代码**都已经过编译 + 运行验证**，各自 README 里有逐段讲解和 5 个进阶任务（带期望输出，不带答案）。你写完之后如果 `make run` 卡住，先对照 README 的「常见报错速查」。

---

## 常见问题

**Q：`make` 找不到？**
本机随 MinGW 装的是 `mingw32-make`，它和 Linux 的 `make` 是同一个程序。加个别名永久生效：

```bash
echo "alias make=mingw32-make" >> ~/.bashrc && source ~/.bashrc
```

**Q：刚编译完立刻运行，报 `Permission denied`？**
Windows Defender 实时扫描会短暂锁住新生成的 `.exe`。等一两秒再跑一次就好，不影响正确性，只是时序问题。

**Q：`make san/exNN` 报 `cannot find -lasan` / `-lubsan`？**
这是本机 MinGW 打包时没带 sanitizer 运行库，**不是你的代码问题**。本机抓 UB 的可靠手段是 `-Wall -Wextra -Wpedantic`（第一层）+ `make strict/exNN` 的 `-fanalyzer`（静态分析层）+ 把危险写法控制在 assert 覆盖范围内。想用 ASan 就去 WSL/Linux 或 MSVC，代码不用改。

**Q：中文在控制台里乱码？**
Git Bash 默认 UTF-8，一般没问题。如果用的是 cmd.exe，先执行 `chcp 65001`。

**Q：assert 里写 `f(a, b) == c` 报「macro needs parens」之类的怪错？**
断言表达式里带逗号的模板参数（如 `assert(is_same<pair<int,int>, X>::value)`）要用外层括号包起来：`assert((expr))`。练习里已经这么写了。

**Q：我能不能直接看参考答案？**
可以，但先自己想 20 分钟。这些练习的价值在「卡住的过程」，不在「看完答案」。看完答案务必**关掉它自己重敲一遍**。

**Q：为什么 `1e9 + 7` 不能写成 `const int MOD`？**
`1e9` 是 `double`，`1e9 + 7` 会先做浮点加法再窄化回 `int`。值虽然刚好相等，但写法本身在诱导 bug——写 `1000000007`。讲义 notes/02 里专门讲了这条。

---

## 同步到 GitHub

这个目录可以直接做成你自己的仓库。三条命令（把 `<你的用户名>` 换成真实账号）：

```bash
cd "/d/ProgramData/work Buddy/CPU/cpp-learn"
git init -b main
git add .gitignore .gitattributes LICENSE README.md Makefile .vscode notes exercises projects
git commit -m "C++ 4 天冲刺：讲义 15 篇 + 练习 14 个 + 项目 2 个"
git remote add origin https://github.com/<你的用户名>/cpp-learn.git
git push -u origin main
```

几点说明：

- **编译产物不会进仓库。** `.gitignore` 已经排掉 `build/`、`*.exe`、`*.o`、`*.d`、`*.s`、`*.i`，提交前用 `git status` 确认一遍。
- **换行符固定 LF**（见 `.gitattributes`）。Makefile 用 CRLF 会让 `make` 报 `missing separator`，所以仓库里统一 LF，VS Code 里看起来没区别。
- 提交身份建议用 GitHub 的 noreply 邮箱，这样 commit 能算到你账号名下，又不会把私人邮箱暴露在 public 仓库里：
  `git config --local user.name "<你的名字>"` + `git config --local user.email "<ID>+<用户名>@users.noreply.github.com"`。
- 提交信息用中文完全没问题，`git commit` 支持 UTF-8。

---

## 之后（先不要动手，等 C/C++ 打完基础再说）

**竞赛线**：洛谷「入门 1」「入门 2」题单 + Codeforces Div.4。本仓库 ex08–ex13 覆盖的正是这些题单的知识点；每做一题回来在 `notes/14` 的边界清单上打勾。

**嵌入式线（STM32）**：需要的东西体积不小，到时候我会先报体积和位置给你确认，**现在一个都不装**：

- STM32CubeIDE 约 3–5 GB，或
- 只装 `arm-none-eabi-gcc` 工具链 + VS Code 插件，约 1–2 GB，更轻
- 硬件：一块 STM32 开发板（F103C8T6 最小系统板最便宜最好上手）

从「PC 上的 C++」到「单片机上的 C++」差在哪，等你 C 那一套（姊妹仓库 c-learn）走完，我再给你写对应的说明。
