# 04 · 条件判断与循环

> 对应练习：`ex02_control_flow.cpp`
> 这一章是"模拟"类题目的全部语法基础。

---

## 1. if / else if / else

```cpp
if (score >= 90)      grade = 'A';
else if (score >= 80) grade = 'B';      // 注意：写 >= 80 就够了，因为 >=90 已经排掉了
else if (score >= 60) grade = 'C';
else                  grade = 'D';
```

三个要点：

**① 顺序即逻辑。** 区间判断要排序：把 `>=90` 放最前，后面的条件自然只覆盖剩下的情况。**条件写反顺序**（先判 `>=60`）会导致高分永远进不了 A 分支——这是真实高频错误。

**② `else` 离最近的未配对 `if`。**

```cpp
if (a)
    if (b) cout << 1;
    else   cout << 2;      // 这个 else 配的是 b，不是 a！
```
加上花括号就没歧义。**永远写花括号**，哪怕只有一条语句。

**③ 浮点数不要用 `==` 判等。**

```cpp
if (x == 0.1)          // 错：0.1 在二进制里存不精确
if (fabs(x - 0.1) < 1e-9)   // 对：判"足够接近"
```

---

## 2. 三元运算符

```cpp
int mx = a > b ? a : b;
cout << (ok ? "Yes" : "No") << "\n";   // 一定要加括号
```

只能表达"二选一"，别嵌套超过两层，可读性会崩。

---

## 3. switch

```cpp
switch (month) {
    case 1: case 2: case 12: season = '冬'; break;   // 多个 case 共用一段
    case 3: case 4: case 5:  season = '春'; break;
    default: season = '?';
}
```

**`break` 不能漏**——漏了会"贯穿"（fall through）执行下一个 case 的语句。`case` 后必须是**整型常量表达式**，不能写 `case x > 3:`，也不能用字符串。范围判断请回到 `if`。

---

## 4. for / while / do-while

```cpp
for (int i = 0; i < n; i++) { }   // 三段：初始化; 条件; 步进
for (int i = 1; i <= n; i++) { }  // 下标从 1 开始，配合 a[n+1] 的数组
while (x > 0) { x /= 10; cnt++; } // 不知道跑几次，跑到条件不成立
do { ... } while (条件);           // 至少执行一次（很少用）
```

选型：**知道次数用 `for`，看条件停用 `while`**。

`for` 的三段里，初始化只执行一次；条件在**每轮开始前**检查；步进在**每轮结束后**执行。所以 `for (int i = 0; i < n; i++)` 当 `i == n` 时循环体不执行。

**死循环的三种成因**：

```cpp
int i = 0;
while (i < n) { cout << i; }        // ① 忘了 i++
for (int i = n; i >= 0; i--) { }    // ② i 是无符号类型，减到 0 再减变巨大 → 永不 <0
while (x != 1) { x = f(x); }        // ③ 永远到不了 1（浮点数尤其危险）
```

第 ② 条是 `unsigned` / `size_t` 的经典陷阱：**倒序循环不要用无符号类型**。

---

## 5. break 与 continue

```cpp
for (int i = 1; i <= 6; i++) {
    if (i == 3) continue;     // 跳过本轮剩余语句，回到条件判断
    if (i == 5) break;        // 整个循环立即结束
    cout << i << " ";
}
// 输出：1 2 4
```

| | `break` | `continue` |
|---|---|---|
| 作用范围 | 当前这一层循环（或 switch） | 当前这一层循环 |
| 效果 | 循环**终止**，跳到循环后面第一条语句 | 本轮**结束**，进入下一轮条件判断 |
| 危险点 | 嵌套循环里只跳一层 | `while` 里 `continue` 会跳过 `i++` → 死循环 |

**`while` 里用 continue 要特别小心**：

```cpp
int i = 0;
while (i < 5) {
    i++;
    if (i == 3) continue;   // 因为 i++ 在前面，安全
    cout << i;
}
```
如果把 `i++` 放在 `continue` 之后，程序就卡在 `i == 3` 无限循环。

**嵌套循环想一次跳出两层**：三个办法，按推荐度排序：① 把内层循环抽成函数，用 `return`；② 设一个 `bool done` 标记，外层判断后 `break`；③ `goto`（C++ 允许，竞赛代码里跳出自定义的双层循环是可接受的实用写法，但教学阶段不建议养成习惯）。

---

## 6. 常见循环模式

```cpp
// ① 求和 / 计数 / 最值（三大件）
long long sum = 0; int cnt = 0, mx = INT_MIN;      // 累加器必须初始化为 0；mx 取最小值起步
for (int i = 0; i < n; i++) { sum += a[i]; if (a[i] > 0) cnt++; mx = max(mx, a[i]); }

// ② 遍历直到某个条件（找到就停）
int pos = -1;                                       // -1 表示"没找到"，比 0 安全
for (int i = 0; i < n; i++) if (a[i] == target) { pos = i; break; }

// ③ 双层枚举所有对
for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++)                 // j 从 i+1 起：不重复、不含自己
        if (a[i] + a[j] == target) { ... }

// ④ 逐位处理整数
while (x > 0) { int d = x % 10; /* 用 d */ x /= 10; }

// ⑤ 区间翻转 / 打标记（就是 light_switch 那题）
for (int i = l; i <= r; i++) a[i] ^= 1;             // 0/1 翻转用异或，比 if 简洁
```

**`j = i + 1` 还是 `j = 0`**：枚举"无序对"用前者（n(n-1)/2 次），枚举"有序对/所有组合"用后者（n² 次）。这个细节直接决定复杂度是 5e6 还是 1e7。

---

## 7. 范围 for（C++11）

```cpp
for (int x : v) cout << x;          // 只读遍历，最简洁
for (int& x : v) x *= 2;            // 要改元素，必须加 &，否则改的是副本
for (const auto& s : names) ...;    // 大对象只读遍历的标配
```

范围 for **拿不到下标**，需要下标时（比如"偶数位置""前一个元素"）老老实实写 `for (int i...)`。

---

## 8. 循环变量类型的一个坑

```cpp
vector<int> v = ...;
for (int i = 0; i < v.size(); i++)        // 警告：有符号/无符号比较
for (size_t i = 0; i < v.size(); i++)     // 正确但啰嗦
for (int i = 0; i < (int)v.size(); i++)   // 竞赛里最常用的做法：强转一次
```

`v.size()` 返回 `size_t`（无符号），和 `int` 比较会触发 `-Wsign-compare` 警告。`n ≤ 2e9` 时 `(int)v.size()` 完全安全。本仓库的练习里统一用 `(int)` 强转，这样能保持 `-Wall -Wextra` 零警告——**零警告是验收标准之一**。

---

## 9. 自检四问

1. `for (unsigned i = n - 1; ; i--) { if (i == 0) break; }` 当 `n = 0` 时会怎样？
2. 为什么 `if (a) if (b) x(); else y();` 里的 `else` 不一定配 `a`？
3. `continue` 在 `while` 里为什么比在 `for` 里危险？
4. 求最大值时 `int mx = 0;` 什么时候会错？

去 `ex02_control_flow.cpp` 用断言验证你的答案。
