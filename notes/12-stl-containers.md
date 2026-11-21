# 12 · 常用数据结构（STL 容器）

> 对应练习：`ex12_bfs_containers.cpp`
> 选型口诀：**后进先出 stack，先进先出 queue，两头都要 deque，有序去重 set，按键映射 map，只关心最大最小 priority_queue。**

---

## 1. 总览表

| 容器 | 底层 | 关键操作复杂度 | 空间 | 典型场景 |
|---|---|---|---|---|
| `vector` | 连续数组 | 随机访问 O(1)，尾插均摊 O(1)，中间插删 O(n) | O(n) | 默认选择，一切线性表 |
| `deque` | 分段连续 | 两端 O(1)，随机访问 O(1) | O(n) | 滑动窗口（单调队列） |
| `stack` | 适配 deque | push/pop/top O(1) | O(n) | 括号匹配、表达式、显式化递归 |
| `queue` | 适配 deque | push/pop/front O(1) | O(n) | BFS、按到达顺序处理 |
| `priority_queue` | 二叉堆 | push/pop O(log n)，top O(1) | O(n) | Top-K、Dijkstra、合并有序流 |
| `set` / `multiset` | 红黑树 | 增删查 O(log n)，**自动有序** | O(n) | 需要前驱/后继/lower_bound |
| `map` | 红黑树 | O(log n)，键有序 | O(n) | 稀疏键映射、词频、坐标→信息 |
| `unordered_set/map` | 哈希表 | 平均 O(1)，最坏 O(n) | O(n) | 只判存在/查值，不需要顺序 |
| `pair` / `tuple` | — | 访问 O(1) | — | 打包两个值，`pair` 自带字典序 |

---

## 2. stack —— 最后进来的最先处理

```cpp
#include <stack>
stack<char> st;
st.push('(');  char t = st.top();  st.pop();  bool e = st.empty();
```

括号匹配（`ex12` 实测：`"([]{})"` → `matched`）：

```cpp
for (char c : s) {
    if (c == '(' || c == '[' || c == '{') st.push(c);
    else {
        if (st.empty()) return false;               // 右括号多了
        char t = st.top(); st.pop();
        if ((c == ')' && t != '(') ||
            (c == ']' && t != '[') ||
            (c == '}' && t != '{')) return false;   // 类型不匹配
    }
}
return st.empty();                                  // 左括号剩了也不算匹配
```

**两个必须的空判**：`top()` 之前判 `empty()`，否则是未定义行为（不报错，返回垃圾）。另外"结束时栈必须为空"这一条常被漏掉——`"((("` 也是合法前缀但不是合法串。

其它用途：表达式求值（双栈）、撤销操作、把递归改写成迭代（防爆栈）。

---

## 3. queue —— 先来先服务

```cpp
#include <queue>
queue<int> q;
q.push(1); q.front(); q.back(); q.size(); q.pop();
```

`ex12` 实测：依次 push 1 2 3 → `front=1 back=3 size=3`，`pop()` 后 `front=2`。

注意 **`queue` 没有 `q[0]`**，只能碰队首队尾——这是它的语义，也是 BFS 正确性的来源。想随机访问请用 `vector` 手动维护头指针（`head` 下标），BFS 里这样写常常更快：

```cpp
vector<pair<int,int>> q;  q.push_back({0,0});
for (size_t head = 0; head < q.size(); head++) {   // 用下标当"出队"
    auto [r, c] = q[head];
    ... q.push_back({nr, nc});                     // 新节点直接追加
}
```

---

## 4. priority_queue —— 只关心"最值"

```cpp
#include <queue>
priority_queue<int> big;                                        // 默认：大顶堆
priority_queue<int, vector<int>, greater<int>> small;           // 小顶堆
priority_queue<pair<int,int>>                                   // 按 first 再 second 降序
big.push(5); big.top(); big.pop();                              // top 是 O(1)
```

**默认是大顶堆**，这个反直觉（很多人以为 priority_queue 是"小的优先"）。要小顶堆必须写全三个模板参数。

**Top-K 标准套路**（维护一个大小为 K 的小顶堆）：

```cpp
priority_queue<int, vector<int>, greater<int>> pq;
for (int x : data) {
    pq.push(x);
    if ((int)pq.size() > K) pq.pop();      // 踢掉当前最小的
}
pq.top();                                  // 第 K 大  ← ex12 实测：5
```

`ex12` 实测：`{7,2,9,1,5}` 取第 3 大 = **5**。

