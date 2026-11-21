/* ============================================================================
 * ex10_enum_greedy.cpp —— 枚举（暴力）· 试除 · 贪心与它的反例
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 枚举题的基本功：区间枚举 + 数位拆分 + 试除到 sqrt，一个边界都不漏
 *   2. 会写「暴力版」和「优化版」两个函数，并让它们互相验证（对拍）
 *   3. 贪心三件套：排序键、扫描、不回头；并且知道键写错会差多少
 *   4. 最重要的一条：贪心必须先找反例，找不到才敢写——本练习自带一个反例
 *
 * 编译运行：
 *   make run/ex10
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex10_enum_greedy.cpp \
 *          -o build/ex10.exe && ./build/ex10.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cassert>
using namespace std;

/* 一件货：重量 weight、价值 value。约定 weight 是正整数。 */
struct Item {
    int weight = 0;
    int value = 0;
};

/* ---------------------------------------------------------------------------
 * TODO 1：枚举 [L, R] 里「十进制表示出现过数字 0」的整数有几个
 *
 *   约定：0 <= L <= R；「0」这个数本身算含 0；L > R 时返回 0。
 *
 *   期望：
 *     countWithZeroDigit(1, 10)   -> 1     （只有 10）
 *     countWithZeroDigit(0, 10)   -> 2     （0 和 10）
 *     countWithZeroDigit(1, 99)   -> 9     （10,20,...,90）
 *     countWithZeroDigit(11, 99)  -> 8
 *     countWithZeroDigit(1, 100)  -> 10    （100 也有 0）
 *     countWithZeroDigit(105, 110)-> 6     （整段都含 0）
 *     countWithZeroDigit(99, 101) -> 2     （100、101）
 *     countWithZeroDigit(1, 1)    -> 0
 *
 *   提示：外层枚举每个数，内层用 ex01 的数位拆分（x % 10 拿一位、x / 10 去一位）。
 *         两个边界必须想清楚：x == 0 时那个 while 一次都不执行，得单独判；
 *         内层一旦找到 0 就 break，别把整位数拆完。
 *         n 到 1e9 时这个写法必死，n 到 1e6 完全能过——先写对，再谈优化。
 * ------------------------------------------------------------------------- */
int countWithZeroDigit(int L, int R)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：质因数分解 + 完全数（试除到 sqrt 的两个用法）
 *
 *   约定：primeFactors(360) -> {2, 2, 2, 3, 3, 5}，升序、含重数；n <= 1 返回空表。
 *         isPerfectNumber(n) 回答「n 的所有真因子（不含自己、含 1）之和是否等于 n」。
 *         n <= 1 一律 false。
 *
 *   期望：
 *     primeFactors(12)       -> {2, 2, 3}
 *     primeFactors(97)       -> {97}         ← 质数：因子就是它自己
 *     primeFactors(49)       -> {7, 7}
 *     primeFactors(64)       -> {2, 2, 2, 2, 2, 2}
 *     primeFactors(2000006)  -> {2, 1000003} ← 大于 sqrt 的那个大因子不能漏
 *     isPerfectNumber(6)     -> true   （1 + 2 + 3 = 6）
 *     isPerfectNumber(28)    -> true   （1+2+4+7+14）
 *     isPerfectNumber(496)   -> true
 *     isPerfectNumber(12)    -> false  （1+2+3+4+6 = 16，是个过剩数）
 *
 *   提示：分解：从 2 开始试除，能除尽就一直除（while 不是 if），除完再往上加。
 *         循环条件写 d <= n / d，不要写 d * d <= n——n 接近 int 上限时
 *         d * d 先溢出，循环提前结束，你会漏掉最大的那个因子（本练习的陷阱）。
 *         而且注意：n 在循环里是被除小的，判断条件要跟着变小的 n 走。
 *         真因子和：因子是成对出现的（d 和 n/d），所以试到 sqrt 就够，
 *         一次加两个；d == n / d（完全平方数）时只能加一次，这是最容易错的边界。
 *         1 这个因子单独起手，n 自己不算进去。
 * ------------------------------------------------------------------------- */
