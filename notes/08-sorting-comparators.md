# 08 · 排序与自定义比较

> 对应练习：`ex07_structs_sorting.cpp`
> `sort` 是竞赛里使用频率最高的 STL 函数，没有之一。

---

## 1. 三种用法

```cpp
#include <algorithm>
#include <vector>
#include <functional>      // greater<>

int a[5] = {5, 2, 9, 1, 7};
vector<int> v = {5, 2, 9, 1, 7};

sort(a, a + 5);                     // 原生数组：传首尾指针
sort(v.begin(), v.end());           // vector：传迭代器（区间左闭右开）
sort(v.rbegin(), v.rend());         // 反向迭代器 = 直接得到降序
sort(v.begin(), v.end(), greater<int>());   // 也可以这样写降序
```

**区间是左闭右开**：`sort(a, a+5)` 排的是 `a[0]..a[4]`；只想排前 3 个就写 `sort(v.begin(), v.begin()+3)`。

---

## 2. 比较函数的语义：只回答一个问题

```cpp
sort(v.begin(), v.end(), [](int x, int y) {
    return x > y;         // 含义："x 应该排在 y 前面吗？"  → 降序
});
```

**铁律：只能写 `<` 或 `>`，绝不能写 `<=` 或 `>=`。**

因为 `sort` 要求比较函数构成**严格弱序**，其中一条是"同一个元素不能比自己更小"（`cmp(a,a)` 必须为 false）。写成 `<=` 时 `cmp(a,a)` 为 true，`sort` 内部会越界访问内存，**在小数据上"看起来正常"，在大数据上直接段错误 (RE)**。这是 OJ 上最经典的玄学 RE 原因之一。

```cpp
// 错误 —— 数据一大就崩
sort(v.begin(), v.end(), [](int a, int b) { return a <= b; });
```

---

## 3. 结构体多关键字排序

```cpp
struct Student { string name; int score; };

sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
    if (a.score != b.score) return a.score > b.score;   // 第一关键字：分数降序
    return a.name < b.name;                             // 第二关键字：名字字典升序
});
```

模式就是"**逐层 if，最后一个 return**"。三个关键字就写两个 `if`。写的时候心里默念："这一层只负责判断这一项是否分出胜负；平局才交给下一层。"

**参数一定要 `const T&`**。写成 `[](Student a, Student b)` 会拷贝两个含 `string` 的对象，n = 1e5 时排序要拷约 1.7e6 次，慢好几倍甚至直接超时。

---

## 4. 实测对照

`ex07` 里的这个例子（用 ASCII 名字是为了避免控制台编码干扰）：

```
输入：Xiaoming 88 / Xiaohong 95 / Xiaogang 88 / Xiaoli 95
规则：分数降序，同分按名字字典升序
实测输出：
Xiaohong 95
Xiaoli 95
Xiaogang 88
Xiaoming 88
```

`Xiaohong < Xiaoli`（第 4 个字符 `h` < `l`），`Xiaogang < Xiaoming`（`g` < `m`）。**字典序是逐字符比 ASCII 码**，`"Z" < "a"`（大写在小写前面），这点在"按名字排序"的题里会直接决定答案。

---

## 5. 排序后还要原来的顺序

排对象会把原顺序打乱。两个办法：

```cpp
// 办法一：备份一份
vector<int> origin = v;

// 办法二：排下标（推荐——不动原数据，只得到顺序）
vector<int> idx(n);
iota(idx.begin(), idx.end(), 0);                  // idx = 0,1,2,...  来自 <numeric>
sort(idx.begin(), idx.end(), [&](int i, int j) { return v[i] < v[j]; });
// 现在 idx[k] = 原数组中第 k 小的元素的下标
```

"排下标"是处理"多套排序规则"的标准技巧：同一份数据要按分数排一次、按学号再排一次，排下标就不用反复拷贝对象。

---

## 6. 其它排序相关工具

| 函数 | 作用 | 复杂度 |
|---|---|---|
| `stable_sort` | 稳定排序（相等元素保持原顺序） | O(n log n)，可能额外内存 |
| `partial_sort(b, mid, e)` | 只保证前 `mid-b` 个是有序的最小值 | O(n log k) |
| `nth_element(b, nth, e)` | 把第 n 小的放到 `nth`，左边都比它小 | **平均 O(n)** |
| `is_sorted(b, e)` | 检查是否已有序 | O(n) |
| `max_element` / `min_element` | 找最大/最小元素的**迭代器** | O(n) |
| `count` / `find` / `all_of` | 线性查找与判定 | O(n) |
| `reverse` / `rotate` | 翻转 / 循环移位 | O(n) |
| `unique` | 把相邻重复挤到后面，返回新逻辑尾 | O(n)，**必须先排序** |
| `binary_search` / `lower_bound` / `upper_bound` | 二分（要求已有序） | O(log n) |

两个高频细节：

```cpp
int mx = *max_element(v.begin(), v.end());        // 记得解引用！
int pos = lower_bound(v.begin(), v.end(), x) - v.begin();   // 得到下标
```

`stable_sort` vs `sort`：题目说"分数相同则按输入顺序输出"时，要么用 `stable_sort` + 只比分数，要么在比较函数里加"序号"作为最后的决胜关键字。**后者更稳**（不依赖算法稳定性，也能应付"反过来排"的变形）。

---

## 7. `unique` 去重的正确写法

```cpp
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());     // 这一句要背下来
```

`unique` **不删除**元素，它只是把不重复的值搬到前面，返回"逻辑末尾"的迭代器。必须配合 `erase` 才真的缩短容器。只想知道"有几个不同的"，可以不 erase：

```cpp
int kinds = (int)(unique(v.begin(), v.end()) - v.begin());
```

---

## 8. 排序类题目的三个陷阱

1. **比较函数写 `<=`** → 大数据 RE。见第 2 节。
2. **没考虑"全部分数相同"的边界** → 你的第二关键字没生效，输出顺序和期望不符。造数据测一下：3 个人同分。
3. **`sort` 在循环里** → n 次排序 = O(n² log n)，多半超时。想想能不能只排一次，或用堆（第 12 章）。

---

## 9. 自检四问

1. `sort(a, a + n, cmp)` 里 `cmp(x, x)` 应该返回什么？
2. 为什么比较函数的参数要写 `const Student&`？
3. 想拿到"排序后每个元素原来的下标"，怎么做？
4. `unique` 之后容器长度变了吗？
