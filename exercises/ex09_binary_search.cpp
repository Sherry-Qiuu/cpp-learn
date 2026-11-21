/* ============================================================================
 * ex09_binary_search.cpp —— 手写二分 · lower_bound 语义 · 二分答案
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 二分的两种模板（找第一个满足 / 找最后一个满足）分开背死，永不混搭
 *   2. 手写出的 lowerIdx 必须和 STL 的 lower_bound 逐值对拍一致
 *   3. 会把「求最小/最大可行值」翻译成一个单调的 check 函数（make it, check it）
 *   4. 知道 mid = (l + r) / 2 什么时候咬人，以及死循环是哪一个符号造成的
 *
 * 编译运行：
 *   make run/ex09
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex09_binary_search.cpp \
 *          -o build/ex09.exe && ./build/ex09.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

/* ---------------------------------------------------------------------------
 * TODO 1：最朴素的二分查找——找到就返回下标，找不到返回 -1
 *
 *   约定：a 已经升序（有重复时返回其中任意一个匹配下标）。空表返回 -1。
 *
 *   期望：
 *     indexOf({}, 5)                            -> -1
 *     indexOf({4}, 4)                           -> 0
 *     indexOf({1, 3, 5, 7, 9, 11}, 1)           -> 0
 *     indexOf({1, 3, 5, 7, 9, 11}, 11)          -> 5
 *     indexOf({1, 3, 5, 7, 9, 11}, 6)           -> -1   ← 卡在两个数中间
 *     indexOf({1, 3, 5, 5, 6, 8, 9}, 5)         -> 2 或 3（只要求 a[返回值] == 5）
 *
 *   提示：这一版用「闭区间 [l, r] + while (l <= r)」：
 *           l = 0, r = size()-1；mid 处小了就把 l 抬到 mid+1，大了就把 r 压到 mid-1。
 *         条件是 l <= r 而不是 l < r——写成 l < r 时区间只剩一个元素就直接退出，
 *         那一个元素永远没被看过，最后一个数据点漏检。
 *         r 用 int 并减一次 (int)a.size() - 1，理由见 ex05 的无符号陷阱。
 * ------------------------------------------------------------------------- */
int indexOf(const vector<int>& a, int x)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：lower_bound / upper_bound 语义——返回「第一个 >= x / 第一个 > x」的下标
 *
 *   约定：a 升序可重复。全都 < x 时返回 (int)a.size()（即「插到最右边」）。
 *
 *   期望（a = {1, 3, 5, 5, 6, 8, 9}，就是讲义 09 那串）：
 *     lowerIdx(a, 5) -> 2      upperIdx(a, 5) -> 4      ← 讲义实测 2 / 4
 *     upperIdx(a, 5) - lowerIdx(a, 5) == 2              ← 5 出现两次
 *     lowerIdx(a, 7) -> 5      upperIdx(a, 7) -> 5      ← 7 不存在，两个指针重合
 *     lowerIdx(a, 0) -> 0      lowerIdx(a, 10) -> 7
 *     lowerIdx({}, 5) -> 0
 *
 *   提示：这一版用「左闭右开区间 [l, r) + while (l < r)」：
 *           l = 0, r = size()；满足就把 r 收到 mid，不满足就把 l 抬到 mid+1。
 *         出来时 l == r，就是答案。注意 r 初值是 size()，
 *         所以循环里 a[mid] 一定是合法下标（mid < r <= size()）。
 *         别和任务 1 的写法混着用，两套的区间含义不一样。
 *         main 里会和 STL 逐值对拍，写错了当场就能看见。
 * ------------------------------------------------------------------------- */
int lowerIdx(const vector<int>& a, int x)
{
    /* TODO 在这里写实现 */
    return -1;
}

