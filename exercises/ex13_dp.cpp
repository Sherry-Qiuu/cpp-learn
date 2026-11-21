/* ============================================================================
 * ex13_dp.cpp —— 动态规划入门：状态定义 · 转移 · 初始化 · 答案位置
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 把 notes/13 的四步法练成肌肉记忆：状态定义 → 转移方程 → 初始化 → 答案位置
 *   2. 线性 / 网格 / 背包 / 子序列各写一遍模板，把「01 背包容量倒序、完全背包正序」
 *      刻进骨头里——这是 DP 里最值钱的一条知识
 *   3. 三个经典坑在本文件里都有实测陷阱：容量方向写反、初始化漏第一行第一列、
 *      LIS 拿 dp[n-1] 当答案
 *
 * 编译运行：
 *   make run/ex13
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex13_dp.cpp \
 *          -o build/ex13.exe && ./build/ex13.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <cassert>
using namespace std;

/* ---------------------------------------------------------------------------
 * TODO 1：热身两连——爬楼梯 + 网格路径（一维与二维的最简状态定义）
 *
 *   期望：
 *     stairs(0)  -> 0        n <= 0 没有楼梯可爬
 *     stairs(1)  -> 1
 *     stairs(2)  -> 2
 *     stairs(3)  -> 3
 *     stairs(10) -> 89       ← 讲义实测值
 *     stairs(20) -> 10946
 *
 *     gridPaths(1, 1)  -> 1
 *     gridPaths(1, 7)  -> 1     （只有一条直路）
 *     gridPaths(2, 2)  -> 2
 *     gridPaths(3, 7)  -> 28    ← 讲义实测值，等于组合数 C(8, 2)，可以互相验证
 *     gridPaths(7, 3)  -> 28    （对称）
 *     gridPaths(10, 10) -> 48620
 *     m 或 n <= 0      -> 0
 *
 *   提示：爬楼梯——定义 dp[i] = 爬到第 i 级的方案数；「最后一步」要么从 i-1
 *         跨 1 级来，要么从 i-2 跨 2 级来，两类互斥且完备 → dp[i] = dp[i-1] + dp[i-2]。
 *         初始化 dp[1] = 1、dp[2] = 2，答案在 dp[n]。
 *         方案数涨得比你想的快：int 只装到 stairs(45)，main 里有实测——dp 一律 long long。
 *         网格——f[i][j] = 到 (i, j) 的路径数；只能从上方或左方来。
 *         ★ 初始化最容易漏的就是「第一行、第一列全是 1」——每个格子都只有一条直路。
 *           漏掉它答案是 0（main 的陷阱 2 实测）。网格题「最值初始化成 0、计数初始化成 1」
 *           的差别就在这，想清楚再动手。
 * ------------------------------------------------------------------------- */
long long stairs(int n)
{
    /* TODO 在这里写实现 */
    return -1;
}

long long gridPaths(int m, int n)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：打家劫舍（不能偷相邻的两家，求最大金额）
 *
 *   期望：
 *     rob({2, 7, 9, 3, 1}) -> 12      ← 讲义实测值，偷 2 + 9 + 1
 *     rob({})              -> 0
 *     rob({5})             -> 5
 *     rob({2, 1})          -> 2       rob({1, 2}) -> 2
 *     rob({2, 1, 2})       -> 4       （偷两头）
 *     rob({3, 3, 3, 3})    -> 6       （全相同也不慌）
 *
 *   提示：g[i] = max(g[i-1], g[i-2] + money[i])——「最后一家偷还是不偷」。
 *         i < 2 时下标会越界。讲义给了两条路：写三元判界，或者在数组前面垫两个 0
 *         让下标整体后移。垫 0 更清晰，推荐，而且以后写背包「恰好装满」的 -INF 初值
 *         也是同一招。
 * ------------------------------------------------------------------------- */
