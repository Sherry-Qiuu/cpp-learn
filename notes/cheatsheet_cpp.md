# C++ 入门速查表

> 对应练习：`ex01`–`ex14` 全覆盖——这一张纸是索引，每一节说的就是你该去哪个练习里把它练熟。

打印出来贴桌上一张纸。所有代码在 `g++ -std=c++17 -Wall -Wextra -Wpedantic` 下验证过。

---

## 1. 编译与运行

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 t.cpp -o build/t.exe && ./build/t.exe
make run/ex07        # 本仓库：编译并运行练习
make san/ex07        # 带 ASan/UBSan，抓越界与溢出
make asm/ex07        # 看汇编
make all-ex / run-all / clean
```

| 选项 | 含义 |
|---|---|
| `-std=c++17` | 语言标准 |
| `-Wall -Wextra -Wpedantic` | 警告全开（**本仓库要求零警告**） |
| `-g` / `-O0` | 调试信息 / 不优化（单步才准） |
| `-O2` | 交题用的优化 |
| `-fsanitize=address,undefined` | 运行时抓越界、有符号溢出、空指针 |
| `-include bits/stdc++.h` | 或直接 `#include <bits/stdc++.h>` |

---

## 2. 类型

| 类型 | 字节 | 范围/精度 |
|---|---|---|
| `int` | 4 | ±2.1e9 |
| `long long` | 8 | ±9.2e18 |
| `double` | 8 | ≈15~16 位有效数字 |
| `char` | 1 | -128~127 |
| `bool` | 1 | true / false |

转换：`static_cast<double>(n) / m`（除法前先转！）、`(int)3.9 == 3`（截断）、`c - '0'`（字符→数字）。

---

## 3. I/O

```cpp
ios::sync_with_stdio(false); cin.tie(nullptr);     // 大数据必备，之后别混用 printf
cin >> a >> b;                                      // 空白（含换行）等价，自动跳
getline(cin, s);                                    // 读整行；前面有 >> 要先 cin.ignore()
cout << fixed << setprecision(2) << v;              // 2 位小数（<iomanip>）
cout << setw(8) << setfill('0') << x;               // 宽度 8（setw 只作用一次）
cerr << "x=" << x << "\n";                          // 调试输出，不污染答案
```

循环输出带空格：`if (i) cout << ' '; cout << a[i];`

---

## 4. 控制流

```cpp
for (int i = 0; i < n; i++)             while (cond) { ... i++; }
break;      // 终止当前这层循环
continue;   // 结束本轮，进入下一轮条件判断
```

`for (unsigned i = n-1; ; i--)` 是死循环（无符号不会变负）。

---

## 5. 函数与递归

```cpp
int  f(const vector<int>& v, int& out);      // 只读 + 输出参数
void dfs(...) { if (边界) return; for (选择) { 做; dfs(); 撤销; } }
```

递归三要素：边界 / 缩小 / 组合。`(l + r) / 2` 改 `l + (r - l) / 2` 防溢出。
**递归深度上限（本机实测）**：小帧 ≈ 4.3 万层；帧内含 1KB 局部数组 ≈ 1000 层。大数组开全局。

---

## 6. 数组 / vector / string

```cpp
vector<int> a(n, 0);                         // n 个 0
vector<vector<int>> g(n, vector<int>(m, 0)); // 二维
a.size()  a.empty()  a.push_back(x)  a.pop_back()  a.resize(k)
g.size()  /* 行 */   g[0].size()  /* 列，g 空时会崩 */

string s = "abc";
s.size()  s.substr(pos, len)  s.find(x) != string::npos  s += "d"  s[i] = 'X'
stoi(s)  to_string(x)
```

去重：`sort(v.begin(), v.end()); v.erase(unique(v.begin(), v.end()), v.end());`
范围 for 要改元素**必须加 `&`**：`for (int& x : v)`。

---

## 7. struct

```cpp
struct Node { int id; string name; int score = 0; };   // 结尾分号！字段给默认值
Node a{1, "x", 95};
vector<Node> v(n);
for (auto& x : v) cin >> x.name >> x.score;             // cin 进范围 for 必须加 &
p->score   // 指针访问；引用/对象用 .
```

---

## 8. 排序与二分

```cpp
sort(v.begin(), v.end());
sort(v.begin(), v.end(), [](const T& a, const T& b) {      // 只能写 < 或 >，禁止 <=
    if (a.k1 != b.k1) return a.k1 > b.k1;                   // 第一关键字
    return a.k2 < b.k2;                                     // 第二关键字
});
stable_sort / partial_sort / nth_element(O(n))
lower_bound(b, e, x)   // 第一个 >= x
upper_bound(b, e, x)   // 第一个 >  x；两者相减 = x 的个数
binary_search(b, e, x) // 是否存在
```

二分模板：

| 目标 | mid | 满足 | 不满足 |
|---|---|---|---|
| 第一个满足 | `(lo+hi)/2` | `hi = mid` | `lo = mid+1` |
| 最后一个满足 | `(lo+hi+1)/2` | `lo = mid` | `hi = mid-1` |

---

## 9. 常见套路代码