int upperIdx(const vector<int>& a, int x)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 3：二分答案——运货：days 天送完，船的最小载重量
 *
 *   约定：w[i] 是第 i 个包裹的重量，必须按给出的顺序装船（不能重排），
 *         一天的运力就是一艘船装的那一批，总和不能超过 capacity。
 *         返回「能在 days 天（含）内送完」的最小 capacity。空表返回 0。
 *
 *   期望：
 *     minCapacity({1,2,3,4,5,6,7,8,9,10}, 5)  -> 15
 *     minCapacity({1,2,3,4,5,6,7,8,9,10}, 1)  -> 55   ← 一趟拉完 = 全部相加
 *     minCapacity({1,2,3,4,5,6,7,8,9,10}, 10) -> 10   ← 一天一件 = 最重的那件
 *     minCapacity({3,2,2,4,1,4}, 3)           -> 6
 *     minCapacity({1,2,3,1,1}, 4)             -> 3
 *     minCapacity({2,3,1,5,10,2,5}, 4)        -> 10
 *     minCapacity({}, 3)                      -> 0
 *
 *   提示：这就是「make it, check it」：答案不好求，但「给定运力 c 能不能送完」
 *         是一个 O(n) 的贪心判定——照着顺序装，装不下就开新的一天。
 *         判定函数对 c 是单调的：c 越大越容易成功。求「最小可行值」= 找
 *         第一个满足条件的位置 → 用任务 2 那套 l < r、mid 下取整、满足时 hi = mid。
 *         下界取「最重的那一件」（再小就装不下了），上界取「全部相加」（一天拉完）。
 *         check 的方向一旦写反，你会稳稳地返回上界 55，而且样例可能照样对——
 *         所以断言里我放了 days == 1 和 days == n 两个极端。
 * ------------------------------------------------------------------------- */
long long minCapacity(const vector<int>& w, int days)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 4：二分答案——floor(sqrt(x))，x 最大 1e18
 *
 *   期望：
 *     intSqrt(0)                   -> 0
 *     intSqrt(1)                   -> 1
 *     intSqrt(2)                   -> 1
 *     intSqrt(15)                  -> 3
 *     intSqrt(16)                  -> 4
 *     intSqrt(99)                  -> 9
 *     intSqrt(1000000000000LL)     -> 1000000     ← 讲义 09 实测的那个数
 *     intSqrt(999999999999LL)      -> 999999
 *     intSqrt(2147395600LL)        -> 46340       ← 46340^2 刚好贴边
 *
 *   提示：要求的是「最后一个满足 mid*mid <= x 的 mid」→ 用另一个模板：
 *         mid 上取整 (lo + hi + 1) / 2，满足就 lo = mid，不满足就 hi = mid - 1。
 *         两个坑：
 *         ① 别写 mid * mid <= x：mid 到 2e9 量级时乘积到 4e18，再大就爆 long long。
 *            改写成 mid <= x / mid，用除法把量级压下来，这是整数平方根的标准写法。
 *         ② 上界取 2000000000LL 就够 1e18 用了（2e9 的平方是 4e18）。
 *            别把 lo 也设成 1：x == 0 时你要能返回 0。
 *            lo 从 0 起会让 mid 永远是整数除法下取整的那个方向，写错就死循环。
 * ------------------------------------------------------------------------- */
long long intSqrt(long long x)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 5：旋转有序数组里找最小值（无重复元素）
 *
 *   定义：{1,2,3,4,5} 向右循环旋若干次得到的新数组，例如 {3,4,5,1,2}。
 *
 *   期望：
 *     minInRotated({3, 4, 5, 1, 2})    -> 1
 *     minInRotated({1, 2, 3, 4, 5})    -> 1   ← 转 0 次，本来就有序
 *     minInRotated({5, 1, 2, 3, 4})    -> 1   ← 最小值贴着第二个格子
 *     minInRotated({4, 5, 6, 1, 2, 3}) -> 1
 *     minInRotated({7})                -> 7
 *     minInRotated({2, 1})             -> 1
 *     minInRotated({1, 2})             -> 1
 *     minInRotated({})                 -> 0   ← 约定空表给 0，先判空再动手
 *
 *   提示：数组被切成两段各自递增，最小值就是第二段的开头。
 *         拿 a[mid] 和**右端点** a[r] 比：a[mid] > a[r] 说明拐点在右边，l = mid + 1；
 *         否则 mid 就是候选，r = mid（不能 r = mid - 1，mid 自己可能正是答案）。
 *         用 l < r 这一套，退出时 l == r 指向最小值。
 *         为什么要和右端点比而不是和左端点比？自己举一个 5 个数的例子推一遍。
 *         顺带记住：有重复元素时（比如 {1,1,1,0,1}）这一招会失效，那是另一道题。
 * ------------------------------------------------------------------------- */
