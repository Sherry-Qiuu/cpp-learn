# 06 · 数组、字符串与 vector

> 对应练习：`ex05_arrays_vectors.cpp`、`ex06_strings.cpp`
> 三种存数据的方式，各有明确适用场景。

---

## 1. 原生数组

```cpp
int a[5] = {2, 4, 6, 8, 10};   // 定长，长度必须是编译期常量
int b[1005] = {0};             // 全部清零（只有前几个显式给出时，剩余补 0）
char s[20];                     // 裸字符数组
```

特性：

- **内存连续**，`a[i]` 就是 `*(a + i)`，访问是 O(1) 且缓存友好。
- **长度固定**，编译期定死，不能 `resize`。
- **不做越界检查**：`a[5]` 编译通过、运行不报错，读到的是隔壁内存的值。这是最危险的一点。
- **不能整体赋值/比较**：`a = b;` 非法，`a == b` 比的是地址不是内容。
- 传给函数会**退化成指针**，`sizeof(a)` 在函数里变成指针大小（8），这是经典陷阱。

开多大？竞赛常见 `const int MAXN = 100005;` 然后 `int a[MAXN];` 开全局。二维 `int dp[1005][1005]` ≈ 4MB，能开；`int dp[5000][5000]` = 100MB，多半 MLE。

---

## 2. vector：会自己变长的数组

```cpp
vector<int> v;                    // 空
vector<int> a(n, 0);              // n 个 0 —— 最常用
vector<int> b = {3, 1, 2};        // 列表初始化
v.push_back(x);                   // 尾部加       O(1) 均摊
v.pop_back();                     // 尾部删       O(1)
v.size();  v.empty();             // 长度 / 是否为空
v[i];      v.at(i);               // 访问；at() 越界会抛异常，适合调试
v.insert(v.begin() + i, x);       // 中间插入     O(n) ← 注意代价
v.erase(v.begin() + i);           // 中间删除     O(n)
v.resize(k);  v.resize(k, 0);     // 改长度，多出来的填 0/指定值
v.clear();                        // 清空
sort(v.begin(), v.end());         // 见第 08 章
```

**为什么优先用 `vector`**：长度可以是变量（`vector<int> a(n+1)`）、会自动释放内存（不会泄漏）、能整体赋值和比较、能安全地传给函数、`.size()` 自带。你写过的 `light_switch.cpp` 里 `vector<int> a(n + 1, 0)` 正是这个用法——**`n+1` 是为了让下标从 1 开始用而不用做减一换算**。

**代价**：`push_back` 扩容时会重新分配并拷贝全部元素（均摊仍是 O(1)）；中间插入删除是 O(n)。知道大小的时候用 `v.reserve(n)` 预分配，能省掉扩容。

**迭代器失效**：`push_back` 或 `erase` 之后，之前保存的指针/下标对应的迭代器全部作废。所以不要在遍历中保存迭代器再插入。

---

## 3. 遍历的四种写法

```cpp
for (int i = 0; i < (int)v.size(); i++)   // 要下标时（推荐，(int) 消警告）
for (int x : v)                           // 只读遍历，最简洁
for (int& x : v) x *= 2;                  // 要改元素，必须加 &
for (auto it = v.begin(); it != v.end(); ++it)   // 迭代器，需要 erase 时用
```

`for (int x : v) x = 0;` 是**无效代码**——改的是副本。这个 bug 编译器只给警告不给错，务必记住加 `&`。

---

## 4. 二维数组 / 二维 vector

```cpp
// 写法一：原生二维（全局，定长，性能最好）
int g[105][105];                  // 行列都固定，n,m ≤ 100

// 写法二：vector 套 vector（行列运行时才知道）
vector<vector<int>> a(n, vector<int>(m, 0));

// 写法三：一维模拟二维（连续内存，最快，稍麻烦）
vector<int> a(n * m, 0);
a[i * m + j] = 7;
```

常用操作：

```cpp
int rows = (int)a.size();            // 行数
int cols = (int)a[0].size();         // 列数（a 为空时这会崩！先判 empty）
for (auto& row : a)                  // 按行遍历
    for (int x : row) cout << x << ' ';

// 网格四方向：方向数组是标配，比写四段 if 干净
int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
for (int k = 0; k < 4; k++) {
    int nx = x + dx[k], ny = y + dy[k];
    if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;   // 先判越界再用
    ...
}
```

**下标顺序永远是 `a[行][列]`**，即 `a[y][x]`。很多人把 `a[x][y]` 写反，越界就崩在这里。建议：网格题里固定用 `g[i][j]` 表示"第 i 行第 j 列"，坐标变量名用 `(r, c)` 而不是 `(x, y)` 来避免歧义。

