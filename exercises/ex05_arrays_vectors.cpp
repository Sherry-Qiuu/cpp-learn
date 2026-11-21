/* ============================================================================
 * ex05_arrays_vectors.cpp —— 一维数组 · 二维表 · vector · 下标与越界
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 把「下标从 0 起算、区间右端不开」练成条件反射，off-by-one 一个都不写错
 *   2. 会按行、按列两种顺序扫二维表，知道 a[0].size() 什么时候会崩
 *   3. 用 vector 现开现用：前缀和、前缀最值、杨辉三角这几个统计模板要能默写
 *   4. 认清 size() 是无符号这件事能炸出什么，以及变长数组为什么不是标准 C++
 *
 * 编译运行：
 *   make run/ex05
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex05_arrays_vectors.cpp \
 *          -o build/ex05.exe && ./build/ex05.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cassert>
using namespace std;

/* ---------------------------------------------------------------------------
 * TODO 1：原地反转（不许再开一个 vector）
 *
 *   期望：
 *     reverseInPlace({})            -> {}
 *     reverseInPlace({7})           -> {7}
 *     reverseInPlace({1, 2, 3})     -> {3, 2, 1}
 *     reverseInPlace({1, 2, 3, 4})  -> {4, 3, 2, 1}
 *
 *   提示：两个下标相向走：i 从 0，j 从 size()-1，交换后 i++、j--，条件写 i < j。
 *         循环变量用 int 并强转一次 (int)v.size() - 1，理由见任务 6。
 *         空表时 j = -1，i < j 立刻为假，一次都不进——这就是 int 的好处。
 *         标准库有 std::reverse，但这一题要你亲手把双指针写对。
 * ------------------------------------------------------------------------- */
void reverseInPlace(vector<int>& v)
{
    /* TODO 在这里写实现 */
    return;
}

/* ---------------------------------------------------------------------------
 * TODO 2：二维表按列求和（外层循环是「列」）
 *
 *   约定：g 非空时每行等长；g 是空表时返回空表。
 *
 *   期望：
 *     colSums({{1, 2, 3}, {4, 5, 6}}) -> {5, 7, 9}
 *     colSums({{7}})                  -> {7}
 *     colSums({{1}, {2}, {3}})        -> {6}
 *     colSums({{}})                   -> {}   ← 一行都没有列
 *     colSums({})                     -> {}
 *
 *   提示：列数 = g[0].size()，但 g 为空时 g[0] 根本不存在，越界读，
 *         编译器一声不吭。所以**第一行先判 g.empty() 并 return**。
 *         循环顺序是「外列内行」，内存访问跨行跳，比按行扫慢——
 *         n 上千的网格题里这就是缓存命中率的区别。正确性优先，先写对。
 * ------------------------------------------------------------------------- */
vector<int> colSums(const vector<vector<int>>& g)
{
    /* TODO 在这里写实现 */
    return {};
}

/* ---------------------------------------------------------------------------
 * TODO 3：矩阵顺时针旋转 90°（n 行 m 列 -> m 行 n 列）
 *
 *   期望：
 *     rotate90cw({{1, 2}, {3, 4}})         -> {{3, 1}, {4, 2}}
 *     rotate90cw({{1, 2, 3}, {4, 5, 6}})   -> {{4, 1}, {5, 2}, {6, 3}}
 *     rotate90cw({{5}})                    -> {{5}}
 *     rotate90cw({})                       -> {}
 *
 *   提示：先在纸上画 2x3 的格子，标出每个格子转完落到哪。结论是
 *         out[j][rows - 1 - i] = g[i][j]（i 是行、j 是列）。
 *         别一上来就背公式，写出「第 j 列倒着变成第 j 行」这句话就够了。
 *         返回值是新建的表：先 vector<vector<int>> out(m, vector<int>(n, 0)) 开够，
 *         再往里填；对着空表 push_back 下标会当场炸。
 * ------------------------------------------------------------------------- */
vector<vector<int>> rotate90cw(const vector<vector<int>>& g)
{
    /* TODO 在这里写实现 */
    return {};
}

/* ---------------------------------------------------------------------------
 * TODO 4：杨辉三角前 n 行（二维 vector 的经典递推）
 *
 *   期望：
 *     yangHui(0) -> {}
 *     yangHui(1) -> {{1}}
 *     yangHui(5) -> {{1}, {1,1}, {1,2,1}, {1,3,3,1}, {1,4,6,4,1}}
 *
 *   提示：第 i 行有 i+1 个数，两头都是 1，中间 a[i][j] = a[i-1][j-1] + a[i-1][j]。
 *         j 从 1 走到 i-1 时，j-1 一定存在吗？i == 0 时上一行存在吗？
 *         两个问句就是这题的全部边界。要么单独处理 n == 0，
 *         要么按讲义的写法把「缺的位置当 0」补出来。
 * ------------------------------------------------------------------------- */
