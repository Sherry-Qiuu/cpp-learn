# 07 · 结构体 struct

> 对应练习：`ex07_structs_sorting.cpp`、项目 `projects/score_report`
> 一句话：**把一组相关的、类型不同的数据打包成一个新类型。**

---

## 1. 定义与使用

```cpp
#include <iostream>
#include <string>
using namespace std;

struct Student {          // ← 结尾的分号！漏了会报一堆莫名其妙的错
    string name;
    int score;
    int id;
};

int main() {
    Student a{"邱邱", 95, 1};    // 聚合初始化，按字段顺序，C++11 起最常用
    Student b;                    // 字段是垃圾值（局部变量）—— 记得赋初值
    b.name = "小林"; b.score = 88; b.id = 2;

    cout << a.name << " " << a.score << "\n";   // 用 . 访问
}
```

给字段**默认值**是防止"忘初始化"的好办法：

```cpp
struct Student {
    string name = "";
    int score = 0;
    int id = 0;
};
Student v[100];       // 现在每个元素自动是 {"", 0, 0}
```

---

## 2. 结构体数组 = 一张表

```cpp
vector<Student> cls = {{"小明",88,1},{"小红",95,2},{"小刚",72,3}};

for (const auto& s : cls)              // 只读遍历用 const auto&
    cout << s.name << ": " << s.score << "\n";

long long total = 0;
for (const auto& s : cls) total += s.score;

// 找第一个及格的
int idx = -1;
for (int i = 0; i < (int)cls.size(); i++) if (cls[i].score >= 60) { idx = i; break; }
```

结构体数组在内存里是**一个接一个排着**的，`cls[2]` 就是基址 + 2×sizeof(Student)。所以：

- **按行遍历（外层对象、内层字段）快**，`sort` 交换结构体会搬动整个对象。
- 字段多、对象大又想频繁排序时，**排序下标**而不是排对象（见第 08 章）。

---

## 3. 传参与返回

```cpp
void printAll(const vector<Student>& v)      { ... }   // 只读：const&
void bonus(vector<Student>& v, int add)      { ... }   // 要改：&
Student bestOf(const vector<Student>& v)     { ... }   // 返回整个对象，OK
int  maxScore(const Student& s)              { return s.score; }   // 只读字段
```

规则和第 05 章一样：小结构体（≤ 2 个 int）可以直接传值；带 `string`/`vector` 字段的结构体**一律 `const T&`**，否则每次调用都要拷贝堆内存，`sort` 里拷贝 n log n 次会明显变慢。

---

## 4. 结构体里放函数（够用就行的"成员函数"）

```cpp
struct Point {
    int x, y;
    long long dist2(const Point& o) const {      // const 表示不改自己
        return 1LL * (x-o.x)*(x-o.x) + 1LL * (y-o.y)*(y-o.y);
    }
    bool operator<(const Point& o) const {       // 重载 < ，让 sort 直接能用
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
};
```

`operator<` 一旦定义，`sort(v.begin(), v.end())`、`set<Point>`、`lower_bound` 都能直接用，省掉每次写 lambda。入门阶段掌握这一条就够，类的其它特性（构造、继承、虚函数）暂时不碰。

---

## 5. 嵌套与结构体装容器

```cpp
struct Answer { int sid; int score; };
struct Problem { vector<Answer> scores; };        // 结构体里装 vector
vector<Student> cls;                              // 结构体装进 vector
Student* p = &cls[0];
cout << p->name;                                  // 指针访问用 ->，等价 (*p).name
```

`->` 和 `.` 的区别只有一条：左边是指针就用 `->`，是对象就用 `.`。入门阶段尽量少用裸指针，用引用 `Student& r = cls[0];` 然后照常用 `.`。

---

## 6. 什么时候用 struct，什么时候用多个数组

| 场景 | 推荐 |
|---|---|
| 一张表，每行多个属性，要**整体排序** | `struct` + `vector<struct>` |
| 只关心一个数值序列，不绑定身份 | 单独一个数组 |
| 两个量天然成对（区间 `[l,r]`、坐标 `(x,y)`） | `pair<int,int>`（自带字典序比较，能直接 sort） |
| 需要按某个字段自动去重/有序 | `struct` + `operator<` + `set` |

`pair` 值得记一下：`vector<pair<int,int>> seg; sort(seg.begin(), seg.end());` 会**先按 `.first` 升序，相等再按 `.second` 升序**，很多题靠这一句省掉自定义比较器。访问用 `p.first` / `p.second`，或者 `auto [l, r] = p;`。

---

## 7. 三个坑

**① 结尾分号**：`struct Point { int x, y; }` 漏分号，报错信息会指向**下一个声明**，看起来毫不相关。这是新手最容易卡住半小时的错误。

**② 比较比的是内容还是地址**：`if (a == b)` 对结构体默认**编译不过**（除非所有字段都可比较且你用了 C++20 的 `==` 推导）。C++17 下要自己写 `operator==` 或逐字段比。

**③ 浅拷贝**：结构体里存了指针/数组时，`Student c = a;` 只拷指针不拷内容。用 `string`/`vector` 字段就不会遇到这个问题——这也是**入门阶段不要用裸 `char name[20]` 而用 `string`** 的理由。

---

## 8. 实战片段：读一张表并处理

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
struct Student { string name; int score; };
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<Student> v(n);
    for (auto& s : v) cin >> s.name >> s.score;      // 注意 & ：直接写进数组元素
    sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.name < b.name;
    });
    for (int i = 0; i < (int)v.size(); i++)
        cout << i + 1 << ". " << v[i].name << " " << v[i].score << "\n";
    return 0;
}
```

`for (auto& s : v) cin >> s.name >> s.score;` 里的 `&` 是关键——漏了就是往副本里读，数组全是空值。**这个 bug 在结构体读入时出现频率极高**，记住：`cin` 进范围 for 变量必须加 `&`。

去 `projects/score_report` 做一次完整的多文件版本（头文件、分离编译、Makefile）。