vector<int> primeFactors(int n)
{
    /* TODO 在这里写实现 */
    return {};
}

bool isPerfectNumber(int n)
{
    /* TODO 在这里写实现 */
    return false;
}

/* ---------------------------------------------------------------------------
 * TODO 3：枚举二元组——和为 target 的无序对有几对（两版）
 *
 *   约定：一对下标 (i, j) 要满足 i < j，每个下标最多用一次，元素可以重复取值。
 *         两版必须给出完全相同的结果；空表返回 0。
 *
 *   期望：
 *     countPairsBrute({1, 2, 3, 4, 3, 5}, 6) -> 3    （1+5, 2+4, 3+3）
 *     countPairsBrute({1, 1, 1}, 2)          -> 3    （三对里任取两个）
 *     countPairsBrute({5, 5, 5}, 10)         -> 3
 *     countPairsBrute({-1, 0, 1, 2}, 1)      -> 2
 *     countPairsBrute({}, 0)                 -> 0
 *
 *   提示：暴力版就是讲义第 2 节那两行：i 从 0，j 从 i + 1——
 *         「j = i + 1 起」这一句同时解决了「不重复」和「不自配」两件事。
 *         快版：把数组排序，然后左右各一个指针（左指最小、右指最大）。
 *         和大了就收右、小了就进左；a[l] == a[r] 时说明这一段全相等，
 *         直接用 k * (k - 1) / 2 结算，别一格一格走。
 *         返回类型为什么是 long long：n = 1e5 且全是同一个数时，
 *         对数是 1e5 × 99999 / 2 = 4999950000，int 装不下。main 里有这一组实测。
 * ------------------------------------------------------------------------- */
long long countPairsBrute(const vector<int>& a, int target)
{
    /* TODO 在这里写实现 */
    return -1;
}

long long countPairsFast(const vector<int>& a, int target)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 4：活动选择——最多能选几个互不重叠的区间
 *
 *   约定：区间用 pair<开始, 结束> 给出，「后一个的开始 >= 前一个的结束」算不重叠
 *         （端点相接允许，比如 [1,2] 和 [2,3] 可以都选）。空表返回 0。
 *
 *   期望（就是讲义 09 的那组数据，7 个区间）：
 *     seg = {{1,6},{2,3},{3,4},{5,7},{6,8},{7,9},{8,10}}
 *     maxActivities(seg) -> 4
 *     maxActivities({{1,3},{2,4},{3,5}}) -> 2
 *     maxActivities({{5,5}})             -> 1
 *     maxActivities({})                  -> 0
 *
 *   提示：讲义里那三行就是答案：按**结束时间**升序排，然后从左到右扫，
 *         「开始 >= 上一个选中的结束」就拿走它，并更新 last。
 *         为什么是结束时间不是开始时间？main 里我把错误的开始时间版也跑了一遍，
 *         同一组数据它只给出 3——差的 1 就是 WA。
 *         理由（交换论证的雏形）：结束得越早，留给后面的空间越大，
 *         所以「最早结束」这个选择永远不会比别人更差。
 *         排序参数写 const pair<int,int>&，别按值拷（第 07、10 章都提过）。
 * ------------------------------------------------------------------------- */