vector<vector<int>> yangHui(int n)
{
    /* TODO 在这里写实现 */
    return {};
}

/* ---------------------------------------------------------------------------
 * TODO 5：前缀和 + 区间查询（讲义里「下标从 1 起算」的那招）
 *
 *   约定：buildPrefix 返回长度 v.size()+1 的表，pre[0] = 0，
 *         pre[i] = v[0] + ... + v[i-1]。
 *         rangeSum(pre, l, r) 返回 v[l..r]（0 起算、闭区间）的和；
 *         l > r 表示空区间，返回 0。
 *
 *   期望：
 *     buildPrefix({})                 -> {0}
 *     buildPrefix({1, 2, 3})          -> {0, 1, 3, 6}
 *     rangeSum(buildPrefix({2,7,1,9}), 1, 2) -> 8
 *     rangeSum(buildPrefix({2,7,1,9}), 0, 3) -> 19
 *     rangeSum(buildPrefix({2,7,1,9}), 2, 1) -> 0
 *
 *   提示：pre[i+1] = pre[i] + v[i]，一个循环搞定，别在里面再套循环（那是 O(n^2)）。
 *         累加器和返回类型都要 long long：两个 2e9 相加在 int 里当场就废了，
 *         和 ex03 的 sumOf 是同一个坑。
 *         查询写成 pre[r+1] - pre[l]。想清楚为什么是 r+1——差一个就是 WA。
 * ------------------------------------------------------------------------- */
vector<long long> buildPrefix(const vector<int>& v)
{
    /* TODO 在这里写实现 */
    return {};
}

long long rangeSum(const vector<long long>& pre, int l, int r)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：数相邻相等对（本练习的陷阱题）
 *
 *   定义：统计满足 v[i] == v[i+1] 的下标 i 有几个。
 *
 *   期望：
 *     countAdjacentEqual({})                -> 0
 *     countAdjacentEqual({1})               -> 0
 *     countAdjacentEqual({1, 1, 1})         -> 2
 *     countAdjacentEqual({1, 2, 2, 3, 3, 3}) -> 3
 *     countAdjacentEqual({1, 2, 3})         -> 0
 *
 *   提示：最自然的写法是 `for (size_t i = 0; i < v.size() - 1; i++)`。
 *         它在空表上会当场爆炸：v.size() 是无符号数，0 - 1 不是 -1，
 *         而是回绕成 18446744073709551615，条件恒真，第一下就越界读。
 *         在竞赛里这就是 RE 或者一个诡异的 WA。
 *         两种安全写法选一种：`i + 1 < v.size()`（右边不开，永不减 1），
 *         或者 `i < (int)v.size() - 1`（先搬到有符号再减）。
 *         顺带一提：int 和 size_t 直接比较，-Wall 会给你 signed/unsigned 警告。
 * ------------------------------------------------------------------------- */
int countAdjacentEqual(const vector<int>& v)
{
    /* TODO 在这里写实现 */
    return -1;
}

