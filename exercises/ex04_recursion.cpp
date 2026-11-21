/* ============================================================================
 * ex04_recursion.cpp —— 递归三要素 · 分治 · 汉诺塔 · 记忆化
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 把「边界 / 缩小 / 组合」这三要素写进肌肉记忆，缺一条就爆栈
 *   2. 同一个问题递归版与迭代版都能写，并说得出各自的代价
 *   3. 理解朴素递归为什么会指数级重复计算，加上数组存结果会变成什么
 *   4. 递归打印过程（汉诺塔）——它是后面 DFS 回溯模板的前置技能
 *
 * 编译运行：
 *   make run/ex04
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex04_recursion.cpp \
 *          -o build/ex04.exe && ./build/ex04.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <climits>
#include <cassert>
using namespace std;

/* 记忆化用的两张表：开在全局（静态区），别开在函数里爆栈。
   main 里会先把它们填成 -1，-1 表示「这个状态还没算过」。 */
long long memoF[60];
long long memoC[30][30];

/* ---------------------------------------------------------------------------
 * TODO 1：阶乘——递归版 + 迭代版，两个都要写
 *
 *   期望：
 *     factRec(0) -> 1   factRec(1) -> 1   factRec(5) -> 120
 *     factRec(13) -> 6227020800            ← 13! 就已经不是 int 了
 *     factRec(20) -> 2432902008176640000
 *     factIter(n) 对所有 n 与 factRec(n) 相等
 *
 *   提示：递归三要素——① 边界：n <= 1 直接返回 1，不再递归；
 *         ② 缩小：fact(n - 1)，规模严格变小，否则会自己叫自己到永远；
 *         ③ 组合：拿子问题的返回值乘上 n。
 *         写成 `return n * factRec(n - 1);` 是错的，n 是 int，
 *         乘法在 int 里做完再扩展已经晚了——先想清楚怎么把这一次乘法抬到 64 位。
 * ------------------------------------------------------------------------- */
long long factRec(int n)
{
    /* TODO 在这里写实现 */
    return -1;
}

long long factIter(int n)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：斐波那契的三种写法，跑一遍就知道递归的代价
 *
 *   规则：fib(0) = 0，fib(1) = 1，fib(n) = fib(n-1) + fib(n-2)。
 *
 *   期望：
 *     fibRec(0) -> 0   fibRec(1) -> 1   fibRec(10) -> 55   fibRec(27) -> 196418
 *     fibIter(n) 同上，而且它跑 fibIter(59) 也是瞬间出结果
 *     fibMemo(n) 同上，用全局数组 memoF 把算过的结果存下来
 *
 *   提示：fibRec 只要 3 行，但它的调用次数是 fib(n) 的 3.236 倍（精确公式
 *         T(n) = 2·fib(n+1) − 1，本机实测 fib(24) 返回 46368 却调用 150049 次）：
 *         算 fib(5) 要算 fib(3) 两次、fib(2) 三次，同一棵树里全是重复子问题。
 *         **assert 里绝对不许出现 fibRec(40) 以上的数**，那是 3.3 亿次调用起步
 *         （fib(45) 实测 36.7 亿次）。
 *         fibMemo：先查表（memoF[n] != -1 就直接交卷），没算过才算并写回表；
 *         查表之前必须已经处理完 n < 2 的边界，否则表里的 -1 会当成答案返回。
 * ------------------------------------------------------------------------- */
long long fibRec(int n)
{
    /* TODO 在这里写实现 */
    return -1;
}

long long fibIter(int n)
{
    /* TODO 在这里写实现 */
    return -1;
}