int rob(const vector<int>& money)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 3：01 背包——一维滚动数组，容量必须【倒序】
 *
 *   期望：
 *     knap01({2, 3, 4, 5}, {3, 4, 5, 6}, 8) -> 10   ← 讲义那张表的最后一行
 *     knap01({}, {}, 8)      -> 0
 *     knap01({9}, {100}, 8)  -> 0                   （最轻的都装不下，价值 0）
 *     knap01({2}, {3}, 2)    -> 3                   （恰好装满）
 *     knap01({2, 3}, {3, 4}, 1) -> 0
 *     knap01({5, 5}, {6, 7}, 5) -> 7                （二选一）
 *
 *   提示：dp[cap] = max(dp[cap], dp[cap - w[i]] + v[i])，cap 从 C 递减到 w[i]。
 *         ★ 为什么倒序？dp[cap] 要读的是「还没考虑第 i 件」的 dp[cap - w[i]]。
 *           倒序走到 cap 时，cap - w[i] 还没被本轮更新，读到的正是上一行 →
 *           每件物品最多用一次。正序则读到本轮新值 → 重复使用 → 见 TODO 4 和陷阱 1。
 *         讲义表里逐行演化的四个中间状态，main 用实测一行为一行地打给你看。
 * ------------------------------------------------------------------------- */
int knap01(const vector<int>& w, const vector<int>& v, int C)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 4：完全背包——同一段代码，容量改成【正序】
 *
 *   期望：
 *     knapComplete({2, 3, 4, 5}, {3, 4, 5, 6}, 8) -> 12   ← 4 件 (2,3)，讲义实测
 *     knapComplete({2}, {3}, 8)    -> 12
 *     knapComplete({3}, {4}, 8)    -> 8                    （只能放两件，剩 2 装不下）
 *     knapComplete({9}, {100}, 8)  -> 0
 *     knapComplete({}, {}, 5)      -> 0
 *
 *   提示：和 TODO 3 只差循环方向：cap 从 w[i] 递增到 C。
 *         正序时 dp[cap - w[i]] 已经是本轮更新过的值——第 i 件可能已经被用过，
 *         再算一次就用了两件 → 语义自动变成「每件可以用无限次」。
 *         一个方向的差别区分两种完全不同的问题。别背代码，背这句话。
 * ------------------------------------------------------------------------- */
int knapComplete(const vector<int>& w, const vector<int>& v, int C)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 5：最长严格上升子序列（LIS），O(n²) 版
 *
 *   期望：
 *     lis({10, 9, 2, 5, 3, 7, 101, 18}) -> 4    ← 讲义实测值（如 2, 5, 7, 101）
 *     lis({})             -> 0
 *     lis({7})            -> 1
 *     lis({1, 2, 3, 4})   -> 4
 *     lis({4, 3, 2, 1})   -> 1                  （全逆序，每个元素自己成一队）
 *     lis({2, 2, 2, 2})   -> 1                  （严格上升：条件写 <，不是 <=）
 *     lis({1, 3, 5, 2})   -> 3                  ← dp[n-1] 只有 2，答案在 max(dp)！
 *     lis({1, 2, 5, 3, 4}) -> 4
 *
 *   提示：LIS 只能用「以 i 结尾」的定义——「前 i 个」那种定义转移不出来。
 *         d[i] = max(d[j] + 1)，枚举 j < i 且 a[j] < a[i]；初始化所有 d[i] = 1；
 *         ★ 答案是 max(d[i])，不是 d[n-1]：最长序列不一定以最后一个元素结尾。
 *         这是「以 i 结尾」定义必然带来的后果，也是初学者丢答案的最高发位置。
 * ------------------------------------------------------------------------- */
int lis(const vector<int>& a)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：最大子段和（Kadane）
 *
 *   期望：
 *     maxSubarray({-2, 1, -3, 4, -1, 2, 1, -5, 4}) -> 6    （子段 [4, -1, 2, 1]）
 *     maxSubarray({})          -> 0
 *     maxSubarray({-5})        -> -5
 *     maxSubarray({-2, -3, -1}) -> -1   ← 全负数：答案是最大的那个元素，不是 0！
 *     maxSubarray({1, 2, 3})   -> 6
 *     maxSubarray({5, -100, 5}) -> 5
 *     maxSubarray({2, -1, 2})  -> 3
 *
 *   提示：cur[i] = max(a[i], cur[i-1] + a[i])——「以 i 结尾」再写一遍：
 *         要么接在前一段后面，要么从 a[i] 单开一段。答案同样是 max(cur[i])。
 *         经典写法两个滚动变量 cur / best 就够。
 *         ★ 陷阱在初始化：cur 和 best 都从 a[0] 起步。谁写 best = 0 起步，
 *           全负数组就交一个 0 上去——而「选一些元素」必须至少选一个。
 * ------------------------------------------------------------------------- */