**`vector<vector<int>>` 的隐藏问题**：每行是独立分配的小块，内存不连续，行与行之间可能有几十倍的缓存命中率差异。`n, m` 上千且需要极限速度时改用写法三。另外 `vector<bool>` 是特化的位压缩版本，`v[i]` 返回的不是 `bool&`——**需要引用时写 `vector<char>` 代替**（这个坑会在 ex11 的 DFS 里出现）。

---

## 5. string：好用的字符串

```cpp
string s = "hello";
s.size();  s.length();      // 长度，两者相同
s[i];                       // 下标访问，可改 s[i] = 'H';
s += " world";              // 拼接（也支持 s = s + s2）
s.substr(pos, len);         // ★ 从 pos 起取 len 个字符，不是 [pos, pos+len)
s.find("cd");               // 子串位置，找不到返回 string::npos
s.find('x') != string::npos // 标准判断写法
s.compare(s2) / (s < s2);   // 字典序比较，直接用运算符就行
s.insert(pos, "abc");  s.erase(pos, len);
s.push_back(c);  s.pop_back();
stoi(s) / to_string(x);     // 字符串 ↔ 数字（<string>）
```

`substr` 参数含义是最高频错误：`s.substr(1, 3)` 在 `"abcdef"` 上得到 `"bcd"`（起点 1，长度 3），不是 `"cde"`。

**字符 ↔ 数字转换**：

```cpp
int d = s[i] - '0';          // 字符 '7' → 数字 7
char c = '0' + (d % 10);     // 数字 → 字符
int whole = stoi(s);         // "123" → 123（超 int 范围会抛异常，用 stoll）
```

**反转 / 回文**：

```cpp
#include <algorithm>
reverse(s.begin(), s.end());
bool isPal = true;
for (int i = 0; i < (int)s.size() / 2; i++)
    if (s[i] != s[s.size()-1-i]) { isPal = false; break; }
```

---

## 6. 三个必会的小算法

```cpp
// ① 翻转一段
reverse(a.begin(), a.begin() + k);           // 前 k 个翻转（迭代器右端是"开区间"）

// ② 循环左移 k 位：标准库直接有，就是 std::rotate
rotate(v.begin(), v.begin() + k, v.end());   // {1,2,3,4,5} k=2 -> {3,4,5,1,2}
// 不用 rotate 的话，三次翻转法等价：
// reverse(v.begin(), v.begin()+k); reverse(v.begin()+k, v.end()); reverse(v.begin(), v.end());

// ③ 去重（必须先排序）
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());    // 这是"擦除-识别"惯用法，必须记
int distinctCount = (int)v.size();
```

注意 STL 里几乎所有区间都是**左闭右开** `[begin, end)`，所以"前 k 个"写作 `[begin, begin+k)`。这个约定贯穿 `sort`/`reverse`/`unique`/`lower_bound`。**唯一的例外要记住**：`substr(pos, len)` 给的是「起点 + 个数」，不是 `[pos, pos+len)` 那对迭代器——因为 string 的下标参数天生是数字而不是迭代器。

---

## 7. 越界：C++ 初学者的头号 bug

```cpp
int a[5];
a[5] = 1;       // 编译通过，运行不报错，写坏了隔壁内存
for (int i = 0; i <= n; i++) sum += a[i];   // 多算一个，常见
```

防御手段：

1. **写循环时立刻在心里翻译一遍**：`i < n` → 最大下标 `n-1`，问自己"最后一个元素访问到了吗？多访问了吗？"
2. **调试时开 Sanitizer**：`make san/ex05`，它会精确报出 `READ of size 4 at ... ex05.cpp:LINE`。
3. 下标可能为负时（`i - 1`、`pos - k`）**先判再取**。

---

## 8. 统计类题型的三板斧

```cpp
// ① 计数数组（值域小时最好用，比 map 快）
int cnt[26] = {0};
for (char c : s) if (islower(c)) cnt[c - 'a']++;      // 字母 → 下标 0~25

// ② 标记数组（判断"出现过没有"）
vector<char> seen(n + 1, 0);

// ③ 频率最高 → 边统计边记录最大值，别再扫一遍
int best = 0;
for (int i = 0; i < 26; i++) best = max(best, cnt[i]);
```

值域 ≤ 1e6 用计数数组；值域大或稀疏用 `map`/`unordered_map`（见第 12 章）。

---

## 9. 自检五问

1. `for (int x : v) x = 0;` 能把 `v` 清零吗？
2. `vector<bool>` 为什么常被建议换成 `vector<char>`？
3. `s.substr(2, 5)` 取的是哪几个字符？
4. `a[0].size()` 什么时候会崩？
5. 删除 `vector` 中间一个元素，代价多大？有没有更快的容器？
