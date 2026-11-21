/* ============================================================================
 * ex08_prefix_sum_diff.cpp —— 前缀和 · 二维前缀和 · 差分
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 把「先预处理一遍，之后每次查询 O(1)」写成肌肉记忆
 *   2. 下标从 1 起算的建表法（竞赛主流）要和 ex05 的 0 起算版本自由换算
 *   3. 二维前缀和的容斥两行、差分的 d[l] += v / d[r+1] -= v 能闭眼默写
 *   4. 再一次确认：只要出现累加，类型从声明那一刻起就得是 long long
 *
 * 编译运行：
 *   make run/ex08
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex08_prefix_sum_diff.cpp \
 *          -o build/ex08.exe && ./build/ex08.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <vector>
#include <utility>
#include <cassert>
using namespace std;

/* 一次「给闭区间 [l, r] 整体加 v」的操作。l、r 按 1 起算。 */
struct RangeAdd {
    int l = 0;
    int r = 0;
    long long v = 0;
};

/* ---------------------------------------------------------------------------
 * TODO 1：一维前缀和（这一版全部按 1 起算）
 *
 *   约定：a[0] 只是占位，不存数据，a[1..n] 才是真正的数，n = a.size() - 1。
 *         buildPre1 返回长度和 a 一样的表 p：p[0] = 0，p[i] = a[1] + ... + a[i]。
 *         rangeQuery(p, l, r) 返回 a[l..r] 的和；l > r 表示空区间，返回 0。
 *
 *   期望：
 *     buildPre1({0})                    -> {0}
 *     buildPre1({0, 2, 7, 1, 9})        -> {0, 2, 9, 10, 19}
 *     rangeQuery({0, 2, 9, 10, 19}, 2, 3) -> 8
 *     rangeQuery({0, 2, 9, 10, 19}, 1, 4) -> 19
 *     rangeQuery({0, 2, 9, 10, 19}, 4, 4) -> 9
 *     rangeQuery({0, 2, 9, 10, 19}, 4, 3) -> 0    ← 空区间
 *
 *   提示：建表就一行递推 p[i] = p[i-1] + a[i]，i 从 1 走到 n；查询 p[r] - p[l-1]。
 *         对比 ex05：那里写的是 p[i+1] = p[i] + v[i]，查询 p[r+1] - p[l]。
 *         两套下标绝不能混着写——p[r] - p[l] 会少算一个 a[l]，
 *         p[r+1] - p[l-1] 会多数两头。这种错编译器一声不吭，交上去就是 WA。
 *         写完拿「整段和」自测一遍：rangeQuery(p, 1, n) 必须等于全部元素相加。
 * ------------------------------------------------------------------------- */
vector<long long> buildPre1(const vector<int>& a)
{
    /* TODO 在这里写实现 */
    return {};
}

long long rangeQuery(const vector<long long>& p, int l, int r)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：二维前缀和——矩形求和（讲义里那两行容斥）
 *
 *   约定：g 是 rows × cols 的表（0 起算，每行等长）。
 *         buildPre2 返回 (rows+1) × (cols+1) 的表 s，第 0 行第 0 列全是 0，
 *         s[i][j] = 左上角 (1,1) 到 (i,j) 这块矩形的和（1 起算）。
 *         rectSum(s, r1, c1, r2, c2) 返回 (r1,c1)-(r2,c2) 的矩形和；
 *         r1 > r2 或 c1 > c2 视为空矩形，返回 0。
 *
 *   期望（g = 第 i 行第 j 列放 i*j 的 3×4 表，即 {{1,2,3,4},{2,4,6,8},{3,6,9,12}}）：
 *     rectSum(s, 2, 2, 3, 4) -> 45     ← 讲义 09 实测的那个数
 *     rectSum(s, 1, 1, 3, 4) -> 60     ← 整张表
 *     rectSum(s, 1, 2, 3, 2) -> 12     ← 一整列
 *     rectSum(s, 2, 3, 2, 3) -> 6      ← 单个格子
 *     buildPre2({{5}})       -> {{0, 0}, {0, 5}}
 *
 *   提示：建表 s[i][j] = s[i-1][j] + s[i][j-1] - s[i-1][j-1] + g[i-1][j-1];
 *         查询 s[r2][c2] - s[r1-1][c2] - s[r2][c1-1] + s[r1-1][c1-1];
 *         两行都是「加减加」的容斥：左上两块重叠了一次，所以要补回一角。
 *         为什么多开一行一列？因为 i = 1 时 s[i-1][j] 就是 s[0][j]，
 *         天然等于 0，一个边界特判都不用写。g 为空表时先返回 {{0}}，
 *         别去碰 g[0]（ex05 讲过的坑）。
 * ------------------------------------------------------------------------- */