int minInRotated(const vector<int>& a)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：返回「最后一个 <= x」的下标，一个都没有返回 -1（本练习的陷阱题）
 *
 *   期望（a = {1, 3, 5, 5, 6, 8, 9}）：
 *     lastNotGreaterThan(a, 5)  -> 3      ← 重复的最后一个
 *     lastNotGreaterThan(a, 4)  -> 1
 *     lastNotGreaterThan(a, 2)  -> 0
 *     lastNotGreaterThan(a, 0)  -> -1     ← 全都比 x 大
 *     lastNotGreaterThan(a, 10) -> 6      ← 全都是答案候选，取最右
 *     lastNotGreaterThan({}, 5) -> -1
 *
 *   提示：这是「最后一个满足条件」的模板：mid 上取整，满足 lo = mid，不满足 hi = mid-1。
 *         区间怎么开随你，但请把这三条对上号，差一个符号就是死循环或者越界：
 *           ① 只要有 lo = mid 这条赋值，mid 就必须上取整（(lo + hi + 1) / 2）。
 *              否则 lo、hi 相邻时 mid == lo，赋值等于原地不动，循环永不结束。
 *           ② 只要有 hi = mid 这条赋值，mid 就必须下取整，同理。
 *           ③ 反过来的两种混搭（下取整 + lo = mid，或上取整 + hi = mid）都会死循环，
 *              而且死循环的表现是「程序卡住不输出」，不是崩溃——最难查的那种。
 *         还有 l + (r - l) / 2 这个写法：和 (l + r) / 2 在数组下标上等价，
 *         但二分答案时 lo、hi 可以是 2e9 量级，l + r 直接冲破 int。见文末观察题。
 * ------------------------------------------------------------------------- */
int lastNotGreaterThan(const vector<int>& a, int x)
{
    /* TODO 在这里写实现 */
    return -1;
}