int maxSubarray(const vector<int>& a)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 7：最长公共子序列（LCS），二维 DP
 *
 *   期望：
 *     lcs("ABCBDAB", "BDCABA") -> 4        （如 BCBA，教科书例题）
 *     lcs("AGGTAB", "GXTXAYB") -> 4        （GTAB）
 *     lcs("", "abc")   -> 0
 *     lcs("abc", "")   -> 0
 *     lcs("ABC", "ABC") -> 3
 *     lcs("ABC", "DEF") -> 0               （没有公共字符）
 *     lcs("AAA", "AA")  -> 2               （重复字符照数）
 *
 *   提示：f[i][j] = s 的前 i 个与 t 的前 j 个的 LCS 长度。「前 i 个」的定义
 *         在这里最顺手，答案在 f[m][n]。
 *         末字符相等 → f[i][j] = f[i-1][j-1] + 1；不等 → max(f[i-1][j], f[i][j-1])。
 *         ★ 数组开 (m+1) x (n+1)，第 0 行第 0 列代表「空串」，天然全 0——
 *           又见垫一行垫一列：把 i-1 < 0 的边界检查整个消掉。编辑距离同理。
 *         注意「子序列」不要求连续；要求连续的是「最长公共子串」，转移不同。
 * ------------------------------------------------------------------------- */
int lcs(const string& s, const string& t)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* 01 背包把容量写成「正序」的演示版：和正确版只差一行循环方向，
 * 算出来的却以下是一条完全背包的答案。只用来打印，不是练习内容。 */
int knap01ForwardDemo(const vector<int>& w, const vector<int>& v, int C)
{
    vector<int> dp(C + 1, 0);
    for (int i = 0; i < (int)w.size(); i++)
        for (int cap = w[i]; cap <= C; cap++)                    // ★ 方向写反了
            dp[cap] = max(dp[cap], dp[cap - w[i]] + v[i]);
    return dp[C];
}

/* 网格路径漏掉「第一行第一列全是 1」的演示版：全表 0 起步，递推自然是 0。 */
long long gridPathsNoInitDemo(int m, int n)
{
    vector<vector<long long>> f(m, vector<long long>(n, 0));
    for (int i = 1; i < m; i++)
        for (int j = 1; j < n; j++)
            f[i][j] = f[i - 1][j] + f[i][j - 1];
    return f[m - 1][n - 1];
}

/* 只取最后一个格子 d[n-1] 当答案的 LIS 演示版。 */
int lisLastCellDemo(const vector<int>& a)
{
    int n = (int)a.size();
    if (n == 0) return 0;
    vector<int> d(n, 1);
    for (int i = 1; i < n; i++)
        for (int j = 0; j < i; j++)
            if (a[j] < a[i]) d[i] = max(d[i], d[j] + 1);
    return d[n - 1];
}

/* 打印 01 背包一维滚动表逐行演化（讲义第 3 节那张表）。纯展示用。 */
vector<vector<int>> knap01Trace(const vector<int>& w, const vector<int>& v, int C)
{
    vector<vector<int>> rows;
    vector<int> dp(C + 1, 0);
    rows.push_back(dp);
    for (int i = 0; i < (int)w.size(); i++) {
        for (int cap = C; cap >= w[i]; cap--)
            dp[cap] = max(dp[cap], dp[cap - w[i]] + v[i]);
        rows.push_back(dp);
    }
    return rows;
}