vector<vector<long long>> buildPre2(const vector<vector<int>>& g)
{
    /* TODO 在这里写实现 */
    return {};
}

long long rectSum(const vector<vector<long long>>& s, int r1, int c1, int r2, int c2)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 3：差分数组——把 m 次「区间加」变成 O(n + m)
 *
 *   约定：初始全是 0，长度 n（下标 1..n）。依次执行 ops 里的每个区间加，
 *         返回最终数组，返回值同样 1 起算（out[0] 恒为 0，只是占位）。
 *
 *   期望：
 *     applyAdds(6, {{1,3,1},{2,6,1},{4,5,1}})  -> {0, 1, 2, 2, 2, 2, 1}
 *          ↑ 讲义 09 实测的那串数
 *     applyAdds(1, {{1,1,5}})                   -> {0, 5}
 *     applyAdds(3, {})                          -> {0, 0, 0, 0}
 *     applyAdds(5, {{2,4,10},{4,5,-3}})         -> {0, 0, 10, 10, 7, -3}
 *
 *   提示：核心两行 d[l] += v; d[r+1] -= v; 全部操作做完再从头做一遍前缀和还原。
 *         本练习的陷阱之一就藏在数组长度里：当 r == n 时 d[r+1] 会写到 d[n+1]，
 *         所以差分数组必须开 n+2（返回值取前 n+1 个）。开成 n+1 就是越界写，
 *         小数据上常常「看起来完全正常」，换个数据就当场爆炸。
 *         另一个陷阱：还原必须在所有修改都记录完之后再做，边改边还原是错的。
 * ------------------------------------------------------------------------- */
vector<long long> applyAdds(int n, const vector<RangeAdd>& ops)
{
    /* TODO 在这里写实现 */
    return {};
}

/* ---------------------------------------------------------------------------
 * TODO 4：差分的应用——最多同时压了多少层
 *
 *   约定：n 个格子（1 起算）初始都是 0，每个区间 [l, r] 表示「这段各盖一层」，
 *         返回被盖得最多的那个格子盖了几层。一个区间都没有、或区间非法（l > r，
 *         直接跳过）时答案是 0。
 *
 *   期望：
 *     maxCovered(10, {{1,3},{2,5},{6,8}})          -> 2
 *     maxCovered(10, {})                           -> 0
 *     maxCovered(5, {{1,5}})                       -> 1
 *     maxCovered(8, {{1,5},{2,6},{3,7},{4,8}})     -> 4
 *     maxCovered(6, {{3,2}})                       -> 0   ← 空区间
 *
 *   提示：和任务 3 同一招，v 恒为 1 而已：d[l] += 1; d[r+1] -= 1;
 *         还原时顺手拿个 max 就行，不必真的存下整个数组。
 *         这就是「公交车上下客」「会议室最多同时几场」的原型：
 *         上车是 l 处的 +1，下车是 r+1 处的 -1。
 * ------------------------------------------------------------------------- */
int maxCovered(int n, const vector<pair<int, int>>& segs)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 5：前缀和 + 枚举——有多少个连续子数组的和恰好是 k
 *
 *   约定：a 是普通 0 起算的数组（这里没有占位格）。统计满足 l <= r 且
 *         a[l] + ... + a[r] == k 的 (l, r) 有几对，返回对数。空表返回 0。
 *
 *   期望：
 *     countSubarraysWithSum({1, -1, 0}, 0)    -> 3
 *     countSubarraysWithSum({1, 2, 3}, 3)     -> 2
 *     countSubarraysWithSum({0, 0, 0}, 0)     -> 6
 *     countSubarraysWithSum({5}, 5)           -> 1
 *     countSubarraysWithSum({5}, 0)           -> 0   ← 空子数组不算
 *     countSubarraysWithSum({}, 0)            -> 0
 *     countSubarraysWithSum({-1, -2, -3}, -3) -> 2
 *
 *   提示：枚举起点 l，把「l 到 r 的累加」换成前缀和 O(1) 查——
 *         这就是讲义第 2 节说的「枚举不确定的，用公式算确定的」。
 *         数据里有负数也有 0，所以「区间和」既不单调也不递增，
 *         双指针那一套在这里不成立，老老实实两重枚举（O(n²)，n 小完全够）。
 *         先把 p 建出来（多开一格、p[0] = 0），内层只写减法，别在里层再累加。
 * ------------------------------------------------------------------------- */