long long fibMemo(int n)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 3：分治求区间 [l, r]（闭区间）的最小值与最大值
 *
 *   期望：
 *     minMaxRange({3, 1, 7, 2}, 0, 3) -> {1, 7}
 *     minMaxRange({5}, 0, 0)          -> {5, 5}
 *     minMaxRange({-9, -1, -5}, 0, 2) -> {-9, -1}
 *     minMaxRange(v, 1, 0)            -> {INT_MAX, INT_MIN}   ← 空区间
 *
 *   提示：三要素在这里长这样——
 *         ① 边界有两层：l > r 是空区间，返回「什么都不影响」的中性值；
 *                     l == r 只剩一个元素，返回 {v[l], v[l]}。
 *         ② 缩小：mid = l + (r - l) / 2，切成 [l, mid] 和 [mid+1, r]。
 *            为什么不用 (l + r) / 2？想清楚 l、r 都很大时会发生什么。
 *         ③ 组合：两个 pair 再取一次 min / max。
 *         空区间那个边界不写，l > r 时递归永远切不下去，直接爆栈。
 * ------------------------------------------------------------------------- */
pair<int, int> minMaxRange(const vector<int>& v, int l, int r)
{
    /* TODO 在这里写实现 */
    return {0, 0};
}

/* ---------------------------------------------------------------------------
 * TODO 4：递归反转字符串
 *
 *   期望：
 *     reverseRec("")      -> ""
 *     reverseRec("a")     -> "a"
 *     reverseRec("abc")   -> "cba"
 *     reverseRec("12345") -> "54321"
 *     reverseRec("abab")  -> "baba"
 *
 *   提示：把「第一个字符」摘下来，递归反转剩下的，再把第一个字符接到尾巴上。
 *         边界是长度 <= 1，写成 s.size() == 0 也行但空串以外的 1 个字符怎么办？
 *         自己走一遍 "ab" 确认不会多算一次。
 *         代价：substr 每次拷一个子串，整趟是 O(n^2)。先用递归把思路跑通，
 *         然后再回头想：为什么这里其实该用双指针循环？
 * ------------------------------------------------------------------------- */
string reverseRec(const string& s)
{
    /* TODO 在这里写实现 */
    return "";
}

/* ---------------------------------------------------------------------------
 * TODO 5：汉诺塔——把移动过程一步一步记下来
 *
 *   规则：n 个盘从 from 柱经过 aux 柱搬到 to 柱。每走一步就往 moves 里 push 一条
 *         "盘编号: 起点->终点"（如 "1: A->C"），函数返回这一步之后的总步数。
 *
 *   期望（hanoi(2, 'A', 'B', 'C', moves)）：
 *     返回 3，moves == {"1: A->B", "2: A->C", "1: B->C"}
 *   期望（hanoi(3, 'A', 'B', 'C', moves)）：
 *     返回 7，moves == {"1: A->C", "2: A->B", "1: C->B", "3: A->C",
 *                       "1: B->A", "2: B->C", "1: A->C"}
 *     hanoi(0, ...) 返回 0 且 moves 为空。
 *
 *   提示：只有一个问题「把 n-1 个盘从某柱搬到某柱」，换个角色名再叫自己一次：
 *         ① 把上面 n-1 个 from -> aux（to 当中转）
 *         ② 把最大的第 n 号 from -> to，并记录这一步
 *         ③ 把 n-1 个 aux -> to（from 当中转）
 *         步数 = ① + 1 + ③ = 2^n - 1。别用 pow(2, n)——那是 double。
 *         注意：moves 一定要传引用。传值的话你记录的步骤全留在副本里，
 *         调用方一个都看不到（ex03 的 swap 就是这个坑）。
 * ------------------------------------------------------------------------- */
long long hanoi(int n, char from, char aux, char to, vector<string>& moves)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：组合数 C(n, k)，朴素递归 + 记忆化
 *
 *   规则：C(n, 0) = C(n, n) = 1；k > n 或 k < 0 时 = 0；
 *         C(n, k) = C(n-1, k-1) + C(n-1, k)（杨辉三角往上走）。
 *
 *   期望：
 *     combRec(5, 2)  -> 10     combRec(5, 0) -> 1     combRec(5, 5) -> 1
 *     combRec(5, 6)  -> 0      combRec(5, -1) -> 0    combRec(0, 0) -> 1
 *     combMemo(28, 14) -> 40116600     combMemo(29, 14) -> 77558760
 *
 *   提示：combRec 是树形递归，C(24, 12) 那种重复子问题会算到地老天荒，
 *         assert 里 combRec 的 n 别超过 24。
 *         combMemo 用全局 memoC[n][k]，先查表再算、算完写回。
 *         越界的 k（负数、大于 n、大于 29）要在「碰数组之前」就挡掉——
 *         顺序永远是先判合法再访问，反过来就是越界读。
 *         这两个函数合起来就是第 13 章的记忆化搜索：状态 (n, k)、转移两条、
 *         边界三条，凑齐了 DP 的骨架也就这么点东西。
 * ------------------------------------------------------------------------- */