int maxActivities(const vector<pair<int, int>>& segs)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 5：部分背包——货可以切开装，求不超过容量的最大价值
 *
 *   约定：每件货要么整件拿走，要么按比例拿走一部分（value * 拿走的重量 / weight）。
 *         容量用完后不再装；空货表或容量为 0 时返回 0。
 *
 *   期望（三件货：(10,60) (20,100) (30,120)，即 weight 和 value）：
 *     bestLoot(items, 50) -> 240     （10 件全拿 + 20 件全拿 + 第三件拿 20/30）
 *     bestLoot(items, 15) -> 85
 *     bestLoot(items, 1)  -> 6
 *     bestLoot(items, 60) -> 280     （全装下）
 *     bestLoot(items, 100)-> 280     （装不满也别多想）
 *     bestLoot(items, 0)  -> 0
 *     bestLoot({{3, 10}}, 1) -> 10.0 / 3
 *
 *   提示：贪心键是「单位价值 value / weight」降序，而不是 value 降序，
 *         也不是 weight 升序。排序比较函数里请写交叉相乘
 *         1LL * a.value * b.weight > 1LL * b.value * a.weight，
 *         别写成 a.value / a.weight——两个 int 相除先截断了，键直接废掉。
 *         排序会改顺序，所以先拷一份 vector<Item> v = items 再排。
 *         最后一件可能装不完，剩下 (double) 的那一次除法才是真的除法。
 * ------------------------------------------------------------------------- */
double bestLoot(const vector<Item>& items, int capacity)
{
    /* TODO 在这里写实现 */
    return -1.0;
}

/* ---------------------------------------------------------------------------
 * TODO 6：找零钱张数——贪心版（本练习的陷阱题：贪心要证明）
 *
 *   约定：面额随便给、可以不含 1；每次尽量用大的那张，凑不出恰好金额就返回 -1，
 *         凑出来了返回张数。amount == 0 返回 0。
 *
 *   期望：
 *     greedyCoins({1, 2, 5}, 11)  -> 3      （5+5+1，这次它确实是最优）
 *     greedyCoins({1, 3, 4}, 6)   -> 3      （4+1+1）
 *     greedyCoins({4, 1, 3}, 6)   -> 3      ← 传进来的顺序是乱的，函数自己排
 *     greedyCoins({1, 5}, 3)      -> 3      （1+1+1）
 *     greedyCoins({1, 7, 10}, 40) -> 4
 *     greedyCoins({1, 3, 4}, 0)   -> 0
 *     greedyCoins({2, 3}, 1)      -> -1
 *
 *   提示：实现只有三行（降序排、能拿几张拿几张、看余数），难的是下面这件事：
 *         main 里我用暴力枚举算过，{1,3,4} 凑 6 的最优解是 2 张（3+3），
 *         而贪心给 3 张。**同一道题，贪心是错的。**
 *         人民币那套 {1,2,5,10,...} 贪心成立，是因为面额之间有特殊关系；
 *         「看着对」不是证明。写竞赛题时的流程是：先猜键 → 手动造反例 →
 *         造不出来再证明 → 都不行就回去写 DP（第 13 章）。
 *         所以这个函数名里带 greedy，别把它当「最少张数」来用。
 * ------------------------------------------------------------------------- */
int greedyCoins(const vector<int>& coins, int amount)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 7：最少的点，让每个区间里都至少落一个点
 *
 *   约定：点在整数位置上，x 落在 [l, r] 内表示 l <= x <= r。返回最少需要几个点。
 *
 *   期望：
 *     minPointsCover({{1,3},{2,5},{4,7},{6,9}})          -> 2  （3 和 7）
 *     minPointsCover({{1,2},{2,3},{3,4}})                -> 2
 *     minPointsCover({{5,5}})                            -> 1
 *     minPointsCover({{1,10},{2,3},{4,5},{6,7}})         -> 3
 *     minPointsCover({})                                 -> 0
 *
 *   提示：和活动选择是同一族题，键还是结束时间，但动作反过来：
 *         「当前这个点还在区间里」就跳过；不在了，就把点放到这个区间的右端点。
 *         放在右端点是关键——它尽可能多地顺手覆盖后面的区间。
 *         初始值要给一个「肯定不在任何区间里」的数，别写 0（区间可能就是 [0, 5]）。
 *         想清楚为什么这次判据是「l > 当前点」（严格大于），
 *         而任务 4 里是「l >= last」：一个是点还能不能用，一个是区间还不重叠。
 * ------------------------------------------------------------------------- */