long long countSubarraysWithSum(const vector<int>& a, long long k)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：前缀和的小应用——平均数最大的前缀有多长（本练习的陷阱题）
 *
 *   定义：前缀指 a[0..i]（至少含一个元素）。返回平均值最大的那个前缀的长度；
 *         并列时取最短。空表返回 0。
 *
 *   期望：
 *     longestBestAvgPrefix({3, 1, 2})     -> 1    （3 > 2 > 2）
 *     longestBestAvgPrefix({1, 2, 3, 4})  -> 4    （越往后越平均不上）
 *     longestBestAvgPrefix({5, 0, 5})     -> 1
 *     longestBestAvgPrefix({2, 2, 2})     -> 1    ← 并列取最短
 *     longestBestAvgPrefix({-5, 1})       -> 2    （-5 < -2，负数也要比对）
 *     longestBestAvgPrefix({})            -> 0
 *
 *   提示：边扫边累加得到 sum 和长度 len，要比较的是 sum1/len1 和 sum2/len2。
 *         **不许用 double 去除**——浮点比较要么用交叉相乘 sum1 * len2 > sum2 * len1，
 *         要么就承认自己在赌精度。竞赛里这种「比值比较」一律交叉相乘。
 *         交叉相乘的陷阱：sum 本身可能上 1e9，再乘一个 len 就冲破 int 上限，
 *         所以两边都必须是 long long，而且要在乘法发生之前就是。
 *         想一想：sum 用 int 存、比较时才 (long long) 转，为什么救不回来？
 * ------------------------------------------------------------------------- */
int longestBestAvgPrefix(const vector<int>& a)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* 暴力版区间加：一格一格改。它不是练习内容，是给你对拍用的参照物（讲义第 2 节）。
 * 复杂度 O(所有区间长度之和)——n、m 都上 1e5 时就是 1e10，必死。 */
vector<long long> simulateAdds(int n, const vector<RangeAdd>& ops)
{
    vector<long long> a(n + 1, 0);
    for (size_t k = 0; k < ops.size(); k++) {
        for (int i = ops[k].l; i <= ops[k].r; i++) a[i] += ops[k].v;
    }
    return a;
}