int main()
{
    cout << "========== 任务 1：原地反转 ==========\n";
    vector<int> e1;
    reverseInPlace(e1);
    assert(e1 == vector<int>{});
    vector<int> e2 = {7};
    reverseInPlace(e2);
    assert(e2 == vector<int>({7}));
    vector<int> e3 = {1, 2, 3};
    reverseInPlace(e3);
    assert(e3 == vector<int>({3, 2, 1}));
    vector<int> e4 = {1, 2, 3, 4};
    reverseInPlace(e4);
    assert(e4 == vector<int>({4, 3, 2, 1}));
    cout << "  reverseInPlace 全部通过\n";

    cout << "\n========== 任务 2：按列求和 ==========\n";
    assert(colSums({{1, 2, 3}, {4, 5, 6}}) == vector<int>({5, 7, 9}));
    assert(colSums({{7}}) == vector<int>({7}));
    assert(colSums({{1}, {2}, {3}}) == vector<int>({6}));
    assert(colSums({{}}) == vector<int>{});
    assert(colSums({}) == vector<int>{});
    cout << "  colSums 全部通过\n";

    cout << "\n========== 任务 3：顺时针旋转 90 度 ==========\n";
    assert(rotate90cw({{1, 2}, {3, 4}}) == vector<vector<int>>({{3, 1}, {4, 2}}));
    assert(rotate90cw({{1, 2, 3}, {4, 5, 6}}) ==
           vector<vector<int>>({{4, 1}, {5, 2}, {6, 3}}));
    assert(rotate90cw({{5}}) == vector<vector<int>>({{5}}));
    assert(rotate90cw({{1, 2, 3}}) == vector<vector<int>>({{1}, {2}, {3}}));
    assert(rotate90cw({}) == vector<vector<int>>{});
    cout << "  rotate90cw 全部通过\n";

    cout << "\n========== 任务 4：杨辉三角 ==========\n";
    assert(yangHui(0) == vector<vector<int>>{});
    assert(yangHui(1) == vector<vector<int>>({{1}}));
    assert(yangHui(2) == vector<vector<int>>({{1}, {1, 1}}));
    assert(yangHui(5) == vector<vector<int>>(
        {{1}, {1, 1}, {1, 2, 1}, {1, 3, 3, 1}, {1, 4, 6, 4, 1}}));
    assert(yangHui(6)[5] == vector<int>({1, 5, 10, 10, 5, 1}));
    cout << "  yangHui 全部通过\n";

    cout << "\n========== 任务 5：前缀和与区间查询 ==========\n";
    assert(buildPrefix({}) == vector<long long>({0}));
    assert(buildPrefix({1, 2, 3}) == vector<long long>({0, 1, 3, 6}));
    vector<long long> pre = buildPrefix({2, 7, 1, 9});
    assert(pre == vector<long long>({0, 2, 9, 10, 19}));
    assert(rangeSum(pre, 1, 2) == 8);
    assert(rangeSum(pre, 0, 3) == 19);
    assert(rangeSum(pre, 2, 2) == 1);
    assert(rangeSum(pre, 2, 1) == 0);
    vector<long long> big = buildPrefix({2000000000, 2000000000, -1});
    assert(big == vector<long long>({0, 2000000000LL, 4000000000LL, 3999999999LL}));
    assert(rangeSum(big, 0, 1) == 4000000000LL);
    cout << "  buildPrefix / rangeSum 全部通过\n";

    cout << "\n========== 任务 6：相邻相等对（无符号陷阱） ==========\n";
    assert(countAdjacentEqual({}) == 0);
    assert(countAdjacentEqual({1}) == 0);
    assert(countAdjacentEqual({1, 1, 1}) == 2);
    assert(countAdjacentEqual({1, 2, 2, 3, 3, 3}) == 3);
    assert(countAdjacentEqual({1, 2, 3}) == 0);
    cout << "  countAdjacentEqual 全部通过\n";

    cout << "\n========== 观察题：不改代码，先猜再跑 ==========\n";
    int arr[5] = {2, 4, 6, 8, 10};
    cout << "  在 main 里 sizeof(arr)/sizeof(arr[0]) = "
         << sizeof(arr) / sizeof(arr[0]) << "（数组长度编译期就知道，能算）\n";
    cout << "  但 sizeof(int*) 在这台机器上是 " << sizeof(int*)
         << "；数组一进函数就退化成指针，再拿 sizeof 求长度就是悄悄算错。\n"
         << "  所以函数参数一律写 const vector<int>&，长度交给 .size()。\n";

    vector<int> small = {10, 20, 30};
    try {
        cout << "  正在访问 small.at(5)...\n";
        cout << small.at(5) << "\n";
    } catch (const out_of_range& e) {
        cout << "  at(5) 直接抛 out_of_range（\"" << e.what() << "\"），当场抓住。\n";
    }
    cout << "  换成 small[5] 呢？编译通过、运行不报错，读的是隔壁内存的垃圾值——\n"
         << "  这才是真正的危险。调试时用 make san/ex05 开 Sanitizer 定位行号。\n";

    /* --- 变长数组（VLA）不是标准 C++，所以只放在注释里 ---
     * int n; cin >> n;
     * int a[n];                 // GNU 扩展。-Wpedantic 会警告，MSVC 直接编译不过
     * 标准写法二选一：
     *   int a[1005];            // 定长，大小是编译期常量；大数组放全局，别放栈上
     *   vector<int> a(n);       // 长度是变量：运行时分配、自动释放、能传函数
     */
    cout << "  int a[n]（n 是变量）不是标准 C++：要变长就 vector<int> a(n)，\n"
         << "  要定长就写常量或者 const int MAXN = 100005 开全局。\n";

    vector<int> copy = {1, 2, 3};
    /* 这里标 [[maybe_unused]] 只是为了让 -Wall 闭嘴；真实代码里不标，
     * 编译器就会甩你一句 "variable 'x' set but not used"——那正是它在告诉你：
     * 你改的是副本，这行等于白写。 */
    for ([[maybe_unused]] int x : copy) x = 0;
    cout << "  for (int x : copy) x = 0; 之后 copy[0] = " << copy[0]
         << " —— 没生效。要改元素必须写 int&，这条规则能帮你躲掉一半的坑。\n";
    for (int& x : copy) x = 0;
    cout << "  换成 for (int& x : copy) x = 0; 之后 copy[0] = " << copy[0]
         << "，这次才真的写进容器里。\n";

    cout << "\n>>> ex05 全部通过。下一站：ex06_strings.cpp\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

void reverseInPlace(vector<int>& v)
{
    for (int i = 0, j = (int)v.size() - 1; i < j; i++, j--) {
        int t = v[i];
        v[i] = v[j];
        v[j] = t;
    }
}

vector<int> colSums(const vector<vector<int>>& g)
{
    if (g.empty()) return {};
    int rows = (int)g.size();
    int cols = (int)g[0].size();
    vector<int> res(cols, 0);
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) res[c] += g[r][c];
    }
    return res;
}