int minPointsCover(const vector<pair<int, int>>& segs)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* 暴力枚举所有子集，算活动选择的真正最优值。不是练习内容，是对拍参照物。
 * 2^n 只在 n <= 20 时能用（讲义第 2 节、第 10 章的量级表）。 */
int maxActivitiesBrute(const vector<pair<int, int>>& segs)
{
    int n = (int)segs.size(), best = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<pair<int, int>> pick;
        for (int i = 0; i < n; i++) {
            if ((mask >> i) & 1) pick.push_back(segs[i]);
        }
        sort(pick.begin(), pick.end(),
             [](const pair<int, int>& a, const pair<int, int>& b) { return a.second < b.second; });
        bool ok = true;
        int last = -1000000000;
        for (size_t i = 0; i < pick.size(); i++) {
            if (pick[i].first < last) { ok = false; break; }
            last = pick[i].second;
        }
        if (ok && (int)pick.size() > best) best = (int)pick.size();
    }
    return best;
}

/* 常见的**错误**贪心键：按开始时间排序，能选就选。这里留着它，是为了看清差距。 */
int maxActivitiesByStart(const vector<pair<int, int>>& segs)
{
    vector<pair<int, int>> v = segs;
    sort(v.begin(), v.end(),
         [](const pair<int, int>& a, const pair<int, int>& b) { return a.first < b.first; });
    int cnt = 0, last = -1;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i].first >= last) { cnt++; last = v[i].second; }
    }
    return cnt;
}

/* 凑零钱的暴力版：枚举每种面额用几张。同样只是参照物。 */
int minCoinsDfs(const vector<int>& coins, int idx, int left)
{
    if (left == 0) return 0;
    if (idx >= (int)coins.size()) return -1;
    int best = -1;
    for (int k = 0; k * coins[idx] <= left; k++) {
        int sub = minCoinsDfs(coins, idx + 1, left - k * coins[idx]);
        if (sub >= 0 && (best < 0 || k + sub < best)) best = k + sub;
    }
    return best;
}

int minCoinsBrute(const vector<int>& coins, int amount)
{
    if (amount == 0) return 0;
    return minCoinsDfs(coins, 0, amount);
}

bool nearDouble(double a, double b)
{
    return fabs(a - b) < 1e-9;
}