```cpp
// 前缀和
p[i] = p[i-1] + a[i];            查 [l,r]：p[r] - p[l-1]
// 差分
d[l] += v; d[r+1] -= v;          还原：d[i] += d[i-1]
// 二维前缀和
s[i][j] = s[i-1][j] + s[i][j-1] - s[i-1][j-1] + a[i-1][j-1];
sum(r1,c1,r2,c2) = s[r2][c2] - s[r1-1][c2] - s[r2][c1-1] + s[r1-1][c1-1];
// 逐位取数字
while (x) { int d = x % 10; ...; x /= 10; }
// 0/1 翻转
a[i] ^= 1;
// gcd / 幂
__gcd(a, b);                     // <numeric> 里有 std::gcd
long long qpow(long long a, long long e, long long m);
```

---

## 10. 容器选型

| 要什么 | 用什么 |
|---|---|
| 线性存、随机访问 | `vector` |
| 两头进出、单调队列 | `deque` |
| 后进先出 | `stack` |
| 先进先出、BFS | `queue` |
| 只取最大/最小 | `priority_queue`（默认大顶，`greater<>` 是小顶） |
| 有序 + 去重 + 前驱后继 | `set` / `multiset` |
| 键值映射且要有序 | `map` |
| 只要查在不在（快） | `unordered_set` / `unordered_map` |
| 值域小的整数计数 | 直接开数组 `int cnt[MAXV]`（比 map 快） |

⚠ 空容器上 `top()/front()/back()` 是未定义行为，先判 `empty()`。
⚠ `map/operator[]` 和 `unordered_map::operator[]` 会**插入**默认值，纯查询用 `find`/`count`。

---

## 11. DFS / BFS

```cpp
// DFS 回溯
if (完成) { 记录答案; return; }
for (每个选择) { if (不合法) continue; 做选择; dfs(); 撤销; }

// BFS 最短路（边权为 1）
dist[start] = 0; q.push(start);
while (!q.empty()) { auto u = q.front(); q.pop();
    for (邻居 v) if (合法 && dist[v] == -1) { dist[v] = dist[u]+1; q.push(v); } }  // 入队时标记！
```

剪枝三层：可行性（越界/重复/超限）、最优性（`depth >= best` 就 return）、记忆化（同状态不重算）。
求**最少步数** → BFS；求**所有方案** → DFS。

---

## 12. DP 四步

```
① 定义（"前 i 个" 还是 "以 i 结尾"）  ② 转移（最后一步从哪来）
③ 初始化                            ④ 答案位置（dp[n] 还是 max(dp)）
```

| 模型 | 转移 | 答案 |
|---|---|---|
| 爬楼梯 | `dp[i]=dp[i-1]+dp[i-2]` | `dp[n]` |
| 网格路径 | `f[i][j]=f[i-1][j]+f[i][j-1]` | `f[n-1][m-1]` |
| 打家劫舍 | `dp[i]=max(dp[i-1], dp[i-2]+a[i])` | `dp[n-1]` |
| 最大子段和 | `dp[i]=max(a[i], dp[i-1]+a[i])` | `max(dp[i])` |
| LIS | `dp[i]=max(dp[j]+1), a[j]<a[i]` | `max(dp[i])` |
| 01 背包 | `dp[c]=max(dp[c], dp[c-w]+v)`，**容量逆序** | `dp[C]` |
| 完全背包 | 同上，**容量正序** | `dp[C]` |
| 背包方案数 | `dp[c] += dp[c-w]` | `dp[C]` |

---

## 13. 复杂度速查

| 复杂度 | n 上限 | | 容器操作 |
|---|---|---|---|
| O(n) | 1e7~1e8 | | `vector` 随机访问 O(1) |
| O(n log n) | 1e6~2e6 | | `vector` 中间插删 O(n) |
| O(n²) | 3000~5000 | | `set/map` 增删查 O(log n) |
| O(n³) | ≤500 | | `unordered_*` 平均 O(1) |
| O(2ⁿ) | ≤20 | | `sort` O(n log n) |
| O(n!) | ≤11 | | 堆 push/pop O(log n) |

内存：256 MB ≈ 6.7e7 个 `int`。本机 1 秒 ≈ 1e8~3e8 次简单操作（`-O2`）。

---

## 14. 报错速查

| 报错 | 原因 |
|---|---|
| `'cout' was not declared` | 缺 `#include <iostream>` 或 `using namespace std;` |
| `expected ';' before ...` | **上一行**漏分号 |
| `expected '}' at end of input` | 花括号没配对 |
| `undefined reference to 'main'` | `main` 拼错 |
| `undefined reference to 'f(int)'` | 声明了没实现 / 少了 .cpp |
| `‘struct X’ defined but not used` 后一堆怪错 | `struct` 结尾漏分号 |
| `no matching function for call to ...` | 实参类型/个数不匹配 |
| 段错误 (Segmentation fault) | 越界、空指针、递归爆栈、`cmp` 写了 `<=` |
| 死循环 | 无符号倒序、`continue` 跳过自增、二分模板混用 |
| 本地对提交错 | 多组数据没重置 / 溢出 / 输出格式 |

---

## 15. ASCII 备忘

`'0'`=48 `'A'`=65 `'a'`=97 空格=32 换行=10。大小写差 32。`c-'A'` → 0~25。