int main()
{
    cout << "========== 任务 1：二分查找返回下标 ==========\n";
    vector<int> uniq = {1, 3, 5, 7, 9, 11};
    assert(indexOf(uniq, 1) == 0);
    assert(indexOf(uniq, 11) == 5);
    assert(indexOf(uniq, 7) == 3);
    assert(indexOf(uniq, 6) == -1);
    assert(indexOf(uniq, 0) == -1);
    assert(indexOf(uniq, 12) == -1);
    assert(indexOf({4}, 4) == 0);
    assert(indexOf({4}, 3) == -1);
    assert(indexOf({}, 5) == -1);
    vector<int> dup = {1, 3, 5, 5, 6, 8, 9};
    int pos = indexOf(dup, 5);
    assert(pos == 2 || pos == 3);
    assert(dup[pos] == 5);                     // 有重复时只保证「指到一个 5」
    cout << "  indexOf 全部通过\n";

    cout << "\n========== 任务 2：lowerIdx / upperIdx + 与 STL 对拍 ==========\n";
    assert(lowerIdx(dup, 5) == 2);
    assert(upperIdx(dup, 5) == 4);
    assert(upperIdx(dup, 5) - lowerIdx(dup, 5) == 2);
    assert(lowerIdx(dup, 7) == 5);
    assert(upperIdx(dup, 7) == 5);             // 不存在时两个指针重合
    assert(upperIdx(dup, 7) - lowerIdx(dup, 7) == 0);
    assert(lowerIdx(dup, 0) == 0);
    assert(lowerIdx(dup, 10) == 7);
    assert(upperIdx(dup, 10) == 7);
    assert(lowerIdx(dup, 1) == 0);
    assert(upperIdx(dup, 9) == 7);
    assert(lowerIdx({}, 5) == 0);
    assert(upperIdx({}, 5) == 0);
    assert(binary_search(dup.begin(), dup.end(), 7) == false);
    assert(binary_search(dup.begin(), dup.end(), 8) == true);
    /* 随机数据对拍：把手写版和标准库一个一个值比过去。这才是「写对了」的证据。 */
    unsigned int seed = 20241109u;
    vector<int> rnd;
    for (int i = 0; i < 200; i++) {
        seed = seed * 1664525u + 1013904223u;  // 无符号回绕，行为完全确定
        rnd.push_back((int)(seed % 50u));
    }
    sort(rnd.begin(), rnd.end());
    int diff = 0;
    for (int x = -1; x <= 51; x++) {
        if (lowerIdx(rnd, x) != (int)(lower_bound(rnd.begin(), rnd.end(), x) - rnd.begin())) diff++;
        if (upperIdx(rnd, x) != (int)(upper_bound(rnd.begin(), rnd.end(), x) - rnd.begin())) diff++;
    }
    assert(diff == 0);
    cout << "  lowerIdx / upperIdx 全部通过，另与 STL 对拍 106 组全一致\n";

    cout << "\n========== 任务 3：二分答案——最小运力 ==========\n";
    assert(minCapacity({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5) == 15);
    assert(minCapacity({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1) == 55);
    assert(minCapacity({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 10) == 10);
    assert(minCapacity({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 20) == 10);
    assert(minCapacity({3, 2, 2, 4, 1, 4}, 3) == 6);
    assert(minCapacity({1, 2, 3, 1, 1}, 4) == 3);
    assert(minCapacity({2, 3, 1, 5, 10, 2, 5}, 4) == 10);
    assert(minCapacity({}, 3) == 0);
    cout << "  minCapacity 全部通过\n";

    cout << "\n========== 任务 4：整数平方根 ==========\n";
    assert(intSqrt(0) == 0);
    assert(intSqrt(1) == 1);
    assert(intSqrt(2) == 1);
    assert(intSqrt(15) == 3);
    assert(intSqrt(16) == 4);
    assert(intSqrt(99) == 9);
    assert(intSqrt(1000000000000LL) == 1000000);
    assert(intSqrt(999999999999LL) == 999999);
    assert(intSqrt(2147395600LL) == 46340);
    assert(intSqrt(2147395599LL) == 46339);
    assert(intSqrt(999999999999999999LL) == 999999999);
    cout << "  intSqrt 全部通过\n";

    cout << "\n========== 任务 5：旋转数组找最小值 ==========\n";
    assert(minInRotated({3, 4, 5, 1, 2}) == 1);
    assert(minInRotated({1, 2, 3, 4, 5}) == 1);
    assert(minInRotated({5, 1, 2, 3, 4}) == 1);
    assert(minInRotated({4, 5, 6, 1, 2, 3}) == 1);
    assert(minInRotated({7}) == 7);
    assert(minInRotated({2, 1}) == 1);
    assert(minInRotated({1, 2}) == 1);
    assert(minInRotated({2, 3, 1}) == 1);
    assert(minInRotated({}) == 0);
    cout << "  minInRotated 全部通过\n";

    cout << "\n========== 任务 6：最后一个 <= x（模板别混搭） ==========\n";
    assert(lastNotGreaterThan(dup, 5) == 3);
    assert(lastNotGreaterThan(dup, 4) == 1);
    assert(lastNotGreaterThan(dup, 2) == 0);
    assert(lastNotGreaterThan(dup, 0) == -1);
    assert(lastNotGreaterThan(dup, 10) == 6);
    assert(lastNotGreaterThan(dup, 1) == 0);
    assert(lastNotGreaterThan(dup, 6) == 4);
    assert(lastNotGreaterThan({}, 5) == -1);
    /* 三个函数说的是同一件事，随便哪两个都能互相验： */
    for (int x = -1; x <= 10; x++) {
        int lg = lastNotGreaterThan(dup, x);
        assert(lg == lowerIdx(dup, x + 1) - 1);
        assert(lg == upperIdx(dup, x) - 1);
    }
    cout << "  lastNotGreaterThan 全部通过，且与 lower/upper 三种写法互证\n";

    cout << "\n========== 观察题 1：mid = (l + r) / 2 什么时候咬人 ==========\n";
    long long lo = 1500000000LL, hi = 2000000000LL;
    cout << "  lo = " << lo << ", hi = " << hi << "\n"
         << "  lo + hi = " << lo + hi << "，而 int 的上限是 " << 2147483647LL << "。\n"
         << "  安全写法算出来是 " << lo + (hi - lo) / 2
         << "（hi - lo 只有 " << hi - lo << "，怎么加都不会爆）。\n";
    /* --- 如果 lo、hi 是 int，上面那行 lo + hi 就是有符号溢出（UB），所以故意不写出来。
     * 关键是：二分**下标**时 l + r 顶多 2 × 2e9，看着像爆不了（数组开不到 2e9 个 int，
     * 内存先没），所以小项目里 (l + r) / 2 蒙对了也没事；
     * 但二分**答案**时 lo、hi 是「重量、价格、边长、时间」这类值，随随便便就是 1e9、1e18，
     * 这时候 l + r 一定爆。既然两种场景共用同一个模板，就一律写 lo + (hi - lo) / 2。
     */

    cout << "\n========== 观察题 2：二分到底多快 + 两种模板殊途同归 ==========\n";
    int steps = 0;
    for (int m = 1000000; m > 0; m /= 2) steps++;
    cout << "  1000000 个候选，每比一次砍掉一半，" << steps
         << " 次就只剩一个。线性扫要 1000000 次。\n"
         << "  这就是第 10 章表里 O(log n) 那一行写「n 上限：任意」的意思。\n";
    /* 同一个平方根，也可以问「第一个平方超过 x 的数是几」，答案减一就是 floor(sqrt(x))。
     * 换问题就要换模板：这里求「第一个满足」，所以 mid 下取整、满足时 hi = mid。
     * lo 从 1 起，因为这里把 0 当成不可能成立的边界。 */
    auto firstTooBig = [](long long x) {
        long long l = 1, r = 2000000000LL;
        while (l < r) {
            long long mid = l + (r - l) / 2;
            if (mid > x / mid) r = mid;        // mid 已经「太大」，它自己是候选
            else l = mid + 1;                  // mid 还不够大，排除掉
        }
        return l;
    };
    long long probe[6] = {0, 1, 15, 16, 99, 1000000000000LL};
    for (int i = 0; i < 6; i++) {
        assert(firstTooBig(probe[i]) - 1 == intSqrt(probe[i]));
    }
    cout << "  「最后一个满足」和「第一个不满足」两条路，答案差 1，殊途同归：\n"
         << "  只要取整方向和赋值方向配对着，两种都能写对；混着用就死循环。\n";

    cout << "\n>>> ex09 全部通过。下一站：ex10_enum_greedy.cpp\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

int indexOf(const vector<int>& a, int x)
{
    int l = 0, r = (int)a.size() - 1;          // 闭区间 [l, r]
    while (l <= r) {                           // 一个元素时也要进去比
        int mid = l + (r - l) / 2;
        if (a[mid] == x) return mid;
        if (a[mid] < x) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}

int lowerIdx(const vector<int>& a, int x)
{
    int l = 0, r = (int)a.size();              // 左闭右开 [l, r)，答案可以是 size()
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (a[mid] >= x) r = mid;              // mid 是候选，保留它
        else l = mid + 1;
    }
    return l;
}

int upperIdx(const vector<int>& a, int x)
{
    int l = 0, r = (int)a.size();
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (a[mid] > x) r = mid;               // 只改一个符号：>= 变 >
        else l = mid + 1;
    }
    return l;
}

long long minCapacity(const vector<int>& w, int days)
{
    long long lo = 0, hi = 0;
    for (size_t i = 0; i < w.size(); i++) {
        if (w[i] > lo) lo = w[i];              // 下界：最重的那一件
        hi += w[i];                            // 上界：一天全拉走
    }
    while (lo < hi) {                          // 第一个满足条件的位置 → mid 下取整
        long long mid = lo + (hi - lo) / 2;
        long long need = 1, cur = 0;
        for (size_t i = 0; i < w.size(); i++) {
            if (cur + w[i] > mid) { need++; cur = w[i]; }
            else cur += w[i];
        }
        if (need <= days) hi = mid;            // 够用 → 再试着少装一点
        else lo = mid + 1;                     // 不够 → 必须加大
    }
    return lo;
}

long long intSqrt(long long x)
{
    long long lo = 0, hi = 2000000000LL;
    while (lo < hi) {                          // 最后一个满足条件的位置 → mid 上取整
        long long mid = lo + (hi - lo + 1) / 2;
        if (mid <= x / mid) lo = mid;          // 用除法，避免 mid * mid 溢出
        else hi = mid - 1;
    }
    return lo;
}

int minInRotated(const vector<int>& a)
{
    if (a.empty()) return 0;
    int l = 0, r = (int)a.size() - 1;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (a[mid] > a[r]) l = mid + 1;        // 拐点在 mid 右边
        else r = mid;                          // mid 自己可能就是最小值
    }
    return a[l];
}

int lastNotGreaterThan(const vector<int>& a, int x)
{
    int l = -1, r = (int)a.size() - 1;         // l 是「已知 <= x」的候选，先取空
    while (l < r) {
        int mid = l + (r - l + 1) / 2;         // 有 l = mid 这条，必须上取整
        if (a[mid] <= x) l = mid;
        else r = mid - 1;
    }
    return l;
}

  为什么二分只有两套模板，而且不能混搭：
    每一套都在回答同一个问题——「怎么让区间严格变小，同时答案一定还在区间里」。
    右闭版 [l, r] 配 while (l <= r)，两侧都要 ±1，否则会原地踏步；
    右开版 [l, r) 配 while (l < r)，保留 mid 的那一侧写 r = mid（不减 1，
    因为 mid 本来就不在区间里），排除 mid 的那一侧写 l = mid + 1。
    一旦把「l = mid」和「mid 下取整」放在一起，区间宽度是 1 时 mid == l，
    赋值后区间一动不动——这就是所有二分死循环的成因，一个符号的事。

  为什么 lowerIdx 的 r 初值是 size()：
    「第一个 >= x」完全可能不存在，此时答案就是「插到所有数后面」= size()。
    把 r 取成 size() 且用右开区间，正好让「空区间 [k, k)」成为合法的终点，
    循环退出时 l == r == k，什么特判都不用写。
    顺带保证了 a[mid] 永远合法：mid < r <= size()。

  为什么要自己手写一遍再和 STL 对拍：
    lower_bound 返回的是迭代器，减 begin() 才是下标，很多人第一步就卡在这。
    更重要的是它是「未定义行为探测器」之外的第二双眼睛：
    对拍一致，说明你的语义和标准库一致；不一致，说明你对模板的理解有洞。
    竞赛里任何优化算法都该这么和暴力互验（讲义 09 第 2 节说的就是这个）。

  为什么二分答案要先证明单调性：
    minCapacity 的前提是「运力越大越容易送完」——这句话一眼就能看出来，所以能用。
    反过来说，如果 check 不是单调的，二分会给你一个「说不清是什么」的下标：
    左边一半成立、右边一半成立，中间可能全是洞，跳过去的答案再也回不来。
    判断方向的口诀：求最小可行值 → 成立时收缩右边界；求最大可行值 → 成立时收缩左边界。
    写反了不会报错，只会给你一个恒等于上界或下界的数字，样例还特别容易碰巧对。

  为什么 intSqrt 里要写 x / mid 而不是 mid * mid：
    上界取到 2e9 是想覆盖 x = 1e18 那种规模；这时 mid * mid 是 4e18，
    再往前一步就超过 long long 的上限 9.2e18 的邻域，直接溢出（有符号溢出，UB）。
    除法版本 mid <= x / mid 两边都不会超过 x，天然安全。
    代价是 mid == 0 时会除零——所以 lo 从 0 起时必须配上取整，
    保证循环里 mid >= lo + 1 >= 1。这两条是绑在一起的，别拆开改。

  为什么旋转数组要和 a[r] 比，不能和 a[l] 比：
    a[mid] > a[r] 说明 mid 落在右边那段递增区之前、拐点在 mid 右侧，且 mid 一定不是最小值，
    可以放心 l = mid + 1；a[mid] <= a[r] 说明 mid 就在最小值所在的那一段上，
    它自己就是候选，只能 r = mid。
    和 a[l] 比就没这么干脆：整个数组本来就有序时 a[mid] > a[l] 也成立，
    你会把答案区间砍掉一半。拿 {1,2,3,4,5} 和 {5,1,2,3,4} 各手推一遍就记住了。
 * ========================================================================== */