int main()
{
    cout << "========== 任务 1：区间枚举 + 数位拆分 ==========\n";
    assert(countWithZeroDigit(1, 10) == 1);
    assert(countWithZeroDigit(0, 10) == 2);
    assert(countWithZeroDigit(1, 99) == 9);
    assert(countWithZeroDigit(11, 99) == 8);
    assert(countWithZeroDigit(1, 100) == 10);
    assert(countWithZeroDigit(105, 110) == 6);
    assert(countWithZeroDigit(99, 101) == 2);
    assert(countWithZeroDigit(1, 1) == 0);
    assert(countWithZeroDigit(10, 10) == 1);
    assert(countWithZeroDigit(0, 0) == 1);
    assert(countWithZeroDigit(20, 10) == 0);
    assert(countWithZeroDigit(1, 1000) == 181);
    cout << "  countWithZeroDigit 全部通过\n";

    cout << "\n========== 任务 2：试除到 sqrt ==========\n";
    assert(primeFactors(12) == vector<int>({2, 2, 3}));
    assert(primeFactors(360) == vector<int>({2, 2, 2, 3, 3, 5}));
    assert(primeFactors(97) == vector<int>({97}));
    assert(primeFactors(49) == vector<int>({7, 7}));
    assert(primeFactors(64) == vector<int>({2, 2, 2, 2, 2, 2}));
    assert(primeFactors(2) == vector<int>({2}));
    assert(primeFactors(2000006) == vector<int>({2, 1000003}));
    assert(primeFactors(1) == vector<int>{});
    assert(primeFactors(0) == vector<int>{});
    assert(isPerfectNumber(6) == true);
    assert(isPerfectNumber(28) == true);
    assert(isPerfectNumber(496) == true);
    assert(isPerfectNumber(8128) == true);
    assert(isPerfectNumber(1) == false);
    assert(isPerfectNumber(0) == false);
    assert(isPerfectNumber(2) == false);
    assert(isPerfectNumber(4) == false);
    assert(isPerfectNumber(12) == false);
    assert(isPerfectNumber(36) == false);
    assert(isPerfectNumber(-6) == false);
    cout << "  primeFactors / isPerfectNumber 全部通过\n";

    cout << "\n========== 任务 3：二元组计数，暴力版与加速版互验 ==========\n";
    assert(countPairsBrute({1, 2, 3, 4, 3, 5}, 6) == 3);
    assert(countPairsBrute({1, 1, 1}, 2) == 3);
    assert(countPairsBrute({5, 5, 5}, 10) == 3);
    assert(countPairsBrute({-1, 0, 1, 2}, 1) == 2);
    assert(countPairsBrute({}, 0) == 0);
    assert(countPairsBrute({7}, 14) == 0);
    vector<int> pool = {1, 2, 3, 4, 3, 5, -2, 0, 6, 6, 6, -2};
    for (int t = -4; t <= 12; t++) {
        assert(countPairsBrute(pool, t) == countPairsFast(pool, t));
    }
    vector<int> many(100000, 1);                // 1e5 个 1：对数会冲破 int
    assert(countPairsFast(many, 2) == 4999950000LL);
    cout << "  countPairsBrute / countPairsFast 全部通过（含 1e5 规模实测）\n";

    cout << "\n========== 任务 4：活动选择 ==========\n";
    vector<pair<int, int>> seg = {{1, 6}, {2, 3}, {3, 4}, {5, 7}, {6, 8}, {7, 9}, {8, 10}};
    assert(maxActivities(seg) == 4);
    assert(maxActivitiesBrute(seg) == 4);       // 和暴力对拍：贪心确实等于最优
    assert(maxActivitiesByStart(seg) == 3);     // 键写错就少 1 个
    assert(maxActivities({{1, 3}, {2, 4}, {3, 5}}) == 2);
    assert(maxActivitiesBrute({{1, 3}, {2, 4}, {3, 5}}) == 2);
    assert(maxActivities({{5, 5}}) == 1);
    assert(maxActivities({}) == 0);
    cout << "  maxActivities 全部通过\n";

    cout << "\n========== 任务 5：部分背包 ==========\n";
    vector<Item> goods = {{10, 60}, {20, 100}, {30, 120}};
    assert(nearDouble(bestLoot(goods, 50), 240.0));
    assert(nearDouble(bestLoot(goods, 15), 85.0));
    assert(nearDouble(bestLoot(goods, 1), 6.0));
    assert(nearDouble(bestLoot(goods, 60), 280.0));
    assert(nearDouble(bestLoot(goods, 100), 280.0));
    assert(nearDouble(bestLoot(goods, 0), 0.0));
    assert(nearDouble(bestLoot({{3, 10}}, 1), 10.0 / 3));
    assert(nearDouble(bestLoot({}, 50), 0.0));
    cout << "  bestLoot 全部通过\n";

    cout << "\n========== 任务 6：找零钱——贪心会错 ==========\n";
    assert(greedyCoins({1, 2, 5}, 11) == 3);
    assert(greedyCoins({1, 3, 4}, 6) == 3);
    assert(greedyCoins({4, 1, 3}, 6) == 3);
    assert(greedyCoins({1, 5}, 3) == 3);
    assert(greedyCoins({1, 7, 10}, 40) == 4);
    assert(greedyCoins({1, 3, 4}, 0) == 0);
    assert(greedyCoins({2, 3}, 1) == -1);
    assert(greedyCoins({2, 4}, 7) == -1);
    /* 暴力枚举给出的真最优：*/
    assert(minCoinsBrute({1, 2, 5}, 11) == 3);
    assert(minCoinsBrute({1, 3, 4}, 6) == 2);
    assert(minCoinsBrute({1, 7, 10}, 40) == 4);
    assert(minCoinsBrute({2, 3}, 1) == -1);
    cout << "  greedyCoins 全部通过\n";

    cout << "\n========== 任务 7：最少打点数 ==========\n";
    assert(minPointsCover({{1, 3}, {2, 5}, {4, 7}, {6, 9}}) == 2);
    assert(minPointsCover({{1, 2}, {2, 3}, {3, 4}}) == 2);
    assert(minPointsCover({{5, 5}}) == 1);
    assert(minPointsCover({{1, 10}, {2, 3}, {4, 5}, {6, 7}}) == 3);
    assert(minPointsCover({{1, 5}, {2, 3}, {4, 6}}) == 2);
    assert(minPointsCover({}) == 0);
    cout << "  minPointsCover 全部通过\n";

    cout << "\n========== 观察题 1：贪心的键写错，答案差多少 ==========\n";
    cout << "  活动选择同一组数据：按结束时间 " << maxActivities(seg)
         << " 个，按开始时间 " << maxActivitiesByStart(seg)
         << " 个，暴力枚举全部子集的最优 " << maxActivitiesBrute(seg) << " 个。\n";
    {
        vector<Item> wrong = goods;
        sort(wrong.begin(), wrong.end(), [](const Item& a, const Item& b) {
            return a.value > b.value;           // 错误键：单价最高的先拿
        });
        double got = 0.0;
        int left = 50;
        for (size_t i = 0; i < wrong.size() && left > 0; i++) {
            if (wrong[i].weight <= left) { left -= wrong[i].weight; got += wrong[i].value; }
            else { got += (double)wrong[i].value * left / wrong[i].weight; left = 0; }
        }
        cout << "  部分背包容量 50：按性价比拿 " << bestLoot(goods, 50)
             << "，按总价值拿 " << got << "。\n"
             << "  两个数都是「贪心」，只有一个是能证明的贪心。键要推，不能猜。\n";
    }

    cout << "\n========== 观察题 2：暴力为什么过不了（第 10 章自检第 1 题） ==========\n";
    {
        vector<int> v = {1, 3, 5, 7, 12, -4};
        long long brute = 0;
        for (int i = 0; i < (int)v.size(); i++)
            for (int j = i + 1; j < (int)v.size(); j++) brute += abs(v[j] - v[i]);
        sort(v.begin(), v.end());
        int n = (int)v.size();
        long long formula = 0;
        for (int k = 0; k < n; k++) formula += (long long)v[k] * (2 * k - n + 1);
        assert(brute == formula && brute == 100);
        cout << "  所有无序对 |a[i]-a[j]| 之和：暴力 O(n^2) 和排序公式 O(n log n)"
             << " 都算出 " << brute << "。\n"
             << "  n = 1e5 时暴力是 5e9 次（必死），公式是 1.7e6 次（眨眼）。"
             << "排序后 a[k] 被加了 k 次、被减了 n-1-k 次，公式就是这么来的。\n";
    }
    {
        int n = 100000;
        cout << "  同一个 n = " << n << "：n^2 = " << (long long)n * n
             << "，n log2 n 约 " << (long long)n * 17 << "，2^n 请自行想象。\n";
    }

    cout << "\n>>> ex10 全部通过。下一站：projects/score_report 与 notes/11 搜索。\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

int countWithZeroDigit(int L, int R)
{
    if (L > R) return 0;
    int cnt = 0;
    for (int x = L; x <= R; x++) {
        int t = x;
        if (t == 0) { cnt++; continue; }        // 0 单独判：下面那个 while 一次都不进
        while (t > 0) {
            if (t % 10 == 0) { cnt++; break; }  // 找到一位 0 就够，别拆完整个数
            t /= 10;
        }
    }
    return cnt;
}

vector<int> primeFactors(int n)
{
    vector<int> f;
    if (n <= 1) return f;
    for (int d = 2; d <= n / d; d++) {          // 写成 d * d <= n 在 n 很大时会溢出
        while (n % d == 0) {
            f.push_back(d);
            n /= d;                             // n 越除越小，判断条件跟着它缩
        }
    }
    if (n > 1) f.push_back(n);                  // 剩下这个因子一定大于 sqrt(原 n)
    return f;
}

bool isPerfectNumber(int n)
{
    if (n <= 1) return false;
    int sum = 1;                                // 1 一定是真因子，先起手
    for (int d = 2; d <= n / d; d++) {
        if (n % d == 0) {
            sum += d;
            if (d != n / d) sum += n / d;       // 完全平方数时那一格只能加一次
        }
    }
    return sum == n;
}

long long countPairsBrute(const vector<int>& a, int target)
{
    long long cnt = 0;
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)          // j 从 i+1 起：不重复、不自配
            if (a[i] + a[j] == target) cnt++;
    return cnt;
}

