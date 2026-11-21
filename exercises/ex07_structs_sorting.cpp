/* ============================================================================
 * ex07_structs_sorting.cpp —— struct · 比较函数 · lambda · sort / stable_sort
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 用 struct 把「一行表」打包成一个类型，传参一律 const T&
 *   2. 多关键字排序的「逐层 if，最后一个 return」写到不用想
 *   3. 明白比较函数只回答一句话——「a 该排在 b 前面吗」，以及为什么只能写 <
 *   4. sort / stable_sort / unique 这三个高频函数配合 vector 把统计题收尾
 *
 * 编译运行：
 *   make run/ex07
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex07_structs_sorting.cpp \
 *          -o build/ex07.exe && ./build/ex07.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <numeric>
#include <algorithm>
#include <cassert>
using namespace std;

/* 一张学生表：三科成绩 + 学号。字段给默认值，防止「忘了初始化是垃圾值」。 */
struct Student {
    string name = "";
    int chinese = 0;
    int math = 0;
    int english = 0;
    int id = 0;
};

/* 网格/平面题里最常见的二元组。这里故意不用 pair，好练成员访问。 */
struct Point {
    int x = 0;
    int y = 0;
};

/* ---------------------------------------------------------------------------
 * TODO 1：算一个学生的总分
 *
 *   期望：
 *     totalOf({{"Anna", 90, 80, 70, 1}})  -> 240
 *     totalOf({{"", 0, 0, 0, 0}})         -> 0
 *     totalOf({{"Zed", 100, 100, 100, 9}}) -> 300
 *
 *   提示：签名已经写死成 const Student&。想一想如果写成 Student s（传值），
 *         每次调用要多拷一个 string；sort 里比较次数是 n log n 量级，
 *         n = 1e5 时这就是「本地能跑、交题超时」的差距。
 *         结构体里只要有 string / vector，参数就一律 const T&，没有例外。
 * ------------------------------------------------------------------------- */
int totalOf(const Student& s)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：找总分最高的人的名字（并列取学号最小的）
 *
 *   期望：
 *     topStudentName({})  -> ""       ← 空表必须给个明确答案
 *     topStudentName({{"Only", 10, 10, 10, 1}}) -> "Only"
 *     {Anna 240 id1, Bella 240 id2, Cathy 220 id3} -> "Anna"
 *     {Bella 240 id2, Anna 240 id1}               -> "Anna"   ← 跟输入顺序无关
 *     {P1 0 id9, P2 0 id3}                        -> "P2"     ← 全是 0 也要比学号
 *
 *   提示：一个 for 加一个「当前最优下标」，边扫边换，别先排序再取第一个（O(n log n)）。
 *         比较规则两句话：总分更高就换；总分相同且学号更小也换。
 *         循环变量的初值别写 0 分当基准——全负/全 0 的数据会教你做人，
 *         ex03 讲过：累加器初始化 0，最值初始化第一个元素。
 * ------------------------------------------------------------------------- */
string topStudentName(const vector<Student>& v)
{
    /* TODO 在这里写实现 */
    return "";
}

/* ---------------------------------------------------------------------------
 * TODO 3：多级排序——总分降序 → 姓名字典升序 → 学号升序
 *
 *   期望（原地排好，函数没有返回值）：
 *     {Xiaoming 264 id1, Xiaohong 285 id2, Xiaogang 264 id3, Xiaoli 285 id4}
 *       -> Xiaohong, Xiaoli, Xiaogang, Xiaoming
 *     三个 LiLei 全是 180 分：id 7、2、5 -> 排完是 id 2、5、7
 *
 *   提示：模板就是「逐层 if，最后一个 return」：
 *           if (a.这一层 != b.这一层) return a.这一层 > b.这一层;   // 或 <
 *           return 下一层;
 *         三个关键字写两个 if。每一层只在「平局」时才把决定权交给下一层。
 *         参数写 const Student&；比较函数只回答「a 该在 b 前面吗」。
 * ------------------------------------------------------------------------- */
void rankStudents(vector<Student>& v)
{
    /* TODO 在这里写实现 */
    return;
}

