/* ============================================================================
 * ex14_contest_drill.cpp —— 竞赛综合演练：读入 · 边界 · 复杂度 · 调试 · 习惯
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 把 notes/14 的「边界清单」和 notes/10 的「量级标尺」写成可以 assert 的代码
 *   2. 练五个竞赛肌肉动作：整行解析读入、最值初始化、前缀和下标约定、
 *      行尾零空格输出、多组数据重置
 *   3. 亲手用对拍抓一个反例——「想不出反例」不等于「是对的」
 *
 * 编译运行：
 *   make run/ex14
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex14_contest_drill.cpp \
 *          -o build/ex14.exe && ./build/ex14.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 *       本文件不从真 stdin 读数：所有「读入」都用 string 构造 istringstream，
 *       语义与 cin 完全一致，assert 才可复现。
 * ========================================================================== */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <climits>
#include <cassert>
using namespace std;

/* ---------------------------------------------------------------------------
 * TODO 1：整行解析——把 text 里所有整数加起来（读入优化）
 *
 *   期望：
 *     parseAllSum("1 2 3")    -> 6
 *     parseAllSum("42")       -> 42
 *     parseAllSum("  42   ")  -> 42        （前后一堆空格）
 *     parseAllSum("")         -> 0         （空输入不是崩溃，是 0）
 *     parseAllSum("1    2\n3 4") -> 10     （多个空格、换行混来都一样）
 *     parseAllSum("-1 2 -3")  -> -2        （负数照收）
 *     parseAllSum("1000000000 1000000000 1000000000") -> 3000000000  ← 爆 int 了
 *
 *   提示：istringstream in(text); long long x; while (in >> x) s += x; 三行完事。
 *         >> 自己会跳过任意空白（空格、\n、\t），所以「按空格 split 再 stoi」是
 *         自找麻烦：连续空格会切出空串、换行管不了、负数还得特判。
 *         求和用 long long——题目说 1e9 以内的数出现三次，int 就装不下了。
 * ------------------------------------------------------------------------- */
long long parseAllSum(const string& text)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：极差 spread = 最大值 - 最小值（边界条件专项）
 *
 *   期望：
 *     spread({})                 -> 0     （空表约定返回 0）
 *     spread({7})                -> 0     （单元素：自己减自己）
 *     spread({5, 5, 5})          -> 0     （全相同）
 *     spread({1, 9, 3})          -> 8
 *     spread({-5, -2})           -> 3     ← mx/mn 从 0 起步会算出 5，见陷阱 1
 *     spread({-3, -1, -2})       -> 2
 *     spread({INT_MIN, INT_MAX}) -> 4294967295   ← 差值爆 int，先减到 long long 里
 *
 *   提示：讲义 14 的铁律——最值用第一个元素初始化（mx = mn = a[0]），不要用 0。
 *         「答案超出 int」也在这题里：(long long)mx - mn，先转再做减法，
 *         和 ex01 的 1LL * w * h 是同一个道理。返回类型我帮你定成 long long 了。
 * ------------------------------------------------------------------------- */
long long spread(const vector<int>& a)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 3：复杂度估算——把 notes/10 的量级标尺写成代码
 *
 *   约定 kind：0 = O(n)，1 = O(n log n)，2 = O(n²)，3 = O(n³)，4 = O(2ⁿ·n)，5 = O(n!)
 *   estOps 返回基本操作次数（double，n 大到离谱也不炸）；n <= 0 或非法 kind 返回 0。
 *   fitsIn1s 返回「1 秒预算 1e8 次操作下能不能过」，恰好等于 1e8 算能过。
 *
 *   期望：
 *     estOps(2, 5000)  -> 2.5e7        estOps(3, 500) -> 1.25e8
 *     estOps(4, 20)    -> 20971520     estOps(5, 11)  -> 39916800（11!）
 *     fitsIn1s(0, 100000000)  -> true    fitsIn1s(0, 100000001) -> false
 *     fitsIn1s(2, 10000)      -> true（恰好 1e8）    fitsIn1s(2, 10001) -> false
 *     fitsIn1s(3, 500)        -> false   fitsIn1s(3, 400) -> true
 *     fitsIn1s(4, 20)         -> true    fitsIn1s(4, 25)  -> false
 *     fitsIn1s(5, 11)         -> true    fitsIn1s(5, 12)  -> false（12! ≈ 4.79e8）
 *
 *   提示：log2 在 <cmath>。★ 陷阱在乘法类型：n 是 long long，
 *         n * n * n 在 n = 1e6 时就已经爆 64 位了吗？（1e18，刚好没爆，但 int 早爆了）
 *         估算图省事就 (double)n 起步。讲义那句「超过 3e8 换思路、1e8 附近看常数」
 *         不用你编码，注释里记住就行。
 * ------------------------------------------------------------------------- */
