# 05 · 函数与递归

> 对应练习：`ex03_functions.cpp`、`ex04_recursion.cpp`
> 函数是"拆问题"的工具，递归是"自己调用自己"的函数。两者一起构成后面 DFS 和 DP 的基础。

---

## 1. 定义、声明与调用

```cpp
#include <iostream>
using namespace std;

int add(int a, int b);          // 声明（原型）：告诉编译器"有这么个函数"

int main() {
    cout << add(2, 3) << "\n";  // 调用
}

int add(int a, int b) {         // 定义：真正的实现
    return a + b;
}
```

规则：调用发生之前，函数必须**已经被编译器看见**（要么定义在前，要么声明在前）。所以：

- **单文件小程序**：把定义写在 `main` 前面，最省事。
- **多文件工程**：声明放 `.h`，定义放 `.cpp`，`main.cpp` 只 `#include` 头文件（见 `projects/score_report`）。

`void` 函数可以不写 `return;`（写在中间用于提前退出很有用）；有返回值的函数**每条路径都必须 return**，漏了就是未定义行为——`-Wall` 会用 `control reaches end of non-void function` 提醒你。

---

## 2. 三种参数传递

```cpp
void byValue(int x)   { x = 100; }   // 副本：改不到外面
void byRef(int& x)    { x = 100; }   // 引用：改的就是外面那个变量
void byConstRef(const string& s) { cout << s.size(); }  // 只读大对象：省拷贝且防误改
```

| 写法 | 是否拷贝 | 能否改外部 | 用在 |
|---|---|---|---|
| `T x` | 拷贝一次 | 不能 | `int`/`char`/`double`/`bool` 等小类型 |
| `T& x` | 不拷贝 | **能** | 需要输出参数、交换、累加器 |
| `const T& x` | 不拷贝 | 不能（编译器拦着） | `string`/`vector`/`struct` 只读传参 |

选择逻辑就三句话：要改它 → `T&`；不改但对象大 → `const T&`；其它 → 直接传值。

**数组/`vector` 作参数**：会退化，长度信息丢失，必须**额外传长度**：

```cpp
int maxOf(const vector<int>& v) { ... }        // vector 自带 size()，不用传长度
int maxOf(const int* a, int n) { ... }         // 裸数组必须传 n
```

---

## 3. 返回值与"返回多个值"

```cpp
// 办法一：引用参数带出去（竞赛最快）
bool findFirst(const vector<int>& v, int target, int& pos) {
    for (int i = 0; i < (int)v.size(); i++)
        if (v[i] == target) { pos = i; return true; }
    return false;
}

// 办法二：pair / tuple（语义清楚）
pair<int,int> minMax(const vector<int>& v) { ... }
auto [lo, hi] = minMax(v);            // C++17 结构化绑定

// 办法三：返回 struct（字段多时最好读）
struct Result { bool ok; int steps; vector<int> path; };
```

---

## 4. 重载与默认参数

```cpp
int    abs_(int x)    { return x < 0 ? -x : x; }
double abs_(double x) { return x < 0 ? -x : x; }   // 同名不同参数 = 重载
int power(int base, int exp = 2) { ... }           // 默认参数写在"声明"里
```

默认参数只能从右往左连续给（`f(int a, int b = 1, int c)` 非法）。**默认值要写在头文件的声明里**，写在 `.cpp` 里调用方看不见。

---

## 5. 作用域与生命周期

```cpp
int g = 0;                 // 全局：程序整个生命周期，自动初始化为 0
void f() {
    int a = 1;             // 局部：进函数创建，出函数销毁
    static int cnt = 0;    // static 局部：只初始化一次，跨调用保留值
    cnt++;
}
```

**全局数组**适合 `int dp[100005];` 这种大对象（在静态区，不受 1MB 栈限制）；**函数内的局部数组超过几 MB 会直接爆栈**。这是"为什么大数组要开全局"的真正原因。

`static` 局部变量做"调用计数器"很方便，但多线程下不安全，竞赛代码无妨。

---

## 6. 递归：把栈交给你

递归 = 函数调用自己，每次调用在栈上开一个新帧。三要素：