int combRec(int n, int k)
{
    /* TODO 在这里写实现 */
    return -1;
}

long long combMemo(int n, int k)
{
    /* TODO 在这里写实现 */
    return -1;
}

int main()
{
    cout << "========== 任务 1：阶乘（递归 vs 迭代） ==========\n";
    assert(factRec(0) == 1);
    assert(factRec(1) == 1);
    assert(factRec(5) == 120);
    assert(factRec(12) == 479001600LL);
    assert(factRec(13) == 6227020800LL);
    assert(factRec(20) == 2432902008176640000LL);
    assert(factIter(0) == 1);
    assert(factIter(1) == 1);
    assert(factIter(20) == 2432902008176640000LL);
    for (int i = 0; i <= 20; i++) assert(factRec(i) == factIter(i));
    cout << "  factRec / factIter 全部通过\n";

    cout << "\n========== 任务 2：斐波那契三连 ==========\n";
    assert(fibRec(0) == 0);
    assert(fibRec(1) == 1);
    assert(fibRec(10) == 55);
    assert(fibRec(27) == 196418);
    assert(fibIter(0) == 0);
    assert(fibIter(1) == 1);
    assert(fibIter(27) == 196418);
    assert(fibIter(59) == 956722026041LL);
    for (int i = 0; i < 60; i++) memoF[i] = -1;
    assert(fibMemo(0) == 0);
    assert(fibMemo(1) == 1);
    assert(fibMemo(10) == 55);
    assert(fibMemo(45) == 1134903170LL);
    assert(fibMemo(59) == 956722026041LL);
    cout << "  fibRec / fibIter / fibMemo 全部通过\n";

    cout << "\n========== 任务 3：分治求最小最大 ==========\n";
    /* 下面比较 pair 的 assert 外面多套了一层括号，不是手滑：
       assert 是宏，宏按「逗号」切参数，而模板的尖括号 <> 不算括号，
       pair<int, int> 里那个逗号会把一条 assert 劈成两个参数。
       再套一层圆括号，逗号就被保护起来了。 */
    assert(minMaxRange({3, 1, 7, 2}, 0, 3).first == 1);
    assert(minMaxRange({3, 1, 7, 2}, 0, 3).second == 7);
    assert((minMaxRange({5}, 0, 0) == pair<int, int>(5, 5)));
    assert((minMaxRange({-9, -1, -5}, 0, 2) == pair<int, int>(-9, -1)));
    assert((minMaxRange({2, 8}, 0, 1) == pair<int, int>(2, 8)));
    assert((minMaxRange({1, 2, 3}, 1, 0) == pair<int, int>(INT_MAX, INT_MIN)));
    cout << "  minMaxRange 全部通过\n";

    cout << "\n========== 任务 4：递归反转字符串 ==========\n";
    assert(reverseRec("") == "");
    assert(reverseRec("a") == "a");
    assert(reverseRec("ab") == "ba");
    assert(reverseRec("abc") == "cba");
    assert(reverseRec("12345") == "54321");
    assert(reverseRec("abab") == "baba");
    cout << "  reverseRec 全部通过\n";

    cout << "\n========== 任务 5：汉诺塔 ==========\n";
    {
        vector<string> moves;
        assert(hanoi(0, 'A', 'B', 'C', moves) == 0);
        assert(moves.empty());

        moves.clear();
        assert(hanoi(1, 'A', 'B', 'C', moves) == 1);
        assert(moves == vector<string>({"1: A->C"}));

        vector<string> want2 = {"1: A->B", "2: A->C", "1: B->C"};
        moves.clear();
        assert(hanoi(2, 'A', 'B', 'C', moves) == 3);
        assert(moves == want2);

        vector<string> want3 = {"1: A->C", "2: A->B", "1: C->B", "3: A->C",
                               "1: B->A", "2: B->C", "1: A->C"};
        moves.clear();
        assert(hanoi(4, 'A', 'B', 'C', moves) == 15);
        moves.clear();
        assert(hanoi(3, 'A', 'B', 'C', moves) == 7);
        assert(moves == want3);
    }
    cout << "  hanoi 全部通过\n";

    cout << "\n========== 任务 6：组合数 ==========\n";
    for (int i = 0; i < 30; i++)
        for (int j = 0; j < 30; j++) memoC[i][j] = -1;
    assert(combRec(0, 0) == 1);
    assert(combRec(5, 2) == 10);
    assert(combRec(5, 0) == 1);
    assert(combRec(5, 5) == 1);
    assert(combRec(5, 6) == 0);
    assert(combRec(5, -1) == 0);
    assert(combRec(24, 12) == 2704156);
    assert(combMemo(5, 2) == 10);
    assert(combMemo(28, 14) == 40116600LL);
    assert(combMemo(29, 14) == 77558760LL);
    assert(combMemo(29, 30) == 0);
    assert(combMemo(29, -3) == 0);
    for (int n = 0; n <= 16; n++)
        for (int k = 0; k <= n; k++) assert(combRec(n, k) == (int)combMemo(n, k));
    cout << "  combRec / combMemo 全部通过\n";

    cout << "\n========== 观察题：不改代码，先猜再跑 ==========\n";
    /* --- 下面两段都会栈溢出，所以它们留在注释里，别取消注释跑 ---
     * ① 缺 base case：
     * long long bad1(int n) { return n * bad1(n - 1); }
     *    编译能过、n=5 也「看起来正常」？不，它连 n=1 都不肯停，一路减到负无穷。
     * ② 边界不缩小（讲义 05 十个高频错误里的第 2 条）：
     * long long bad2(int n) { if (n == 0) return 1; return bad2(n); }
     *    有 if、有 return、看着挺像递归，就是规模一点没变，直接爆栈。
     * 本机 Windows 默认 1MB 栈：帧很小时约 4.3 万层就崩；函数里放个
     * int buf[256] 的话 1000 层左右就没了。所以深度可能上千的递归，
     * 要么把大数组挪到全局，要么改写成迭代 + 手写 stack。
     */
    cout << "  递归爆栈的真实症状是「程序直接崩，连 assert 都来不及报行号」，\n"
         << "  所以每次写递归先问三句：边界在哪？规模有没有严格变小？组合方式对不对？\n";
    cout << "  汉诺塔 n = 20 会打印 1048575 步、递归深度只有 20——步数多不等于深度大。\n"
         << "  这句话能帮你分清「时间不够」和「栈不够」。\n";

    cout << "\n>>> ex04 全部通过。下一站：ex05_arrays_vectors.cpp\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

long long factRec(int n)
{
    if (n <= 1) return 1;                        // ① 边界
    return (long long)n * factRec(n - 1);        // ② 缩小 ③ 组合
}

long long factIter(int n)
{
    long long r = 1;
    for (int i = 2; i <= n; i++) r *= i;
    return r;
}

long long fibRec(int n)
{
    if (n < 2) return n;
    return fibRec(n - 1) + fibRec(n - 2);
}

long long fibIter(int n)
{
    long long a = 0, b = 1;                      // a = fib(0), b = fib(1)
    for (int i = 0; i < n; i++) {
        long long t = a + b;
        a = b;
        b = t;
    }
    return a;
}

long long fibMemo(int n)
{
    if (n < 2) return n;                         // 边界先处理，别把 0 存进表里
    long long& r = memoF[n];                     // 引用当别名：改 r 就是改 memoF[n]
    if (r != -1) return r;                       // 算过就直接交卷
    return r = fibMemo(n - 1) + fibMemo(n - 2);
}

pair<int, int> minMaxRange(const vector<int>& v, int l, int r)
{
    if (l > r) return {INT_MAX, INT_MIN};        // 空区间：中性值
    if (l == r) return {v[l], v[l]};             // 只剩一个元素
    int mid = l + (r - l) / 2;                   // 不用 (l + r) / 2：防溢出
    pair<int, int> L = minMaxRange(v, l, mid);
    pair<int, int> R = minMaxRange(v, mid + 1, r);
    return {min(L.first, R.first), max(L.second, R.second)};
}

string reverseRec(const string& s)
{
    if (s.size() <= 1) return s;
    return reverseRec(s.substr(1)) + s[0];
}

long long hanoi(int n, char from, char aux, char to, vector<string>& moves)
{
    if (n == 0) return 0;
    long long steps = hanoi(n - 1, from, to, aux, moves);
    moves.push_back(to_string(n) + ": " + from + "->" + to);
    return steps + 1 + hanoi(n - 1, aux, from, to, moves);
}

int combRec(int n, int k)
{
    if (k < 0 || k > n) return 0;                // 挡住非法状态，也挡住递归的不合法分支
    if (k == 0 || k == n) return 1;
    return combRec(n - 1, k - 1) + combRec(n - 1, k);
}

long long combMemo(int n, int k)
{
    if (k < 0 || k > n || n > 29) return 0;      // 先判合法，再碰数组
    if (k == 0 || k == n) return 1;
    long long& r = memoC[n][k];
    if (r != -1) return r;
    return r = combMemo(n - 1, k - 1) + combMemo(n - 1, k);
}

  为什么递归必须先想边界，再写递归调用：
    三要素里最容易漏的是「边界」，但它恰恰是递归唯一的出口。写成
    `return n * fact(n - 1)` 而没有 `if (n <= 1) return 1`，程序不会给你
    「死循环」的机会——栈帧一层层压下去，几万次调用后直接崩，
    连 assert 的行号都来不及打印。所以顺序永远是：先写下 base case，
    再写缩小，最后写怎么把子问题的答案组合起来。

  为什么 fibRec 慢、fibMemo 快，快在哪：
    fibRec 的调用是一棵树，fib(5) 里 fib(3) 出现两次、fib(2) 出现三次，
    叶子数是 fib(n) 量级，所以总调用次数 T(n) = 2·fib(n+1) − 1——指数，
    比值趋近 2φ ≈ 3.236（本机实测 fib(24) 调用 150049 次，fib(45) 调用 36.7 亿次）。
    加一张表之后，每个状态 n 只算一次，剩下的都是查表，复杂度降到 O(n)。
    这就是「记忆化 = 自顶向下的 DP」：状态、转移、边界三样东西一个都没变，
    只是给重复计算加了一道闸门。第 13 章写的就是它。

  为什么哨兵值要选 -1，而不是靠全局数组默认的 0：
    全局数组自动初始化为 0，而 0 常常是合法答案（fib(0) 就是 0）。
    用 0 当「没算过」， fib(0) 这种结果会被当成脏数据反复重算，
    更糟的是有些题里 0 是唯一正确答案，逻辑就整个错了。
    所以先把表填成 -1（一个「不可能是答案」的值），再让 n < 2 的边界走 return 不进表。

  为什么汉诺塔的 moves 必须是引用：
    这道题的答案不在返回值里，而在「一路 push 出来的移动清单」里。传值的话每次调用
    都拷一份新 vector，子调用 push 进去的步骤全在副本上，函数一返回就销毁，
    调用方拿到的是空表。要往外带东西，只有两条路：return 出来，或者用引用带出去。
    深度方向要「做选择 → 递归 → 撤销选择」的那类题（第 11 章的 DFS 回溯模板），
    用的正是这里的引用。
 * ========================================================================== */