double estOps(int kind, long long n)
{
    /* TODO 在这里写实现 */
    return -1;
}

bool fitsIn1s(int kind, long long n)
{
    /* TODO 在这里写实现 */
    return false;
}

/* ---------------------------------------------------------------------------
 * TODO 4：区间和 rangeSum(a, l, r) = a[l] + ... + a[r]（闭区间，调试专项）
 *
 *   期望（a = {1, 2, 3, 4, 5}）：
 *     rangeSum(a, 1, 3) -> 9      （2 + 3 + 4）
 *     rangeSum(a, 0, 4) -> 15
 *     rangeSum(a, 2, 2) -> 3      （单格区间就是它自己）
 *     rangeSum(a, 0, 0) -> 1
 *     rangeSum(a, 3, 1) -> 0      （l > r：空区间，返回 0）
 *   期望（a 是 100000 个 100000）：
 *     rangeSum(a, 0, 99999) -> 10000000000        ← 1e10，必须 long long
 *
 *   提示：main 里 rangeSumBuggy 已经给出了一份「能编译、样例像对」的错误答案，
 *         它的错在下标约定：pre 按 0-based 建（pre[i] 含 a[i]），查询却按
 *         1-based 的公式 pre[r] - pre[l] 来减，每项都漏了 a[l]。
 *         你要么把 pre 建成标准的 1-based（pre[0] = 0，pre[i] = 前 i 个元素之和，
 *         答案 = pre[r+1] - pre[l]），要么改查询公式——约定和公式必须配套。
 *         调试习惯（讲义 14 第 3 节）：对 [2,2] 这种一眼能手算的区间打印中间量，
 *         第一个和手算不一致的位置就是 bug。
 * ------------------------------------------------------------------------- */
long long rangeSum(const vector<int>& a, int l, int r)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 5：输出习惯——行内一个空格、行尾零空格
 *
 *   期望：
 *     formatRow({1, 2, 3})  -> "1 2 3"        （注意：结尾没有空格！）
 *     formatRow({})         -> ""
 *     formatRow({42})       -> "42"
 *     formatRow({-1, 0, 5}) -> "-1 0 5"
 *     formatTable({{1, 2}, {3}}) -> "1 2\n3"  （行间 \n，末尾没有 \n）
 *     formatTable({})       -> ""
 *     formatTable({{7}})    -> "7"
 *
 *   提示：「空格只出现在元素之间」的标准写法：if (i) os << ' '; 再输出 a[i]；
 *         或者攒进 ostringstream（本练习 assert 的是字符串，必须用 ostringstream 拼）。
 *         main 的观察题里有实测：行尾空格肉眼看不见，严格的 OJ 直接判 WA，
 *         用 DBG[...] 方括号夹住打印才能现形。
 * ------------------------------------------------------------------------- */
string formatRow(const vector<int>& row)
{
    /* TODO 在这里写实现 */
    return "";
}

string formatTable(const vector<vector<int>>& table)
{
    /* TODO 在这里写实现 */
    return "";
}

/* ---------------------------------------------------------------------------
 * TODO 6：多组数据模板——while (in >> n) 的语义（每组必须重置！）
 *
 *   约定：input 里连续存放若干组数据，每组先是 n（n >= 0），后面跟恰好 n 个整数；
 *         一组的答案是这 n 个整数的和。读到一半流就断了的那一组【整组丢弃】，
 *         不输出半个答案。每组答案一行，用 \n 连起来，末尾不多 \n。
 *
 *   期望：
 *     eachCaseSums("3 1 2 3 2 5 9") -> "6\n14"   两组：{1,2,3} 和 {5,9}
 *     eachCaseSums("")        -> ""
 *     eachCaseSums("1 7")     -> "7"
 *     eachCaseSums("0")       -> "0"              n=0 也是合法一组，和为 0
 *     eachCaseSums("2 1 2 0") -> "3\n0"
 *     eachCaseSums("3 1 2")   -> ""               只有两个数，凑不齐 n=3，整组丢
 *     eachCaseSums("2 1 2 3 5") -> "3"            第二组 3 个数只来了 1 个
 *     eachCaseSums("2 -3 5")  -> "2"
 *
 *   提示：结构就是讲义第 4 节的骨架：while (in >> n) { 每组在这里从头开始 }。
 *         「组内取数」用 for + if (!(in >> x)) 判失败，失败就 break 出整个 while。
 *         main 陷阱 1 演示了「ans 声明在 while 外面」的下场：第二组开始全是前缀和。
 * ------------------------------------------------------------------------- */