/* ---------------------------------------------------------------------------
 * TODO 4：按 y 坐标排序——函数指针版比较器 + 严格弱序（本练习的陷阱题）
 *
 *   约定：belowY(a, b) 回答「a 是不是该排在 b 前面」：y 小的在前，y 相同 x 小的在前。
 *         sortPointsByY 就一句 sort，比较器传 belowY 这个函数名。
 *
 *   期望：
 *     belowY({0, 1}, {0, 2}) -> true       belowY({0, 2}, {0, 1}) -> false
 *     belowY({0, 1}, {0, 1}) -> false      ← 自己不能排在自己前面！
 *     belowY({1, 1}, {3, 1}) -> true       belowY({3, 1}, {1, 1}) -> false
 *     sortPointsByY({(3,1), (1,3), (2,1), (0,0)}) -> (0,0) (2,1) (3,1) (1,3)
 *
 *   提示：这一题的重点在下面这条铁律——
 *         **比较函数里只能出现 < 或 >，绝不能写 <= 或 >=。**
 *         写成 `return a.y <= b.y;` 时 belowY(a, a) 返回 true，
 *         等于告诉 sort「a 比 a 自己还小」。sort 内部的循环条件是
 *         「找到不比 pivot 小的就停」，比较器一旦自反，那个循环停不下来，
 *         指针会一路走出数组末尾——小数据上「看起来完全正常」，
 *         数据一大直接段错误 RE。这是 OJ 上最玄学的 RE 成因，没有之一。
 *         所以断言里我只测 belowY 的正确行为，那种写法绝不允许你交上去。
 *         传函数名 `sort(v.begin(), v.end(), belowY)` 和传 lambda 是等价的，
 *         两种写法都要会：规则只在别处复用时抽成函数，否则就地写 lambda。
 * ------------------------------------------------------------------------- */
bool belowY(const Point& a, const Point& b)
{
    /* TODO 在这里写实现 */
    return false;
}

void sortPointsByY(vector<Point>& v)
{
    /* TODO 在这里写实现 */
    return;
}

/* ---------------------------------------------------------------------------
 * TODO 5：去重——sort + unique + erase 三件套
 *
 *   约定：把 v 排好序、去掉重复值，返回去重之后还剩几个元素（v 本身要改）。
 *
 *   期望：
 *     dedup({})               -> 0，v 变成 {}
 *     dedup({5, 5, 5})        -> 1，v 变成 {5}
 *     dedup({3, 1, 2, 3, 1})  -> 3，v 变成 {1, 2, 3}
 *     dedup({1, 2})           -> 2，v 变成 {1, 2}
 *     dedup({-1, 0, -1, 2})   -> 3，v 变成 {-1, 0, 2}
 *
 *   提示：三句缺一不可，背下来：
 *           sort(v.begin(), v.end());
 *           v.erase(unique(v.begin(), v.end()), v.end());
 *         unique 只把**相邻**的重复值挤到后面，返回「逻辑末尾」的迭代器，
 *         它既不排序也不改变容器长度。所以：不 sort 直接用 unique 是错的，
 *         不 erase 容器长度就还是原来那个。
 * ------------------------------------------------------------------------- */
int dedup(vector<int>& v)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：只按总分降序排，但要求同分的人保持输入顺序
 *
 *   期望：
 *     {A1 240 id1, B1 230 id2, C1 240 id3}  -> A1, C1, B1
 *     {P1 180, P2 180, P3 180}              -> P1, P2, P3（顺序一点不动）
 *
 *   提示：这就是 stable_sort 的用途——相等元素的相对顺序保证不变。
 *         普通 sort 不保证，而且它「这次碰巧没打乱」不代表下次也不打乱。
 *         比较函数只写总分，不要写学号（写了就变成任务 3 了）。
 *         竞赛里更稳的做法是把「输入序号」当成最后一个决胜关键字塞进比较函数，
 *         这样连算法稳定性都不依赖，换成升序降序也不会翻车。
 * ------------------------------------------------------------------------- */