long long countPairsFast(const vector<int>& a, int target)
{
    vector<int> v = a;
    sort(v.begin(), v.end());
    long long cnt = 0;
    int l = 0, r = (int)v.size() - 1;
    while (l < r) {
        long long s = (long long)v[l] + v[r];
        if (s < target) l++;
        else if (s > target) r--;
        else if (v[l] == v[r]) {                // 这一段全相等：C(k, 2) 一次结清
            long long k = r - l + 1;
            cnt += k * (k - 1) / 2;
            break;
        } else {
            long long cl = 1, cr = 1;
            while (l + 1 < r && v[l + 1] == v[l]) { cl++; l++; }
            while (r - 1 > l && v[r - 1] == v[r]) { cr++; r--; }
            cnt += cl * cr;
            l++;
            r--;
        }
    }
    return cnt;
}

int maxActivities(const vector<pair<int, int>>& segs)
{
    vector<pair<int, int>> v = segs;
    sort(v.begin(), v.end(),
         [](const pair<int, int>& a, const pair<int, int>& b) { return a.second < b.second; });
    int cnt = 0, last = -1;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i].first >= last) { cnt++; last = v[i].second; }
    }
    return cnt;
}

double bestLoot(const vector<Item>& items, int capacity)
{
    vector<Item> v = items;
    sort(v.begin(), v.end(), [](const Item& a, const Item& b) {
        return 1LL * a.value * b.weight > 1LL * b.value * a.weight;   // 交叉相乘比性价比
    });
    double got = 0.0;
    int left = capacity;
    for (size_t i = 0; i < v.size() && left > 0; i++) {
        if (v[i].weight <= left) {
            left -= v[i].weight;
            got += v[i].value;
        } else {
            got += (double)v[i].value * left / v[i].weight;           // 只装得下一部分
            left = 0;
        }
    }
    return got;
}