string eachCaseSums(const string& input)
{
    /* TODO 在这里写实现 */
    return "";
}

/* ---------------------------------------------------------------------------
 * TODO 7：对拍——固定种子造数据，O(n²) 暴力对 O(n) 的 Kadane
 *
 *   约定：第 t 轮：n = (long long)rng() % 6 + 1（1..6），
 *         每个元素 a[i] = (long long)rng() % 19 - 9（-9..9）。
 *         暴力：双重循环枚举所有子段取最大（非空子段）；
 *         高效：ex13 的 Kadane（cur = max(a[i], cur + a[i])）。
 *         返回第一个两版答案不一致的轮次编号；rounds 轮全一致返回 -1。
 *         rounds <= 0 直接返回 -1。
 *
 *   期望：
 *     stressFirstBad(20260711, 200) -> -1     ← 两版都对，200 轮拍不出反例
 *     stressFirstBad(12345, 100)    -> -1
 *     stressFirstBad(1, 50)         -> -1
 *     stressFirstBad(777, 0)        -> -1
 *
 *   提示：mt19937_64 rng((unsigned long long)seed); 引擎序列是标准规定的，
 *         同一个种子在任何编译器上产生同一串数——这正是「对拍可复现」的根基
 *         （uniform_int_distribution 的算法是实现相关的，别混着用，直接 %）。
 *         main 里 firstBadVsWrongKadane 把高效版换成「允许空子段」的错版本，
 *         同一个种子立刻在第 16 轮拍出反例。对拍是找反例，不是找 bug：
 *         拿到反例手算一遍，你就知道自己错在哪一类数据上了。
 * ------------------------------------------------------------------------- */
int stressFirstBad(long long seed, int rounds)
{
    /* TODO 在这里写实现 */
    return 0;
}

/* mx/mn 从 0 起步的经典错误版极差。只用来 assert 错误值，不是练习内容。 */
long long spreadFromZeroInit(const vector<int>& a)
{
    int mx = 0, mn = 0;                          // ★ 铁律违反：拿 0 当最值起点
    for (int x : a) { mx = std::max(mx, x); mn = std::min(mn, x); }
    return (long long)mx - mn;
}

/* 前缀和「建表 0-based、查询按 1-based」的错位版本：每一项都漏了 a[l]。 */
long long rangeSumBuggy(const vector<int>& a, int l, int r)
{
    if (a.empty() || l > r) return 0;
    vector<long long> pre(a.size(), 0);
    pre[0] = a[0];
    for (size_t i = 1; i < a.size(); i++) pre[i] = pre[i - 1] + a[i];   // pre[i] 含 a[i]
    return pre[r] - pre[l];                      // ★ 该减 pre[l-1]（l=0 时减 0），却减了 pre[l]
}

/* 每组不重置 ans 的多组读入：讲义说这是「本地对、提交错」三大原因之首。 */
string eachCaseSumsSticky(const string& input)
{
    istringstream in(input);
    string res;
    long long s = 0;                             // ★ 声明在了 while 外面，粘住了
    long long n;
    while (in >> n) {
        bool ok = true;
        for (long long i = 0; i < n; i++) {
            long long x;
            if (!(in >> x)) { ok = false; break; }
            s += x;
        }
        if (!ok) break;
        if (!res.empty()) res += "\n";
        res += to_string(s);
    }
    return res;
}

/* 行尾多一个空格的输出：肉眼看不出，OJ 看得出。 */
string rowTrailingSpaceDemo(const vector<int>& row)
{
    string res;
    for (int x : row) res += to_string(x) + " ";   // ★ 最后一个后面也跟着
    return res;
}