void sortByScoreStable(vector<Student>& v)
{
    /* TODO 在这里写实现 */
    return;
}

void dumpStudents(const vector<Student>& v, const string& tag)
{
    cout << "  " << tag << "：";
    for (const Student& s : v) cout << s.name << "(" << totalOf(s) << ") ";
    cout << "\n";
}

int main()
{
    cout << "========== 任务 1：总分 ==========\n";
    assert(totalOf({"Anna", 90, 80, 70, 1}) == 240);
    assert(totalOf({"", 0, 0, 0, 0}) == 0);
    assert(totalOf({"Zed", 100, 100, 100, 9}) == 300);
    cout << "  totalOf 全部通过\n";

    cout << "\n========== 任务 2：最高分是谁 ==========\n";
    /* 测试数据先起名再传：把 {{...}} 这种临时初始化列表直接写在 assert 里，
     * 新版 GCC 会报一句 dangling-pointer 噪音。数据有名有姓也更易读。 */
    vector<Student> none;
    vector<Student> solo = {{"Only", 10, 10, 10, 1}};
    vector<Student> trio = {{"Anna", 90, 80, 70, 1},
                            {"Bella", 80, 80, 80, 2},
                            {"Cathy", 100, 60, 60, 3}};
    vector<Student> swapped = {{"Bella", 80, 80, 80, 2},
                               {"Anna", 90, 80, 70, 1}};
    vector<Student> zeros = {{"P1", 0, 0, 0, 9}, {"P2", 0, 0, 0, 3}};
    assert(topStudentName(none) == "");
    assert(topStudentName(solo) == "Only");
    assert(topStudentName(trio) == "Anna");
    assert(topStudentName(swapped) == "Anna");
    assert(topStudentName(zeros) == "P2");
    cout << "  topStudentName 全部通过\n";

    cout << "\n========== 任务 3：三级排序 ==========\n";
    {
        vector<Student> cls = {{"Xiaoming", 88, 88, 88, 1},
                               {"Xiaohong", 95, 95, 95, 2},
                               {"Xiaogang", 88, 88, 88, 3},
                               {"Xiaoli", 95, 95, 95, 4}};
        rankStudents(cls);
        assert(cls[0].name == "Xiaohong");
        assert(cls[1].name == "Xiaoli");
        assert(cls[2].name == "Xiaogang");
        assert(cls[3].name == "Xiaoming");
    }
    {
        vector<Student> same = {{"LiLei", 60, 60, 60, 7},
                                {"LiLei", 60, 60, 60, 2},
                                {"LiLei", 60, 60, 60, 5}};
        rankStudents(same);
        assert(same[0].id == 2 && same[1].id == 5 && same[2].id == 7);
    }
    cout << "  rankStudents 全部通过\n";

    cout << "\n========== 任务 4：严格弱序 + 函数指针比较器 ==========\n";
    assert(belowY({0, 1}, {0, 2}) == true);
    assert(belowY({0, 2}, {0, 1}) == false);
    assert(belowY({0, 1}, {0, 1}) == false);      // cmp(a, a) 必须是 false
    assert(belowY({1, 1}, {3, 1}) == true);
    assert(belowY({3, 1}, {1, 1}) == false);
    vector<Point> pts = {{3, 1}, {1, 3}, {2, 1}, {0, 0}};
    sortPointsByY(pts);
    assert(pts[0].x == 0 && pts[0].y == 0);
    assert(pts[1].x == 2 && pts[1].y == 1);
    assert(pts[2].x == 3 && pts[2].y == 1);
    assert(pts[3].x == 1 && pts[3].y == 3);
    sortPointsByY(pts);                          // 排一个已经有序的表，不能排坏
    assert(pts[0].y == 0 && pts[3].y == 3);
    cout << "  belowY / sortPointsByY 全部通过\n";

    cout << "\n========== 任务 5：去重三件套 ==========\n";
    vector<int> d0;
    assert(dedup(d0) == 0 && d0 == vector<int>{});
    vector<int> d1 = {5, 5, 5};
    assert(dedup(d1) == 1 && d1 == vector<int>({5}));
    vector<int> d2 = {3, 1, 2, 3, 1};
    assert(dedup(d2) == 3 && d2 == vector<int>({1, 2, 3}));
    vector<int> d3 = {1, 2};
    assert(dedup(d3) == 2 && d3 == vector<int>({1, 2}));
    vector<int> d4 = {-1, 0, -1, 2};
    assert(dedup(d4) == 3 && d4 == vector<int>({-1, 0, 2}));
    cout << "  dedup 全部通过\n";

    cout << "\n========== 任务 6：稳定排序 ==========\n";
    {
        vector<Student> v = {{"A1", 80, 80, 80, 1},
                             {"B1", 90, 70, 70, 2},
                             {"C1", 80, 80, 80, 3}};
        sortByScoreStable(v);
        assert(v[0].name == "A1" && v[1].name == "C1" && v[2].name == "B1");
    }
    {
        vector<Student> v = {{"P1", 60, 60, 60, 1},
                             {"P2", 60, 60, 60, 2},
                             {"P3", 60, 60, 60, 3}};
        sortByScoreStable(v);
        assert(v[0].name == "P1" && v[1].name == "P2" && v[2].name == "P3");
    }
    cout << "  sortByScoreStable 全部通过\n";

    cout << "\n========== 观察题：不改代码，先猜再跑 ==========\n";
    /*  sort 和 stable_sort 到底差在哪：数据量小一点，sort 可能碰巧也没打乱，
     *  所以这里放 20 个人，一半 200 分一半 190 分，只按总分降序排。 */
    vector<Student> big;
    for (int i = 0; i < 20; i++) {
        Student s;
        s.name = "S" + to_string(i);
        int sc = (i % 2 == 0) ? 200 : 190;
        s.chinese = sc; s.math = 0; s.english = 0; s.id = i;
        big.push_back(s);
    }
    vector<Student> bySort = big, byStable = big;
    auto byScoreOnly = [](const Student& a, const Student& b) {
        return totalOf(a) > totalOf(b);         // 只比总分，同分怎么排它不管
    };
    sort(bySort.begin(), bySort.end(), byScoreOnly);
    stable_sort(byStable.begin(), byStable.end(), byScoreOnly);
    dumpStudents(big, "原顺序");
    dumpStudents(bySort, "sort 之后（同分顺序不保证）");
    dumpStudents(byStable, "stable_sort 之后（同分一定是 0 2 4 ... 1 3 5 ...）");
    cout << "  两次跑出来的 sort 结果可能不一样，stable_sort 的结果永远是确定的。\n"
         << "  题目说「同分按输入顺序输出」时，要么 stable_sort，\n"
         << "  要么把输入序号写进比较函数当最后一层——后者更稳，别赌算法行为。\n";

    vector<int> idx(big.size());
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int i, int j) {
        if (totalOf(big[i]) != totalOf(big[j])) return totalOf(big[i]) > totalOf(big[j]);
        return big[i].id < big[j].id;
    });
    cout << "  排下标（原数组一个字节都没动）：名次顺序 = ";
    for (int i : idx) cout << big[i].name << " ";
    cout << "\n  对象大、又要来回换几种排序规则时，排下标比排对象便宜得多。\n";

    vector<pair<int, int>> seg = {{2, 5}, {1, 9}, {2, 3}, {1, 2}};
    sort(seg.begin(), seg.end());
    cout << "  pair 自带字典序，sort 连比较器都不用写：";
    for (const auto& p : seg) cout << "(" << p.first << "," << p.second << ") ";
    cout << "\n";

    /*  还有两个纯语法坑，写在注释里：
     *  ① struct 结尾那个分号不能漏。漏了报错会指向下一个声明，
     *     看起来跟结构体毫无关系，新手能卡半小时。
     *  ② C++17 里 Student a, b;  a == b; 编译不过——结构体没有默认相等比较。
     *     要么逐字段比，要么自己写 operator==，要么升到 C++20。
     *  ③ 带 string 字段的 struct 拷贝是有堆分配的，别在热路径上按值传。
     */
    cout << "  结构体比较：a == b 在 C++17 下编译不过，得自己写 operator==。\n";

    cout << "\n>>> ex07 全部通过。下一站：projects/score_report（把这一章做成多文件版本）\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