vector<vector<int>> rotate90cw(const vector<vector<int>>& g)
{
    if (g.empty()) return {};
    int rows = (int)g.size();
    int cols = (int)g[0].size();
    vector<vector<int>> out(cols, vector<int>(rows, 0));
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            out[c][rows - 1 - r] = g[r][c];
    return out;
}

vector<vector<int>> yangHui(int n)
{
    vector<vector<int>> a;
    for (int i = 0; i < n; i++) {
        vector<int> row(i + 1, 1);
        for (int j = 1; j < i; j++) row[j] = a[i - 1][j - 1] + a[i - 1][j];
        a.push_back(row);
    }
    return a;
}

vector<long long> buildPrefix(const vector<int>& v)
{
    vector<long long> pre(v.size() + 1, 0);
    for (int i = 0; i < (int)v.size(); i++) pre[i + 1] = pre[i] + v[i];
    return pre;
}

long long rangeSum(const vector<long long>& pre, int l, int r)
{
    if (l > r) return 0;
    return pre[r + 1] - pre[l];
}

int countAdjacentEqual(const vector<int>& v)
{
    int cnt = 0;
    for (int i = 0; i + 1 < (int)v.size(); i++) {
        if (v[i] == v[i + 1]) cnt++;
    }
    return cnt;
}

  为什么反转的循环条件写 i < j 而不是 i <= j：
    i == j 时是自己和自己交换，白做一次；写 i <= j 会让整个数组反过来又转回去一半。
    真正的关键是「判定用 < 」：偶数长度时 i、j 交错的那一刻就停，
    奇数长度时正中间那个元素根本没被碰过——它本来就该待在原地。

  为什么 colSums 第一行必须是 if (g.empty()) return {}：
    空表的 g[0] 不存在。原生数组越界不报错，vector 的 operator[] 同样不报错，
    它只是把隔壁内存当元素给你，接着 .size() 读到的就是垃圾。
    「先判空再取 a[0].size()」这条规则在二维 vector 里到处要用；
    讲义自检五问第 4 问，答案就是这个。

  为什么旋转矩阵要按「列倒过来就是行」这句话写，而不是背 out[c][rows-1-r]：
    公式背下来一定会记反符号。纸上标一下第 0 行落到哪一列，
    或者记住极端情况：右上角 g[0][cols-1] 转完一定在右下角。
    拿这一条去验你的公式，两秒钟就能查出错在哪。

  为什么前缀和要多开一格、pre[0] = 0：
    这样「前 i 个元素的和」正好是 pre[i]，区间 [l, r] 的和就是 pre[r+1] - pre[l]，
    不用为 l == 0 单开一个 if。少开一格就会在边界上多写一个分支，
    而分支就是 bug 的入口。多花一个整数的内存换掉一整类边界错误，非常值。

  为什么 buildPrefix 的累加器必须一开始就是 long long：
    v 里放两个 2e9，加法执行的那一刻就在 int 里溢出了（有符号溢出是未定义行为），
    最后再赋给 long long 也救不回来。类型要在运算之前就对，不是之后。

  为什么 countAdjacentEqual 的循环写成 i + 1 < (int)v.size()：
    size() 返回无符号数，v.size() - 1 在空表上回绕成 1.8e19，
    条件恒真，第一次访问 v[0] 就越界。把 -1 挪到不等号左边（i + 1 < size）
    就永远不会减出负数；再加一个 (int) 强转消掉 signed/unsigned 警告。
    这一招在「倒序遍历」「比较相邻两个」两类循环里通用，值得背下来。
 * ========================================================================== */