int greedyCoins(const vector<int>& coins, int amount)
{
    if (amount == 0) return 0;
    vector<int> v = coins;
    sort(v.begin(), v.end(), greater<int>());
    int cnt = 0, left = amount;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] <= 0 || v[i] > left) continue;
        int k = left / v[i];
        cnt += k;
        left -= k * v[i];
    }
    return left == 0 ? cnt : -1;
}

int minPointsCover(const vector<pair<int, int>>& segs)
{
    vector<pair<int, int>> v = segs;
    sort(v.begin(), v.end(),
         [](const pair<int, int>& a, const pair<int, int>& b) { return a.second < b.second; });
    int cnt = 0;
    int point = -2000000000;                    // 一个肯定落在所有区间左边的值
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i].first > point) {               // 当前点管不到这个区间了
            cnt++;
            point = v[i].second;                // 就放到它的右端点，顺手多盖几个
        }
    }
    return cnt;
}

  为什么 primeFactors 的循环条件写 d <= n / d：
    试除只需要到 sqrt(n)，这一点没有疑问；疑问全在「怎么算 sqrt」。
    d * d <= n 在 n 接近 21 亿、d 到 46341 时，d * d 已经在 int 里溢出（UB），
    变成负数后条件瞬间为假，循环提前结束，那个大因子就被吞掉了。
    改成除法 n / d 永远不会溢出，代价只是一次整除。
    另一半容易漏的是结尾那句 if (n > 1)：所有小于等于 sqrt 的因子除完之后，
    剩下的 n 若大于 1，它本身必定是一个（且是唯一一个）大于 sqrt 的质因子。
    2000006 = 2 × 1000003 就是专门拿来验这一句的。

  为什么完全数那里要写 if (d != n / d)：
    因子是成对的，d 和 n/d 一起出现，所以试到 sqrt 就能把全部因子捞完。
    但当 n 是完全平方数时，sqrt 处这一对是同一个数，加两次就错了。
    36 的真因子和是 1+2+3+4+6+9+12+18 = 55，其中 6 只算一次。
    「成对枚举 + 中间那个单独处理」在求因子个数、求因子和里到处都在用。

  为什么两边夹能成立、而且要留一个暴力版在旁边：
    排序之后，和太小就只能把左指针往右挪（变大一点），和太大就只能把右指针往左挪，
    每一步都排除掉一整类不可能，所以 l、r 各走一遍就是 O(n)。
    它完全依赖「排序」这个前置动作——无序数组上双指针毫无意义，
    这正是讲义第 3 节说的「排序把无序变成有序」。
    相等那一段直接套 C(k, 2) 一次结清，既躲开「同一对下标被数两次」，
    也让一堆重复值不会把循环拖成长尾。
    至于为什么两个函数放在同一个文件里：加速版逻辑复杂、边界多，人眼看不出错；
    暴力版简单到不可能错。main 里那一排 target 让它们逐值互验，
    这就是对拍——竞赛里真正的对拍会随机生成几千组数据比输出。

  为什么活动选择要按结束时间，而不是开始时间或区间长度：
    要证的是「选结束最早的那个区间，永远不会让答案变差」。
    假设某个最优解第一个选的是 X，而结束最早的是 Y：因为 Y.end <= X.end，
    把 X 换成 Y 之后，剩下的区间依然全都从 X.end 之后开始，自然也 >= Y.end，
    所以还是合法解，个数不变。这就叫交换论证。
    按开始时间排没有这个性质：开始最早的那个往往长得要命，把后面全堵死，
    本练习那组数据它就少选一个（3 对 4）。按长度最短排同样能造出反例，自己动手找。

  为什么部分背包能贪心、找零钱不能：
    部分背包里「拿走一件货的一部分」不消耗别的方案，装不下的部分原地留着，
    所以每一步的最优选择不会影响后面任何一步——单价最高先拿就是对的。
    找零钱是「整张整张」的离散选择，多拿一张大的会挤掉若干张小的组合，
    {1,3,4} 凑 6 时贪心先拿 4 只剩 2，只能用两张 1；而 3+3 两张就够。
    所以讲义里那句「成立的前提是局部最优能推出全局最优，这不是显然的」是真话：
    先造反例，造不出来再证明，两件事一件都不能省。

  为什么 minPointsCover 把点放在右端点：
    按右端点排序后，当前区间是所有还没被覆盖的区间里结束最早的。
    要覆盖它，点只能落在 [l, r] 里；越靠右，就越可能同时落进后面的区间，
    所以放 r 是最省的选择。反过来放 l 就只能覆盖它自己，点数会膨胀。
    判据写 first > point（严格大于）：point 恰好等于 first 时，
    闭区间 [first, ...] 里已经有了这个点，不用再放新的。
 * ========================================================================== */