```
① 边界（base case）：最小规模的问题直接给答案，不再递归
② 缩小：每次调用必须让问题规模严格变小
③ 组合：用子问题的返回值拼出当前的答案
```

```cpp
long long fact(int n) {                      // 阶乘
    if (n <= 1) return 1;                    // ① 边界
    return (long long)n * fact(n - 1);       // ② 缩小 + ③ 组合
}
```

`fact(4)` 的执行过程：

```
压栈：fact(4) → fact(3) → fact(2) → fact(1)   ← 命中边界
弹栈：   24   ←  6    ←  2    ←  1
```

**递归深度是有上限的**。本机（Windows，默认 1MB 栈）实测：帧很小时约 **4.3 万层**爆栈；函数里放一个 `int buf[256]`（约 1KB）后 **1000 层左右**就崩。Linux/在线 IDE 通常给 8MB，能到几十万层。所以：**深度可能超过 1e4 的递归，要把大数组挪到全局，或改成迭代 + 手写 `stack`。**

---

## 7. 递归的两个经典用途

**① 分治**：把问题切成两半，各自治好再合并。

```cpp
int sumRange(const vector<int>& a, int l, int r) {   // 闭区间 [l, r]
    if (l > r) return 0;                            // 空区间是边界，不是特判麻烦
    if (l == r) return a[l];
    int mid = l + (r - l) / 2;                      // 不用 (l+r)/2：防溢出
    return sumRange(a, l, mid) + sumRange(a, mid + 1, r);
}
```

**② 枚举型递归（回溯）**：见 `11-search-dfs-bfs.md`，模板是"做选择 → 递归 → 撤销选择"。

---

## 8. 递归的代价与记忆化

```cpp
int fib(int n) { return n < 2 ? n : fib(n-1) + fib(n-2); }   // 朴素
```

`fib(24)` 实测要 **46368 次调用**（约 2·fib(n) 次），`fib(45)` 就要 18 亿次——因为 `fib(3)` 这种子问题被重复算了成千上万次。加个数组把算过的存下来：

```cpp
long long memo[50];
long long fibMemo(int n) {
    if (n < 2) return n;
    long long& r = memo[n];              // 引用做别名：改 r 就是改 memo[n]
    if (r != -1) return r;               // 算过就交卷
    return r = fibMemo(n-1) + fibMemo(n-2);
}
```

`fibMemo(45)` 只需 45 次计算。**"递归 + 数组存结果" 就是记忆化搜索，它就是动态规划的一种写法**（自顶向下）。理解这一步，第 13 章的 DP 就不是新东西了。

注意 `memo` 必须初始化成一个"不可能的答案"（如 -1），因为全局数组默认是 0，而 0 常常是合法结果。

---

## 9. 递归 vs 迭代的转换

任何递归都能改写成迭代 + 显式栈，代价是代码变长。改写动机有两个：**爆栈**、**想要更细的控制**。

```cpp
// 把"中序遍历式"的线性递归改成迭代
long long factIter(int n) { long long r = 1; for (int i = 2; i <= n; i++) r *= i; return r; }
```

判断标准：**递归参数是单调变化的（阶乘、累加、斐波那契）→ 直接改循环**；递归是树形/图形的（搜索、分治、DFS）→ 保留递归更自然，除非深度超限。

---

## 10. 五个高频错误

1. **忘记 `return`**：`int f(...)` 的某条分支没返回，调用方拿到垃圾值。
2. **边界不缩小**：`if (n == 0) return 1; return f(n);` → 无限递归崩栈。
3. **递归改不动外部变量**：想把结果带出来，参数却用了值传递（少写 `&`）。
4. **`int` 溢出**：阶乘、`pow`、路径计数，第 13 项就爆 `int`，一律 `long long` 或边算边取模。
5. **数组越界**：递归里 `a[n]`、`a[-1]` 不报错但读到脏数据。写递归时先想清楚"最小时的 `n` 是多少"。

---

## 11. 自检四问

1. `void f(int x)` 里改 `x`，外面为什么不变？改成什么能变？
2. 为什么大数组要开全局而不是函数内？
3. 朴素 `fib(n)` 的调用次数为什么是指数级的？记忆化把它变成什么复杂度？
4. `(l + r) / 2` 什么时候错？怎么改？