int main()
{
    cout << "========== 任务 1：一维前缀和（1 起算） ==========\n";
    assert(buildPre1({0}) == vector<long long>({0}));
    vector<long long> p = buildPre1({0, 2, 7, 1, 9});
    assert(p == vector<long long>({0, 2, 9, 10, 19}));
    assert(rangeQuery(p, 1, 1) == 2);
    assert(rangeQuery(p, 2, 3) == 8);
    assert(rangeQuery(p, 1, 4) == 19);
    assert(rangeQuery(p, 4, 4) == 9);
    assert(rangeQuery(p, 4, 3) == 0);
    assert(rangeQuery(p, 1, 0) == 0);
    vector<long long> q = buildPre1({0, 1500000000, 1500000000, -1});
    assert(q == vector<long long>({0, 1500000000LL, 3000000000LL, 2999999999LL}));
    assert(rangeQuery(q, 1, 2) == 3000000000LL);
    cout << "  buildPre1 / rangeQuery 全部通过\n";

    cout << "\n========== 任务 2：二维前缀和 ==========\n";
    vector<vector<int>> g = {{1, 2, 3, 4}, {2, 4, 6, 8}, {3, 6, 9, 12}};
    vector<vector<long long>> s = buildPre2(g);
    assert(s.size() == 4 && s[0].size() == 5);
    assert(s[0][0] == 0 && s[0][3] == 0 && s[3][0] == 0);
    assert(s[1][1] == 1 && s[1][2] == 3 && s[2][2] == 9);
    assert(s[1][4] == 10 && s[3][4] == 60);
    assert(rectSum(s, 2, 2, 3, 4) == 45);
    assert(rectSum(s, 1, 1, 3, 4) == 60);
    assert(rectSum(s, 1, 2, 3, 2) == 12);
    assert(rectSum(s, 2, 3, 2, 3) == 6);
    assert(rectSum(s, 2, 3, 1, 1) == 0);
    assert(buildPre2({{5}}) == vector<vector<long long>>({{0, 0}, {0, 5}}));
    assert(buildPre2({{}}) == vector<vector<long long>>({{0}, {0}}));
    assert(buildPre2({}) == vector<vector<long long>>({{0}}));
    cout << "  buildPre2 / rectSum 全部通过（45 与讲义一致）\n";

    cout << "\n========== 任务 3：差分区间加 ==========\n";
    vector<RangeAdd> ops = {{1, 3, 1}, {2, 6, 1}, {4, 5, 1}};
    assert(applyAdds(6, ops) == vector<long long>({0, 1, 2, 2, 2, 2, 1}));
    assert(applyAdds(1, {{1, 1, 5}}) == vector<long long>({0, 5}));
    assert(applyAdds(3, {}) == vector<long long>({0, 0, 0, 0}));
    assert(applyAdds(5, {{2, 4, 10}, {4, 5, -3}}) == vector<long long>({0, 0, 10, 10, 7, -3}));
    assert(applyAdds(2, {{1, 2, 2000000000LL}}) ==
           vector<long long>({0, 2000000000LL, 2000000000LL}));
    cout << "  applyAdds 全部通过（1 2 2 2 2 1 与讲义一致）\n";

    cout << "\n========== 任务 4：差分算最大覆盖 ==========\n";
    assert(maxCovered(10, {{1, 3}, {2, 5}, {6, 8}}) == 2);
    assert(maxCovered(10, {}) == 0);
    assert(maxCovered(5, {{1, 5}}) == 1);
    assert(maxCovered(8, {{1, 5}, {2, 6}, {3, 7}, {4, 8}}) == 4);
    assert(maxCovered(6, {{3, 2}}) == 0);
    assert(maxCovered(6, {{4, 4}}) == 1);
    cout << "  maxCovered 全部通过\n";

    cout << "\n========== 任务 5：和恰好为 k 的子数组个数 ==========\n";
    assert(countSubarraysWithSum({1, -1, 0}, 0) == 3);
    assert(countSubarraysWithSum({1, 2, 3}, 3) == 2);
    assert(countSubarraysWithSum({0, 0, 0}, 0) == 6);
    assert(countSubarraysWithSum({5}, 5) == 1);
    assert(countSubarraysWithSum({5}, 0) == 0);
    assert(countSubarraysWithSum({}, 0) == 0);
    assert(countSubarraysWithSum({-1, -2, -3}, -3) == 2);
    assert(countSubarraysWithSum({1, 2, 3}, 6) == 1);
    cout << "  countSubarraysWithSum 全部通过\n";

    cout << "\n========== 任务 6：平均数最大的前缀（long long 陷阱） ==========\n";
    assert(longestBestAvgPrefix({3, 1, 2}) == 1);
    assert(longestBestAvgPrefix({1, 2, 3, 4}) == 4);
    assert(longestBestAvgPrefix({5, 0, 5}) == 1);
    assert(longestBestAvgPrefix({2, 2, 2}) == 1);
    assert(longestBestAvgPrefix({1, 5}) == 2);
    assert(longestBestAvgPrefix({-1, -2}) == 1);
    assert(longestBestAvgPrefix({-5, 1}) == 2);
    assert(longestBestAvgPrefix({0, 0}) == 1);
    assert(longestBestAvgPrefix({}) == 0);
    cout << "  longestBestAvgPrefix 全部通过\n";

    cout << "\n========== 观察题 1：差分 vs 暴力，先对拍再算账 ==========\n";
    vector<RangeAdd> many;
    for (int k = 1; k <= 500; k++) {
        RangeAdd op;
        op.l = k % 300 + 1;
        op.r = k % 300 + 40;
        op.v = k % 7 - 3;
        many.push_back(op);
    }
    vector<long long> fast = applyAdds(1000, many);
    vector<long long> slow = simulateAdds(1000, many);
    assert(fast == slow);                       // 优化算法必须先和暴力对齐
    long long work = 0;                       // 暴力真正改了多少格，逐次加出来
    for (const RangeAdd& o : many) work += o.r - o.l + 1;
    cout << "  1000 格 × 500 次区间加：两种写法结果逐格相同。\n"
         << "  暴力真改了 " << work << " 格（每个区间长 40），差分只写 "
         << 2 * (long long)many.size() << " 个端点 + 还原 " << 1000 << " 格。\n"
         << "  把区间长度换成整段（1000 × 500 = " << 1000LL * (long long)many.size()
         << "）才够到暴力的最坏情况。再把它放大到 n = m = 1e6：1e12 对 3e6——\n"
         << "  这就是第 10 章那张表里「过不了」和「轻松过」的区别。\n";

    cout << "\n========== 观察题 2：累加爆 int，到底第几步爆 ==========\n";
    int step = 30000;
    long long limit = 2147483647LL;             // int 的上限，运行时算出来
    long long acc = 0;
    int cnt = 0;
    while (acc + step <= limit) { acc += step; cnt++; }
    cout << "  每次加 " << step << "，int 最多能连加 " << cnt
         << " 次（" << acc << "），再加一次就 " << acc + step
         << " —— 超出 int 上限。\n"
         << "  所以 1e5 个 3e4 求和、或者 1e5×1e5 的矩形和，中间值一定会跑到 3e9。\n";
    /* --- 上面那段是「用真实累加算出边界」，真把累加器写成 int 就是有符号溢出（UB）：
     * int bad = 0;
     * for (int i = 0; i < 100000; i++) bad += 30000;    // 第 71583 次起就是 UB
     * 编译器可以给你个负数，也可以假装什么都没发生。-Wall 抓不到，
     * make san/ex08 的 UBSan 一抓一个准，还会报行号。
     */
    cout << "  规则：前缀和数组、差分数组一律 long long。多花 4 字节/格，\n"
         << "  256MB 也还能放 6.7e7 格，换来的是永远不用想「这里会不会爆」。\n";

    cout << "\n>>> ex08 全部通过。下一站：ex09_binary_search.cpp\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

vector<long long> buildPre1(const vector<int>& a)
{
    int n = (int)a.size() - 1;              // a[0] 是占位
    vector<long long> p(n + 1, 0);          // p[0] = 0，多开一格换来零个边界特判
    for (int i = 1; i <= n; i++) p[i] = p[i - 1] + a[i];
    return p;
}

long long rangeQuery(const vector<long long>& p, int l, int r)
{
    if (l > r) return 0;
    return p[r] - p[l - 1];
}

vector<vector<long long>> buildPre2(const vector<vector<int>>& g)
{
    if (g.empty()) return vector<vector<long long>>(1, vector<long long>(1, 0));
    int rows = (int)g.size();
    int cols = (int)g[0].size();
    vector<vector<long long>> s(rows + 1, vector<long long>(cols + 1, 0));
    for (int i = 1; i <= rows; i++)
        for (int j = 1; j <= cols; j++)
            s[i][j] = s[i - 1][j] + s[i][j - 1] - s[i - 1][j - 1] + g[i - 1][j - 1];
    return s;
}

long long rectSum(const vector<vector<long long>>& s, int r1, int c1, int r2, int c2)
{
    if (r1 > r2 || c1 > c2) return 0;
    return s[r2][c2] - s[r1 - 1][c2] - s[r2][c1 - 1] + s[r1 - 1][c1 - 1];
}

vector<long long> applyAdds(int n, const vector<RangeAdd>& ops)
{
    vector<long long> d(n + 2, 0);          // n + 2：r == n 时还要写 d[n + 1]
    for (size_t k = 0; k < ops.size(); k++) {
        d[ops[k].l] += ops[k].v;
        d[ops[k].r + 1] -= ops[k].v;
    }
    vector<long long> out(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        d[i] += d[i - 1];
        out[i] = d[i];
    }
    return out;
}

int maxCovered(int n, const vector<pair<int, int>>& segs)
{
    vector<long long> d(n + 2, 0);
    for (size_t k = 0; k < segs.size(); k++) {
        int l = segs[k].first, r = segs[k].second;
        if (l > r) continue;                // 空区间：一层都不盖
        d[l] += 1;
        d[r + 1] -= 1;
    }
    int best = 0;
    for (int i = 1; i <= n; i++) {
        d[i] += d[i - 1];
        if (d[i] > best) best = (int)d[i];
    }
    return best;
}

long long countSubarraysWithSum(const vector<int>& a, long long k)
{
    int n = (int)a.size();
    vector<long long> p(n + 1, 0);
    for (int i = 0; i < n; i++) p[i + 1] = p[i] + a[i];
    long long cnt = 0;
    for (int l = 0; l < n; l++)
        for (int r = l; r < n; r++)
            if (p[r + 1] - p[l] == k) cnt++;
    return cnt;
}

int longestBestAvgPrefix(const vector<int>& a)
{
    int bestLen = 0;
    long long bestSum = 0, sum = 0;
    for (int i = 0; i < (int)a.size(); i++) {
        sum += a[i];
        long long len = i + 1;
        if (bestLen == 0 || sum * bestLen > bestSum * len) {
            bestLen = (int)len;
            bestSum = sum;
        }
    }
    return bestLen;
}

  为什么前缀和要多开一格、让 p[0] = 0：
    这样「前 i 个数的和」恰好是 p[i]，区间和恰好是 p[r] - p[l-1]，
    l == 1 时用到 p[0]，它天然存在且等于 0，一个 if 都不用写。
    少开一格就得为最左边单独写个分支，而每一个分支都是一潜在的 bug 入口。
    讲义说「多留一圈 0」在 DP、差分里反复出现，指的就是这个套路。

  为什么 1 起算和 0 起算不能混：
    两套公式各自内部自洽，混起来谁都救不了。0 起算时 p[i] 表示「前 i 个数」，
    区间是 p[r+1] - p[l]；1 起算时 p[i] 表示「前 i 个数」但数据也从 1 开始，
    区间是 p[r] - p[l-1]。差的这两个 +1/-1 就是 WA 与 AC 的距离。
    养成习惯：一份代码里只用一种，写完立刻用 rangeQuery(p, 1, n) == 全部和 自测。

  二维容斥那两行的几何直觉：
    s[i-1][j] 是上面那块，s[i][j-1] 是左边那块，两个相加时左上角 s[i-1][j-1]
    被算了两次，所以减掉一次，最后补上当前格子本身。
    查询同理：整块减去上边和左边两条「带」，可左上角那块被减了两次，再加回来。
    在纸上画四个矩形涂颜色，比背符号快也牢靠得多。

  为什么差分必须开 n+2：
    d[r+1] -= v 是这条链的收尾动作，r 恰好等于 n 时就要写 d[n+1]。
    数组只开到 n+1（下标 0..n）就是越界写：轻则改掉隔壁变量，重则段错误。
    最坑的是它经常「碰巧没错」——因为 n+1 那一格正好在分配到的内存里。
    多开两格是竞赛里的标准操作，别省。

  为什么边改边还原是错的：
    差分的语义是「端点差分，前缀和还原」。还原一遍得到的就是最终值，
    中途还原等于把还没收尾的操作当成已完成，后面的 +v/-v 就加到错误的基数上。
    记成一句话：先记端点，最后统一积分。

  为什么 countSubarraysWithSum 不能用双指针：
    双指针要求「窗口变大时和单调变大」，数据里有负数或 0 这条就不成立了：
    右端点往右挪，和可能反而变小，左端点不知道该不该跟进。
    此时老老实实枚举两端、用前缀和 O(1) 取值是安全的（n 小），
    n 大就要上「前缀和 + 哈希表记出现次数」——那是第 12 章容器的活。

  为什么比值比较一定要交叉相乘、而且要提前 long long：
    double 只有 53 位有效二进制，两个都接近 1e9 的数相除再比大小，
    完全可能因为舍入判成相等或判反。交叉相乘 sum1 * len2 与 sum2 * len1
    全程是整数，结论精确。但 len 可能到 1e5、sum 可能到 1e14，
    乘起来直接爆 int——必须在声明时就是 long long，而不是比较那一刻才转型，
    因为乘法一旦在 int 里做完，转回来也已经错了（和 ex01 的 area 同一个坑）。
 * ========================================================================== */