int totalOf(const Student& s)
{
    return s.chinese + s.math + s.english;
}

string topStudentName(const vector<Student>& v)
{
    if (v.empty()) return "";
    int best = 0;
    for (int i = 1; i < (int)v.size(); i++) {
        if (totalOf(v[i]) > totalOf(v[best])) best = i;
        else if (totalOf(v[i]) == totalOf(v[best]) && v[i].id < v[best].id) best = i;
    }
    return v[best].name;
}

void rankStudents(vector<Student>& v)
{
    sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        int ta = a.chinese + a.math + a.english;
        int tb = b.chinese + b.math + b.english;
        if (ta != tb) return ta > tb;                 // 第一关键字：总分降序
        if (a.name != b.name) return a.name < b.name; // 第二关键字：姓名字典升序
        return a.id < b.id;                           // 第三关键字：学号升序
    });
}

bool belowY(const Point& a, const Point& b)
{
    if (a.y != b.y) return a.y < b.y;
    return a.x < b.x;
}

void sortPointsByY(vector<Point>& v)
{
    sort(v.begin(), v.end(), belowY);
}

int dedup(vector<int>& v)
{
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return (int)v.size();
}

void sortByScoreStable(vector<Student>& v)
{
    stable_sort(v.begin(), v.end(), [](const Student& a, const Student& b) {
        return totalOf(a) > totalOf(b);
    });
}

  为什么比较函数只能写 < / >：
    sort 要求比较函数是一个「严格弱序」，其中最直观的一条是 irreflexive：
    cmp(a, a) 必须是 false。写成 a.y <= b.y 时 cmp(a, a) 是 true，
    等于对 sort 撒谎说「a 该排在 a 前面」。它内部的分区循环拿这个条件当终止判据，
    于是指针一路走出数组边界。小数据、O2 没开、内存刚好后面是可用空间时一切正常，
    换个编译器或者数据涨到 1e5 就当场 RE。这种 bug 没法调试，只能靠习惯躲开。

  为什么多关键字要写成「逐层 if，最后一个 return」：
    每一层只负责一件事：这一项能分出胜负就立刻给答案，分不出才交给下一层。
    如果写成 `return a.score > b.score && a.name < b.name;` 这种「一句话全判」，
    分数高但名字靠后的人就会被误判，排序结果直接错。分层写还有一个好处：
    想加一个决胜关键字就在最后补一行，想改方向就改那一行，逻辑不会互相污染。

  为什么 topStudentName 里 best 从 0 开始而不是拿 0 分当基准：
    「找最大」的基准必须来自数据本身。用 0 当基准，遇到全员 0 分或者全是负数，
    循环里那个条件永远不成立，你会返回一个根本没参与比较的默认对象。
    这条规则在所有「找最值」的题里通用，和 ex03 的 minMaxOf 是同一件事。

  为什么 sort + unique + erase 三件套少一句都不行：
    unique 只做一件事：把相邻重复值挤到区间尾部，返回「去重后的逻辑末尾」迭代器。
    它不排序，所以 {3, 1, 3} 里的两个 3 不相邻，一个都删不掉；
    它也不改变容器长度，所以不 erase 的话 v.size() 还是原来的，
    后面再用下标遍历就会读到被搬走的尾巴。
    只想知道「有几种不同元素」时可以省掉 erase：
    int kinds = (int)(unique(v.begin(), v.end()) - v.begin());

  为什么排下标是个值得记住的技巧：
    排序的过程本质是不断交换元素。Student 里带 string，一次交换就是几次堆操作；
    而 int 下标只有 4 字节。更重要的是「同一份数据要按好几种规则排」时，
    排下标不用反复拷贝对象，原数组的顺序也一直保得住——
    统计名次、离线处理询问（第 09 章的扫描线）都靠这一招。
 * ========================================================================== */