`multiset` 也能做同样的事，而且**支持删除任意元素**（`priority_queue` 不能删非堆顶），需要"动态删除最值"时改用 `multiset`：

```cpp
multiset<int> ms; ms.insert(x); auto it = ms.begin(); ms.erase(it);
```

---

## 5. set / map —— 自动有序 + 去重

```cpp
set<int> se = {5, 1, 3, 1, 9};              // 自动去重 + 升序
se.insert(4); se.count(3); se.erase(1);
*se.begin();                                // 最小  ← ex12 实测：1
*se.rbegin();                               // 最大
auto it = se.lower_bound(3);                // 第一个 >= 3  ← 实测：3
```

`ex12` 实测：`se.size() = 4`（重复的 1 被去掉）。

```cpp
map<string,int> cnt;                        // 键自动有序
for (string w : words) cnt[w]++;            // ★ operator[] 对不存在的键先插入 0 再返回引用
for (auto& [k, v] : cnt) cout << k << ":" << v << " ";
```

`ex12` 实测：`{"pear","apple","pear","fig"}` → `apple:1 fig:1 pear:2`（**按字母序**输出）。

**`operator[]` 的双刃剑**：`cnt["x"]++` 统计词频很爽，但如果你只是想**查询**，`if (cnt["x"] > 0)` 会把 `"x"` 以 0 塞进 map 里，污染容器（`size()` 变大）。只查询要用：

```cpp
if (cnt.count(key)) ...          // 存在吗（不插入）
auto it = cnt.find(key);         // 找（不插入），it == cnt.end() 表示没有
```

`unordered_map` 用法一样，平均 O(1)，但**不保证顺序**且 `operator[]` 同样会插入。要"插入时才建、读取时不建"就用 `find`。

---

## 6. deque 与单调队列（滑动窗口最值）

```cpp
deque<int> dq;                    // 存下标，不存值
for (int i = 0; i < n; i++) {
    while (!dq.empty() && a[dq.back()] <= a[i]) dq.pop_back();  // 尾部没竞争力的丢掉
    dq.push_back(i);
    if (dq.front() <= i - k) dq.pop_front();                    // 过期的从头部丢
    if (i >= k - 1) ans[i - k + 1] = a[dq.front()];             // 队首 = 窗口最大
}
```

`ex12` 实测：`a = {1,3,2,5,4}`，窗口 `k=3` → `3 5 5`。

为什么是 O(n)：每个下标最多进队一次、出队一次，总共 2n 次操作。这是 deque 最重要的用途，也是"用数据结构把 O(nk) 降成 O(n)"的经典例子。

---

## 7. 选型决策树

```
需要按顺序访问？
├─ 是，且长度固定/已知 → vector
├─ 是，且两头都要操作   → deque
└─ 需要"总是拿最大/最小"
    ├─ 只取，不删别的   → priority_queue
    ├─ 要能删任意元素   → multiset
    └─ 要排序 + 去重    → set
需要"键 → 值"的对应关系？
├─ 键是整数且值域 ≤ 1e7 → 数组直接开（最快，别用 map）
├─ 需要按键有序遍历     → map
└─ 只要查在不在/对应值  → unordered_map / unordered_set
```

---

## 8. 四个使用禁忌

1. **`top()` / `front()` / `back()` 之前必须判空**，空容器上是未定义行为。
2. **遍历时删元素**要用 `it = c.erase(it)` 的写法，直接 `for (auto x : c) c.erase(...)` 是错的（迭代器失效）。
3. **`set`/`map` 里不能改 key**：`(*it) = 5` 编译不过，要改得先 `erase` 再 `insert`。改 value 可以（`it->second = x`）。
4. **`unordered_map` 被卡**：它的哈希函数可能被构造冲突数据打成 O(n) 每次操作（部分 OJ 有这种反哈希题）。有被卡风险时换 `map` 或手写哈希。

---

## 9. 顺手记一下：Dijkstra 长什么样

`priority_queue` 最经典的用途，等你学完最短路再回来读：

```cpp
vector<long long> d(n, INF);
priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
d[src] = 0; pq.push({0, src});
while (!pq.empty()) {
    auto [du, u] = pq.top(); pq.pop();
    if (du > d[u]) continue;                     // 旧记录，跳过
    for (auto [v, w] : g[u]) if (d[v] > du + w) { d[v] = du + w; pq.push({d[v], v}); }
}
```

现在只需理解：**它就是"用堆优化的 BFS"——把"最近的一层先处理"换成"当前距离最小的先处理"。**