int main()
{
    cout << "========== 任务 1（上）：爬楼梯 ==========\n";
    assert(stairs(0) == 0);
    assert(stairs(1) == 1);
    assert(stairs(2) == 2);
    assert(stairs(3) == 3);
    assert(stairs(10) == 89);
    assert(stairs(20) == 10946);
    cout << "  1..10 级：" << stairs(1);
    for (int i = 2; i <= 10; i++) cout << ", " << stairs(i);
    cout << "\n  就是斐波那契。stairs(45) = " << stairs(45)
         << "，离 INT_MAX = " << INT_MAX << " 只差一步；\n"
         << "  stairs(46) = " << stairs(46) << " 用 int 已经装不下了——方案数类一律 long long。\n";
    cout << "  stairs 全部通过\n";

    cout << "\n========== 任务 1（下）：网格路径 ==========\n";
    assert(gridPaths(1, 1) == 1);
    assert(gridPaths(1, 7) == 1);
    assert(gridPaths(2, 2) == 2);
    assert(gridPaths(3, 7) == 28);
    assert(gridPaths(7, 3) == 28);
    assert(gridPaths(10, 10) == 48620);
    assert(gridPaths(0, 5) == 0);
    assert(gridPaths(5, 0) == 0);
    cout << "  3x7 -> " << gridPaths(3, 7) << "（组合数 C(8,2) = 28，两条路互相验证）\n";
    cout << "  gridPaths 全部通过\n";

    cout << "\n========== 任务 2：打家劫舍 ==========\n";
    assert(rob({2, 7, 9, 3, 1}) == 12);
    assert(rob({}) == 0);
    assert(rob({5}) == 5);
    assert(rob({2, 1}) == 2);
    assert(rob({1, 2}) == 2);
    assert(rob({2, 1, 2}) == 4);
    assert(rob({3, 3, 3, 3}) == 6);
    cout << "  {2,7,9,3,1} -> " << rob({2, 7, 9, 3, 1}) << "（偷 2 + 9 + 1，不是看着吓人的 7 + 3）\n";
    cout << "  rob 全部通过\n";

    cout << "\n========== 任务 3：01 背包（倒序） ==========\n";
    {
        vector<int> w = {2, 3, 4, 5}, v = {3, 4, 5, 6};
        assert(knap01(w, v, 8) == 10);
        assert(knap01({}, {}, 8) == 0);
        assert(knap01({9}, {100}, 8) == 0);
        assert(knap01({2}, {3}, 2) == 3);
        assert(knap01({2, 3}, {3, 4}, 1) == 0);
        assert(knap01({5, 5}, {6, 7}, 5) == 7);
        vector<vector<int>> rows = knap01Trace(w, v, 8);
        assert((rows == vector<vector<int>>({
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 3, 3, 3, 3, 3, 3, 3},
            {0, 0, 3, 4, 4, 7, 7, 7, 7},
            {0, 0, 3, 4, 5, 7, 8, 9, 9},
            {0, 0, 3, 4, 5, 7, 8, 9, 10}})));
        cout << "  逐件演化（cap 0..8），和讲义第 3 节的表逐格核对：\n";
        const char* label[5] = {"初始    ", "物品1(2,3)", "物品2(3,4)", "物品3(4,5)", "物品4(5,6)"};
        for (size_t r = 0; r < rows.size(); r++) {
            cout << "    " << label[r] << ":";
            for (size_t c = 0; c < rows[r].size(); c++) cout << " " << rows[r][c];
            cout << "\n";
        }
    }
    cout << "  knap01 全部通过\n";

    cout << "\n========== 任务 4：完全背包（正序） ==========\n";
    assert(knapComplete({2, 3, 4, 5}, {3, 4, 5, 6}, 8) == 12);
    assert(knapComplete({2}, {3}, 8) == 12);
    assert(knapComplete({3}, {4}, 8) == 8);
    assert(knapComplete({9}, {100}, 8) == 0);
    assert(knapComplete({}, {}, 5) == 0);
    cout << "  同一组数据：01 背包 = " << knap01({2, 3, 4, 5}, {3, 4, 5, 6}, 8)
         << "，完全背包 = " << knapComplete({2, 3, 4, 5}, {3, 4, 5, 6}, 8)
         << "（4 件 (2,3) 装满是 12，但 01 背包里 (2,3) 只有一件）\n";
    cout << "  knapComplete 全部通过\n";

    cout << "\n========== 任务 5：LIS ==========\n";
    assert(lis({10, 9, 2, 5, 3, 7, 101, 18}) == 4);
    assert(lis({}) == 0);
    assert(lis({7}) == 1);
    assert(lis({1, 2, 3, 4}) == 4);
    assert(lis({4, 3, 2, 1}) == 1);
    assert(lis({2, 2, 2, 2}) == 1);
    assert(lis({1, 3, 5, 2}) == 3);
    assert(lis({1, 2, 5, 3, 4}) == 4);
    cout << "  lis 全部通过\n";

    cout << "\n========== 任务 6：最大子段和 ==========\n";
    assert(maxSubarray({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    assert(maxSubarray({}) == 0);
    assert(maxSubarray({-5}) == -5);
    assert(maxSubarray({-2, -3, -1}) == -1);
    assert(maxSubarray({1, 2, 3}) == 6);
    assert(maxSubarray({5, -100, 5}) == 5);
    assert(maxSubarray({2, -1, 2}) == 3);
    cout << "  maxSubarray 全部通过\n";

    cout << "\n========== 任务 7：LCS ==========\n";
    assert(lcs("ABCBDAB", "BDCABA") == 4);
    assert(lcs("AGGTAB", "GXTXAYB") == 4);
    assert(lcs("", "abc") == 0);
    assert(lcs("abc", "") == 0);
    assert(lcs("ABC", "ABC") == 3);
    assert(lcs("ABC", "DEF") == 0);
    assert(lcs("AAA", "AA") == 2);
    cout << "  lcs 全部通过\n";

    cout << "\n========== 陷阱 1：01 背包容量写成正序 ==========\n";
    {
        vector<int> w = {2, 3, 4, 5}, v = {3, 4, 5, 6};
        int right = knap01(w, v, 8);
        int flipped = knap01ForwardDemo(w, v, 8);
        assert(right == 10);
        assert(flipped == 12);                 // 错误值也敢 assert：错得非常安静
        cout << "  正确（倒序）= " << right << "，正序 = " << flipped << "。\n"
             << "  偏大的 2 正是物品 1（2,3）被重复用了一次：dp[cap-2] 读到的是本轮新值。\n"
             << "  最阴的地方：flipped 恰好等于完全背包的正确答案 12（任务 4 实测）——\n"
             << "  如果题目要的是 01 背包，你交上去的就是「样例像对了、大样例全错」。\n"
             << "  讲义表格里每一行的演化（任务 3 打印）就是这个方向的差别放大出来的。\n";
    }

    cout << "\n========== 陷阱 2：初始化漏第一行第一列 ==========\n";
    {
        long long right = gridPaths(3, 7);
        long long missing = gridPathsNoInitDemo(3, 7);
        assert(right == 28);
        assert(missing == 0);
        cout << "  正确 " << right << "，漏初始化 " << missing << "。\n"
             << "  转移一行没写错，只是没人给第一行第一列发「1」，全表就全是 0。\n"
             << "  计数类 DP 的初始化是「有路 = 1」，不是「没路 = 0」——先把最小的那张表手填出来。\n";
    }

    cout << "\n========== 陷阱 3：LIS 拿 dp[n-1] 当答案 ==========\n";
    {
        vector<int> a = {1, 3, 5, 2};
        assert(lis(a) == 3);
        assert(lisLastCellDemo(a) == 2);       // d[n-1] 只是「以 2 结尾」的最长串
        cout << "  {1,3,5,2}：正确答案 3（1,3,5），只看最后一格 = " << lisLastCellDemo(a)
             << "。\n  讲义那组 {10,9,2,5,3,7,101,18} 碰巧 d[n-1] 也是 4，样例过了大样例炸——\n"
             << "  「以 i 结尾」的定义欠你的债，必须在 max(d[i]) 处还。\n";
    }

    cout << "\n>>> ex13 全部通过。下一站：ex14_contest_drill.cpp（综合演练 + 竞赛习惯）\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

long long stairs(int n)
{
    if (n <= 0) return 0;
    vector<long long> dp(n + 1, 0);
    dp[1] = 1;
    if (n >= 2) dp[2] = 2;
    for (int i = 3; i <= n; i++) dp[i] = dp[i - 1] + dp[i - 2];
    return dp[n];
}

long long gridPaths(int m, int n)
{
    if (m <= 0 || n <= 0) return 0;
    vector<vector<long long>> f(m, vector<long long>(n, 1));   // ★ 第一行第一列全是 1
    for (int i = 1; i < m; i++)
        for (int j = 1; j < n; j++)
            f[i][j] = f[i - 1][j] + f[i][j - 1];
    return f[m - 1][n - 1];
}

int rob(const vector<int>& money)
{
    int n = (int)money.size();
    vector<int> g(n + 2, 0);                    // 前面垫两个 0，下标整体后移
    for (int i = 0; i < n; i++)
        g[i + 2] = max(g[i + 1], g[i] + money[i]);
    return g[n + 1];
}

int knap01(const vector<int>& w, const vector<int>& v, int C)
{
    vector<int> dp(C + 1, 0);
    for (int i = 0; i < (int)w.size(); i++)
        for (int cap = C; cap >= w[i]; cap--)   // ★ 倒序：读到的是「上一行」
            dp[cap] = max(dp[cap], dp[cap - w[i]] + v[i]);
    return dp[C];
}

int knapComplete(const vector<int>& w, const vector<int>& v, int C)
{
    vector<int> dp(C + 1, 0);
    for (int i = 0; i < (int)w.size(); i++)
        for (int cap = w[i]; cap <= C; cap++)   // ★ 正序：本轮新值可被继续加工
            dp[cap] = max(dp[cap], dp[cap - w[i]] + v[i]);
    return dp[C];
}

int lis(const vector<int>& a)
{
    int n = (int)a.size();
    if (n == 0) return 0;
    vector<int> d(n, 1);                        // 每个元素自己就是长度 1 的序列
    int best = 1;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++)
            if (a[j] < a[i]) d[i] = max(d[i], d[j] + 1);   // 严格：< 不是 <=
        best = max(best, d[i]);
    }
    return best;                                // ★ 答案是 max，不是 d[n-1]
}

int maxSubarray(const vector<int>& a)
{
    if (a.empty()) return 0;
    int cur = a[0], best = a[0];                // ★ 从 a[0] 起步，不是 0
    int n = (int)a.size();
    for (int i = 1; i < n; i++) {
        cur = max(a[i], cur + a[i]);
        best = max(best, cur);
    }
    return best;
}

int lcs(const string& s, const string& t)
{
    int m = (int)s.size(), n = (int)t.size();
    vector<vector<int>> f(m + 1, vector<int>(n + 1, 0));   // 第 0 行/列 = 空串
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            if (s[i - 1] == t[j - 1]) f[i][j] = f[i - 1][j - 1] + 1;
            else f[i][j] = max(f[i - 1][j], f[i][j - 1]);
    return f[m][n];
}

  为什么转移方程要从「最后一步」想：
    dp[i] 表示的是一个已经定义清楚的问题，转移问的是「这个问题的最优解，
    最后一步做了哪个选择」。爬楼梯的最后一步只有跨 1 或跨 2 两种，互斥且完备，
    所以两类的方案数直接相加；打家劫舍的最后一步只有偷或不偷第 i 家，
    偷就必须接 i-2，不偷就继承 i-1。把所有可能的「最后一步」枚举干净，
    方程就写完了——比凭空背 max(g[i-1], g[i-2]+a[i]) 可靠得多。

  为什么 01 背包必须倒序、完全背包必须正序：
    一维数组是二维表「一行」的替身，dp[cap] 里的「上一行」指还没考虑第 i 件物品时
    的值。倒序从 C 往 w[i] 走，算 dp[cap] 时 cap - w[i] < cap 还没被本轮碰过，
    读到的还是上一行 → 第 i 件最多进一次 → 01 背包。正序时 cap - w[i] 已经被本轮
    更新过，里面可能已经装着第 i 件，再加一次就是两件 → 每件无限用 → 完全背包。
    main 里实测的 10 和 12 正是同一组数据在两种方向下的答案，差的 2 是 (2,3) 被白嫖了一次。

  为什么 LIS 的答案在 max(d[i]) 而 Kadane 也是 max：
    两者都是「以 i 结尾」的定义：d[i] 只统计以 a[i] 收尾的那一族子序列。
    最长的那条完全可能在中间某个元素就收尾了（{1,3,5,2} 最长是 1,3,5，尾元素 2 只能接出
    长度 2），所以逐格取 max 才能把答案捞回来。「前 i 个」定义答案在 dp[n]，
    「以 i 结尾」定义答案在 max(dp)——状态定义决定了答案位置，这是四步法的第四步。

  为什么 maxSubarray 的 best 要从 a[0] 而不是 0 起步：
    题目要求「选一些元素」，至少选一个。全负数组（{-2,-3,-1}）的正确答案是 -1，
    best = 0 起步等于默许「什么都不选」，交上去 0 就是 WA。初始化必须对应一个
    真实存在的状态，这是讲义五个高频错误里「无解被当成价值 0」的同一件事。

  为什么 LCS 要把数组开成 (m+1) x (n+1)：
    第 0 行第 0 列表示「其中一边是空串」，公共长度天然是 0，转移里 f[i-1][...]
    也永远不越界，一行代码的判界都省了。背包垫 dp[0]、打家劫舍垫两个 0、LCS 垫行列，
    是同一招：用一个不存在的哨兵状态，把边界条件吸收进通项。DP 写得不别扭，
    一半的功夫在这一招上。
 * ========================================================================== */