/* 把 TODO 7 的高效版换成「允许什么都不选」的错 Kadane，对拍立刻拍出反例。 */
int firstBadVsWrongKadane(long long seed, int rounds, vector<int>* badData = nullptr)
{
    mt19937_64 rng((unsigned long long)seed);
    for (int t = 0; t < rounds; t++) {
        int n = (int)(rng() % 6) + 1;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (int)(rng() % 19) - 9;
        int best = a[0];                          // 暴力（对的）
        for (int i = 0; i < n; i++) {
            int s = 0;
            for (int j = i; j < n; j++) { s += a[j]; best = std::max(best, s); }
        }
        int cur = 0, wrong = 0;                   // Kadane 手滑版：best = 0 起步
        for (int x : a) { cur = std::max(0, cur + x); wrong = std::max(wrong, cur); }
        if (best != wrong) {
            if (badData) *badData = a;
            return t;
        }
    }
    return -1;
}

int main()
{
    cout << "========== 任务 1：整行解析 ==========\n";
    assert(parseAllSum("1 2 3") == 6);
    assert(parseAllSum("42") == 42);
    assert(parseAllSum("  42   ") == 42);
    assert(parseAllSum("") == 0);
    assert(parseAllSum("   \n\t ") == 0);
    assert(parseAllSum("1    2\n3 4") == 10);
    assert(parseAllSum("-1 2 -3") == -2);
    assert(parseAllSum("0 0 0") == 0);
    assert(parseAllSum("1000000000 1000000000 1000000000") == 3000000000LL);
    cout << "  三个 1e9 加起来 = " << parseAllSum("1000000000 1000000000 1000000000")
         << "，int 上限 2147483647 装不下——读入累加器先定 long long。\n";
    cout << "  parseAllSum 全部通过\n";

    cout << "\n========== 任务 2：极差与边界 ==========\n";
    assert(spread({}) == 0);
    assert(spread({7}) == 0);
    assert(spread({5, 5, 5}) == 0);
    assert(spread({1, 9, 3}) == 8);
    assert(spread({-5, -2}) == 3);
    assert(spread({-3, -1, -2}) == 2);
    assert(spread({INT_MIN, INT_MAX}) == 4294967295LL);
    cout << "  spread 全部通过\n";

    cout << "\n========== 任务 3：复杂度估算 ==========\n";
    assert(estOps(0, 100000) == 100000.0);
    assert(estOps(1, 2) == 2.0);
    assert(estOps(2, 5000) == 2.5e7);
    assert(estOps(3, 500) == 1.25e8);
    assert(estOps(4, 20) == 20971520.0);
    assert(estOps(5, 11) == 39916800.0);
    assert(estOps(2, 0) == 0.0);
    assert(estOps(99, 10) == 0.0);
    assert(fitsIn1s(0, 100000000) == true);        // 恰好 1e8：算能过
    assert(fitsIn1s(0, 100000001) == false);
    assert(fitsIn1s(1, 1000000) == true);          // 1e6·log2(1e6) ≈ 2e7
    assert(fitsIn1s(1, 5000000) == false);         // ≈ 1.11e8
    assert(fitsIn1s(2, 5000) == true);
    assert(fitsIn1s(2, 10000) == true);            // 恰好 1e8
    assert(fitsIn1s(2, 10001) == false);
    assert(fitsIn1s(3, 500) == false);             // 1.25e8，n³ 的 n 上限就这么点
    assert(fitsIn1s(3, 400) == true);
    assert(fitsIn1s(4, 20) == true);
    assert(fitsIn1s(4, 25) == false);
    assert(fitsIn1s(5, 11) == true);
    assert(fitsIn1s(5, 12) == false);
    cout << "  表里那几行经验值的代码化：O(n²) 过线 n = " << 10000
         << "（恰好 1e8）、O(n³) 过线在 400 与 500 之间、2ⁿ·n 到 n = 20 还稳、n! 到 11 顶天。\n"
         << "  讲义说了：1e8 是标尺不是判决书，常数大的（map、endl、vector< vector >）要打折。\n";
    cout << "  estOps / fitsIn1s 全部通过\n";

    cout << "\n========== 任务 4：区间和（先承认错误输出，再修对） ==========\n";
    {
        vector<int> a = {1, 2, 3, 4, 5};
        /* 第一步：把「现在这个错版」的输出 assert 下来——这不是认输，是定位。
         * 手算 [1,3] = 2+3+4 = 9，错版给 7，差的正是 a[l] = 2；[2,2] 手算 3，错版给 0。
         * 第一个和手算不一致的位置就是 bug 本体（讲义 14 第 3 节）。 */
        assert(rangeSumBuggy(a, 1, 3) == 7);       // 错，应为 9
        assert(rangeSumBuggy(a, 0, 4) == 14);      // 错，应为 15
        assert(rangeSumBuggy(a, 2, 2) == 0);       // 错，应为 3
        assert(rangeSumBuggy(a, 0, 0) == 0);       // 错，应为 1
        /* 第二步：你的实现必须把这些都对回来。 */
        assert(rangeSum(a, 1, 3) == 9);
        assert(rangeSum(a, 0, 4) == 15);
        assert(rangeSum(a, 2, 2) == 3);
        assert(rangeSum(a, 0, 0) == 1);
        assert(rangeSum(a, 3, 1) == 0);
        vector<int> big(100000, 100000);
        assert(rangeSum(big, 0, 99999) == 10000000000LL);
        assert(rangeSum(big, 50000, 50000) == 100000);
        cout << "  错版 [1,3] = " << rangeSumBuggy(a, 1, 3)
             << "，正确 = " << rangeSum(a, 1, 3) << "，差的恰好是 a[l] = " << a[1] << "。\n"
             << "  pre 建表的约定（含不含 a[i]）决定查询公式，二者必须配套——\n"
             << "  数组开 n+1、pre[0] = 0，全程一种下标制，这类错位就无从发生。\n";
    }
    cout << "  rangeSum 全部通过\n";

    cout << "\n========== 任务 5：输出零多余空格 ==========\n";
    assert(formatRow({1, 2, 3}) == "1 2 3");
    assert(formatRow({}) == "");
    assert(formatRow({42}) == "42");
    assert(formatRow({-1, 0, 5}) == "-1 0 5");
    assert(formatTable({{1, 2}, {3}}) == "1 2\n3");
    assert(formatTable({}) == "");
    assert(formatTable({{7}}) == "7");
    assert(formatTable({{1}, {2}}) == "1\n2");
    cout << "  formatRow / formatTable 全部通过\n";

    cout << "\n========== 任务 6：多组数据 while (in >> n) ==========\n";
    assert(eachCaseSums("3 1 2 3 2 5 9") == "6\n14");
    assert(eachCaseSums("") == "");
    assert(eachCaseSums("1 7") == "7");
    assert(eachCaseSums("0") == "0");
    assert(eachCaseSums("2 1 2 0") == "3\n0");
    assert(eachCaseSums("3 1 2") == "");
    assert(eachCaseSums("2 1 2 3 5") == "3");
    assert(eachCaseSums("2 -3 5") == "2");
    cout << "  eachCaseSums 全部通过\n";

    cout << "\n========== 任务 7：固定种子对拍 ==========\n";
    assert(stressFirstBad(20260711, 200) == -1);
    assert(stressFirstBad(12345, 100) == -1);
    assert(stressFirstBad(1, 50) == -1);
    assert(stressFirstBad(777, 0) == -1);
    cout << "  4 个种子共 350 轮，暴力与 Kadane 全部一致（firstBad = -1）。\n";
    cout << "  stressFirstBad 全部通过\n";

    cout << "\n========== 陷阱 1：多组数据不重置 ==========\n";
    {
        string good = eachCaseSums("3 1 2 3 2 5 9");
        string sticky = eachCaseSumsSticky("3 1 2 3 2 5 9");
        assert(good == "6\n14");
        assert(sticky == "6\n20");                // 第二组把第一组的和背在身上了
        assert(sticky != good);
        cout << "  对的：\"6\\n14\"，不重置的：\"6\\n20\"（第二组的 14 被顶成了 20）。\n"
             << "  第一组居然是对的！所以「样例过了」——第二组开始才崩。\n"
             << "  本地对、提交错的三大原因之首就是它。ans 声明在 while 里面，天塌不下来。\n";
    }

    cout << "\n========== 陷阱 2：最值从 0 起步 ==========\n";
    {
        assert(spreadFromZeroInit({-5, -2}) == 5);        // 错，应为 3
        assert(spreadFromZeroInit({-3, -1, -2}) == 3);    // 错，应为 2
        assert(spreadFromZeroInit({1, 9, 3}) == 9);       // 错，应为 8：mn 也被 0 劫持了
        assert(spread({-5, -2}) == 3);
        assert(spread({-3, -1, -2}) == 2);
        assert(spread({1, 9, 3}) == 8);
        cout << "  0 起步的极差：全负数组白送一个不存在的 0；全正数组又假装有最小值 0。\n"
             << "  两头都错——mx = mn = a[0] 一行解决，别跟 0 较劲。\n";
    }

    cout << "\n========== 陷阱 3：对拍拍出手滑版 Kadane ==========\n";
    {
        vector<int> bad;
        int t = firstBadVsWrongKadane(20260711, 200, &bad);
        assert(t == 16);                         // 实测值：同一颗种子永远在第 16 轮翻车
        cout << "  高效版写成 cur = max(0, cur + x)（允许空子段）后，第 " << t << " 轮就不一致，\n"
             << "  反例 a = {";
        for (size_t i = 0; i < bad.size(); i++) cout << (i ? ", " : "") << bad[i];
        cout << "}：暴力给 -2，错版给 0——全是负数时它「什么都不偷」了。\n"
             << "  这就是对拍的价值：不用你想到「全负」这种数据，随机数替你想到。\n"
             << "  种子 20260711 + mt19937_64，任何编译器上都是第 16 轮——可复现才能调试。\n";
    }

    cout << "\n========== 观察题：行尾空格与读入加速 ==========\n";
    {
        string messy = rowTrailingSpaceDemo({1, 2, 3});
        string clean = formatRow({1, 2, 3});
        assert(messy == "1 2 3 ");                // 末尾多一个空格，屏幕上看一模一样
        assert(clean == "1 2 3");
        assert(messy != clean);
        cout << "  方括号夹住才现形：DBG[" << messy << "]\n";
        cout << "  正确写法：      DBG[" << clean << "]\n";
        cout << "  严格的 OJ 按字节比输出，这个空格就是 WA；宽容的 OJ 按 token 比，\n"
             << "  但你不能赌它宽容——if (i) cout << ' '; 的模式写一百次成习惯。\n";
    }
    /* --- 交题模板的读入加速，本文件用不上（不从真 stdin 读），背下来 ---
     * ios::sync_with_stdio(false);   // 断开 cin/cout 与 C stdio 的同步，快 3~5 倍
     * cin.tie(nullptr);              // cin 之前不再强制 flush cout
     * 代价：关了同步之后，cin/cout 与 scanf/printf 不能再混用；
     *       '\n' 代替 endl（endl 每次 flush，1e6 行能差出几百 ms）。
     * 讲义 10 第 6 节实测：不加速的 cin 读 1e6 个数慢 3~5 倍。
     */
    cout << "  多组数据防御性读入：int n; if (!(cin >> n)) return 0; ——空输入直接收工，\n"
         << "  不然 n 是垃圾值，后面全乱。任务 6 的 while (in >> n) 就是它的循环版。\n";

    cout << "\n>>> ex14 全部通过。练习到此收官：去 projects/ 打 p1、p2，提交前把第 5 节清单过一遍。\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

long long parseAllSum(const string& text)
{
    istringstream in(text);
    long long s = 0, x;
    while (in >> x) s += x;                       // >> 自动跳过任意空白
    return s;
}

long long spread(const vector<int>& a)
{
    if (a.empty()) return 0;
    int mx = a[0], mn = a[0];                      // ★ 用第一个元素初始化
    for (int x : a) { mx = std::max(mx, x); mn = std::min(mn, x); }
    return (long long)mx - mn;                     // 转了再减，INT_MAX - INT_MIN 才不爆
}

double estOps(int kind, long long n)
{
    if (n <= 0) return 0.0;
    double dn = (double)n;
    switch (kind) {
        case 0: return dn;
        case 1: return dn * log2(dn);
        case 2: return dn * dn;
        case 3: return dn * dn * dn;
        case 4: return pow(2.0, dn) * dn;
        case 5: { double f = 1.0; for (long long i = 2; i <= n; i++) f *= (double)i; return f; }
        default: return 0.0;
    }
}

bool fitsIn1s(int kind, long long n)
{
    return estOps(kind, n) <= 1e8;                 // 恰好 1e8 算过
}

long long rangeSum(const vector<int>& a, int l, int r)
{
    if (l > r || a.empty()) return 0;
    int n = (int)a.size();
    if (l < 0) l = 0;
    if (r >= n) r = n - 1;
    vector<long long> pre(n + 1, 0);               // ★ 1-based：pre[i] = 前 i 个之和
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + a[i];
    return pre[r + 1] - pre[l];                    // 与约定配套的公式
}

string formatRow(const vector<int>& row)
{
    ostringstream os;
    for (size_t i = 0; i < row.size(); i++) {
        if (i) os << ' ';                          // ★ 空格只在元素之间
        os << row[i];
    }
    return os.str();
}

string formatTable(const vector<vector<int>>& table)
{
    ostringstream os;
    for (size_t i = 0; i < table.size(); i++) {
        if (i) os << '\n';
        os << formatRow(table[i]);                 // 行内规则复用，末尾不多 \n
    }
    return os.str();
}

string eachCaseSums(const string& input)
{
    istringstream in(input);
    string res;
    long long n;
    while (in >> n) {
        long long s = 0;                           // ★ 每组的累加器死在组内
        bool ok = true;
        for (long long i = 0; i < n; i++) {
            long long x;
            if (!(in >> x)) { ok = false; break; }
            s += x;
        }
        if (!ok) break;                            // 残缺组整组丢弃
        if (!res.empty()) res += "\n";
        res += to_string(s);
    }
    return res;
}

int stressFirstBad(long long seed, int rounds)
{
    mt19937_64 rng((unsigned long long)seed);      // 固定种子 = 可复现
    for (int t = 0; t < rounds; t++) {
        int n = (int)(rng() % 6) + 1;
        vector<int> a(n);
        for (int i = 0; i < n; i++) a[i] = (int)(rng() % 19) - 9;
        int best = a[0];                           // 暴力 O(n²)：对拍里的「标准答案」
        for (int i = 0; i < n; i++) {
            int s = 0;
            for (int j = i; j < n; j++) { s += a[j]; best = std::max(best, s); }
        }
        int cur = a[0], fast = a[0];               // Kadane O(n)
        for (int i = 1; i < n; i++) {
            cur = std::max(a[i], cur + a[i]);
            fast = std::max(fast, cur);
        }
        if (best != fast) return t;                // 抓到反例，报轮次
    }
    return -1;
}

  为什么用 >> 而不是「按空格 split 再 stoi」：
  整数流 >> 把「跳过任意空白、识别负号、失败置 eof」全打包好了，
  连续空格、制表符、跨行数字都不在话下；手工 split 遇到 "1  2" 会切出空串，
  stoi("") 当场抛异常，而 OJ 的数据里 "1  2" 太常见了。竞赛里读入的一切姿势
  最终都会退化成 while (cin >> x)，早一天习惯早一天受益。

  为什么极差返回 long long 且 (long long)mx - mn：
  int 的极差最大是 INT_MAX - INT_MIN = 4294967295，本身就装不回 int——不是「可能」
  溢出，是「必然」。ex01 里 1LL * w * h 的同款教训：转换要在运算之前，
  (long long)(mx - mn) 就晚了，括号里已经溢出了。

  为什么 rangeSum 用 pre[i] = 前 i 个之和（pre[0] = 0）：
  这个约定下 pre 的下标 i 表示「前 i 个」，区间 [l, r] 是「前 r+1 个」减「前 l 个」，
  即 pre[r+1] - pre[l]，l = 0 时减 pre[0] = 0，不需要任何特判。错版 pre[i] 含 a[i]
  （「前 i+1 个」），查询公式却照抄了另一种约定，于是每项都少一个 a[l]。
  前缀和的 bug 九成不是加法错，是约定串了位——建表时把 pre[i] 的含义用一句中文
  写在旁边，比事后拿十组数据瞎试快得多。

  为什么「恰好等于 1e8 算过」也要写进代码：
  讲义的经验值（O(n²) 到 3000~5000）本来就是「n 代进去、和 1e8 比」的口算，
  写成 fitsIn1s 是为了让口算变成 assert：边界数据（10000²）恰好压线，这时
  决定生死的是常数——map、endl、缓存不友好都能把 1e8 变成 3e8。标尺给你方向，
  不给你保证。

  为什么对拍用 rng() % k 而不是 uniform_int_distribution：
  mt19937_64 的输出序列和 % 的算术都是标准钉死的，同一颗种子在任何编译器、
  任何机器上给出同一串数据；uniform_int_distribution 的具体算法是实现自定义的，
  libstdc++ 和 MSVC 可能给出不同的数。对拍的命根子是「反例可复现」，
  所以只信引擎取模这点 bias 换确定性，在生成器场景稳赚。
  main 里 16 轮那个反例（全负数组）就是随机数替你想到、而你多半想不到的边界。
 * ========================================================================== */
