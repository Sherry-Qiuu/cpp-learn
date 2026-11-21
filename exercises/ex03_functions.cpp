/* ============================================================================
 * ex03_functions.cpp —— 函数定义 · 三种传参 · 返回值 · 重载 · 默认参数
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 三种参数写法（T / T& / const T&）按「要不要改、大不大」一眼选对
 *   2. 体会「值传参改不到实参」这件事，它不是 bug，是语言规则
 *   3. 把多个结果用返回值带出来，而不是塞一堆输出参数
 *   4. 重载与默认参数的边界；把一坨 main 拆成能单独测的小函数
 *
 * 编译运行：
 *   make run/ex03
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex03_functions.cpp \
 *          -o build/ex03.exe && ./build/ex03.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cassert>
using namespace std;

/* 反面教材，不是 TODO：值传参时函数里的一切修改都随栈帧销毁 */
void byValueNoEffect(int x)
{
    x = 100;
    cout << "  byValueNoEffect 内部把 x 改成 " << x << "，外面那位一点都不动\n";
}

/* 反面教材：static 局部变量跨调用保留，做「调用计数器」很方便 */
int callCount()
{
    static int n = 0;
    return ++n;
}

/* ---------------------------------------------------------------------------
 * TODO 1：手写 swap（这一题必须用引用才能做对）
 *
 *   期望：
 *     int x = 1, y = 2; swapValues(x, y);   ->  x == 2, y == 1
 *     int p = -5, q = 7; swapValues(p, q);  ->  p == 7, q == -5
 *     int z = 3; swapValues(z, z);          ->  z 还是 3
 *
 *   提示：参数写成 `int a, int b` 编译器不会报错，assert 会当场教你做人。
 *         借出来、盖上去、再放回去——顺序错了值就丢了。
 * ------------------------------------------------------------------------- */
void swapValues(int& a, int& b)
{
    /* TODO 在这里写实现 */
    return;
}

/* ---------------------------------------------------------------------------
 * TODO 2：一个函数里同时用「引用」和「const 引用」
 *
 *   规则：把 text 变成 text + "[" + tag + "]"，tag 只读不许动。
 *
 *   期望：
 *     string s = "hi"; string tag = "cpp";
 *     appendTag(s, tag);   ->  s == "hi[cpp]"，且 tag 仍然是 "cpp"
 *     appendTag(s, "again");  ->  s == "hi[cpp][again]"
 *
 *   提示：第一个参数少了 & 就改不到实参；第二个参数写成 string tag 也能跑，
 *         但每次调用都完整拷贝一遍字符串——大对象这样传是性能问题，
 *         而且丢了「我不会改你」这个承诺。选 const string&。
 * ------------------------------------------------------------------------- */
void appendTag(string& text, const string& tag)
{
    /* TODO 在这里写实现 */
    return;
}

/* ---------------------------------------------------------------------------
 * TODO 3：vector 作参数 + 累加器该用什么类型
 *
 *   期望：
 *     sumOf({})                                -> 0
 *     sumOf({1, 2, 3})                         -> 6
 *     sumOf({-1, 1})                           -> 0
 *     sumOf({2000000000, 2000000000})          -> 4000000000
 *     sumOf({2000000000, 2000000000, -1})      -> 3999999999
 *
 *   提示：参数用 const vector<int>&，别拷一整个数组。
 *         陷阱在累加器：两个 2e9 相加已经超过 int 上限 2147483647，
 *         如果 acc 是 int，加法发生的那一刻就已经废了，最后返回 long long 救不回来。
 *         这一条和 ex01 的 area() 是同一个道理。
 * ------------------------------------------------------------------------- */
long long sumOf(const vector<int>& v)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 4：函数重载——同名求最小值，三个版本
 *
 *   期望：
 *     minOf(3, 7)         -> 3
 *     minOf(3, 7, 2)      -> 2
 *     minOf(2.5, 1.5)     -> 1.5
 *
 *   提示：三个签名分别是 (int, int)、(int, int, int)、(double, double)。
 *         三参数那个可以直接调用两参数那个，别复制粘贴逻辑。
 *         重载只看**参数列表**，返回类型不同不构成重载——先想想为什么。
 *         还有：minOf(3, 4.0) 这个调用会编译失败，想想编译器为什么不选。
 * ------------------------------------------------------------------------- */
int minOf(int a, int b)
{
    /* TODO 在这里写实现 */
    return -1;
}

int minOf(int a, int b, int c)
{
    /* TODO 在这里写实现 */
    return -1;
}

double minOf(double a, double b)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 5：默认参数——base 的 exp 次方，不写 exp 就当作平方
 *
 *   期望：
 *     power(5)      -> 25        （exp 默认 2）
 *     power(2, 10)  -> 1024
 *     power(0, 0)   -> 1         （空乘积约定为 1，别返回 0）
 *     power(-3, 3)  -> -27
 *     power(1, 100) -> 1
 *
 *   提示：默认值写在参数表里 `int exp = 2`；本文件只有定义没有单独声明，
 *         所以写这一处就够。真实工程里默认值要写在头文件的声明里。
 *         保证 exp >= 0，负数不用管。循环从 1 开始乘几次？先想清楚 r 的初值。
 * ------------------------------------------------------------------------- */
int power(int base, int exp = 2)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：把多个结果带出来的两种写法
 *
 *   写法一（引用输出参数，竞赛最快）：找到了把下标写进 pos 返回 true；
 *     没找到把 pos 置为 **-1** 返回 false。没找到时也必须赋值，
 *     留着调用方的旧值就是脏数据。
 *   写法二（返回值，语义最干净）：minMaxOf 返回 {最小值, 最大值}，空表 {-1, -1}。
 *
 *   期望：
 *     findFirst({4,5,4}, 4, pos) -> true, pos == 0   （first，不是 last）
 *     findFirst({4,5,4}, 7, pos) -> false, pos == -1
 *     minMaxOf({3,1,7,2}) -> {1, 7}    minMaxOf({42}) -> {42, 42}
 *
 *   提示：minMaxOf 的 lo/hi 初值取 v[0]；取 0 的话全负数组必错——这就是
 *         「累加器初始化 0、最值初始化第一个元素」这条规则的来源。
 * ------------------------------------------------------------------------- */
bool findFirst(const vector<int>& v, int target, int& pos)
{
    /* TODO 在这里写实现 */
    return false;
}

pair<int, int> minMaxOf(const vector<int>& v)
{
    /* TODO 在这里写实现 */
    return {-1, -1};
}

/* ---------------------------------------------------------------------------
 * TODO 7：拆小函数——先写 gcd，再让 lcm 调用它
 *
 *   期望：
 *     gcdOf(12, 18)   -> 6      gcdOf(7, 13)          -> 1
 *     gcdOf(0, 5)     -> 5      gcdOf(-4, 6)          -> 2
 *     lcmOf(4, 6)     -> 12     lcmOf(0, 5)           -> 0
 *     lcmOf(-4, 6)    -> 12     lcmOf(100000, 99999)  -> 9999900000
 *
 *   提示：辗转相除用 while (b != 0)：t = a % b; a = b; b = t; 返回 a。
 *         lcm 千万别写成 a * b / g——乘法先算，100000 * 99999 在 int 里已经爆了。
 *         正确顺序是 **先除后乘**：a / g * b，而且中间要在 long long 里做。
 *         负数：先取绝对值再算。注意别对 INT_MIN 直接取负（那是 UB），
 *         先把值搬到 long long 再处理符号。
 * ------------------------------------------------------------------------- */
int gcdOf(int a, int b)
{
    /* TODO 在这里写实现 */
    return -1;
}

long long lcmOf(int a, int b)
{
    /* TODO 在这里写实现 */
    return -1;
}

int main()
{
    cout << "========== 任务 1：必须用引用才能改到实参 ==========\n";
    {
        int x = 1, y = 2;
        swapValues(x, y);
        assert(x == 2 && y == 1);
        int p = -5, q = 7;
        swapValues(p, q);
        assert(p == 7 && q == -5);
        int z = 3;
        swapValues(z, z);
        assert(z == 3);
        int zero = 0, big = 2147483647;
        swapValues(zero, big);
        assert(zero == 2147483647 && big == 0);
    }
    cout << "  swapValues 全部通过\n";

    cout << "\n========== 任务 2：引用 + const 引用混用 ==========\n";
    {
        string text = "hi";
        string label = "cpp";
        appendTag(text, label);
        assert(text == "hi[cpp]");
        assert(label == "cpp");
        appendTag(text, "again");
        assert(text == "hi[cpp][again]");
        string blank;
        appendTag(blank, "");
        assert(blank == "[]");
    }
    cout << "  appendTag 全部通过\n";

    cout << "\n========== 任务 3：累加器的类型 ==========\n";
    assert(sumOf({}) == 0);
    assert(sumOf({1, 2, 3}) == 6);
    assert(sumOf({-1, 1}) == 0);
    assert(sumOf({2000000000, 2000000000}) == 4000000000LL);
    assert(sumOf({2000000000, 2000000000, -1}) == 3999999999LL);
    cout << "  sumOf 全部通过\n";

    cout << "\n========== 任务 4：重载 ==========\n";
    assert(minOf(3, 7) == 3);
    assert(minOf(7, 3) == 3);
    assert(minOf(5, 5) == 5);
    assert(minOf(3, 7, 2) == 2);
    assert(minOf(2, 1, 0) == 0);
    assert(minOf(-4, -9) == -9);
    assert(minOf(2.5, 1.5) == 1.5);
    assert(minOf(0.0, -0.5) == -0.5);
    cout << "  minOf 三个重载全部通过\n";

    cout << "\n========== 任务 5：默认参数 ==========\n";
    assert(power(5) == 25);
    assert(power(0) == 0);
    assert(power(-4) == 16);
    assert(power(2, 10) == 1024);
    assert(power(0, 0) == 1);
    assert(power(-3, 3) == -27);
    assert(power(1, 100) == 1);
    cout << "  power 全部通过\n";

    cout << "\n========== 任务 6：把结果带出来 ==========\n";
    {
        int pos = 999;
        assert(findFirst({4, 5, 4}, 4, pos) == true && pos == 0);
        assert(findFirst({4, 5, 4}, 5, pos) == true && pos == 1);
        assert(findFirst({4, 5, 4}, 7, pos) == false && pos == -1);
        assert(findFirst({}, 1, pos) == false && pos == -1);

        pair<int, int> mm = minMaxOf({3, 1, 7, 2});
        assert(mm.first == 1 && mm.second == 7);
        assert(minMaxOf({42}).first == 42 && minMaxOf({42}).second == 42);
        assert(minMaxOf({}).first == -1 && minMaxOf({}).second == -1);
        auto [lo, hi] = minMaxOf({-9, -1, -5});      // C++17 结构化绑定
        assert(lo == -9 && hi == -1);
    }
    cout << "  findFirst / minMaxOf 全部通过\n";

    cout << "\n========== 任务 7：gcd 与 lcm ==========\n";
    assert(gcdOf(12, 18) == 6);
    assert(gcdOf(18, 12) == 6);
    assert(gcdOf(7, 13) == 1);
    assert(gcdOf(0, 5) == 5);
    assert(gcdOf(-4, 6) == 2);
    assert(lcmOf(4, 6) == 12);
    assert(lcmOf(3, 5) == 15);
    assert(lcmOf(0, 5) == 0);
    assert(lcmOf(-4, 6) == 12);
    assert(lcmOf(100000, 99999) == 9999900000LL);
    cout << "  gcdOf / lcmOf 全部通过\n";

    cout << "\n========== 观察题：不改代码，先猜再跑 ==========\n";
    int outer = 7;
    byValueNoEffect(outer);
    cout << "  回到 main，outer 仍然是 " << outer << "（值传参 = 副本）\n";
    assert(outer == 7);
    cout << "  callCount() 连叫三次：" << callCount() << " " << callCount()
         << " " << callCount() << "  <- static 局部跨调用保留\n";
    /* --- 编译器帮你拦住的两件事，写在这里感受一下 ---
     * ① minOf(3, 4.0)：一个要 double->int，一个要 int->double，
     *    两边都是「转换」，谁也不比谁更匹配 -> ambiguous call，编译直接失败。
     *    想调哪个就显式强转：minOf((double)3, 4.0)。
     * ② int f(int x) { if (x > 0) return x; }  ——  x <= 0 那条路径没 return，
     *    -Wall 会报 control reaches end of non-void function。别关警告，那是救命的。
     * ③ 大数组开全局：int dp[1000005]; 放函数里几 MB 直接爆栈（Windows 默认 1MB），
     *    放函数外是静态区，爱开多大开多大。
     */
    cout << "  以上三条都在注释里，自己改一版跑跑看，报错比看十遍讲义记得牢。\n";

    cout << "\n>>> ex03 全部通过。下一站：ex04_recursion.cpp\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

void swapValues(int& a, int& b)
{
    int t = a;
    a = b;
    b = t;
}

void appendTag(string& text, const string& tag)
{
    text += "[";
    text += tag;
    text += "]";
}

long long sumOf(const vector<int>& v)
{
    long long acc = 0;
    for (int x : v) acc += x;
    return acc;
}

int minOf(int a, int b)
{
    return a < b ? a : b;
}

int minOf(int a, int b, int c)
{
    return minOf(minOf(a, b), c);
}

double minOf(double a, double b)
{
    return a < b ? a : b;
}

int power(int base, int exp = 2)
{
    int r = 1;                       // 初值必须是 1：乘法的单位元
    for (int i = 0; i < exp; i++) r *= base;
    return r;
}

bool findFirst(const vector<int>& v, int target, int& pos)
{
    for (int i = 0; i < (int)v.size(); i++) {
        if (v[i] == target) {
            pos = i;
            return true;             // 提前 return，不需要 break 也不需要标记
        }
    }
    pos = -1;
    return false;
}

pair<int, int> minMaxOf(const vector<int>& v)
{
    if (v.empty()) return {-1, -1};
    int lo = v[0], hi = v[0];
    for (int x : v) {
        if (x < lo) lo = x;
        if (x > hi) hi = x;
    }
    return {lo, hi};
}

int gcdOf(int a, int b)
{
    long long x = a < 0 ? -(long long)a : (long long)a;
    long long y = b < 0 ? -(long long)b : (long long)b;
    while (y != 0) {
        long long t = x % y;
        x = y;
        y = t;
    }
    return (int)x;
}

long long lcmOf(int a, int b)
{
    if (a == 0 || b == 0) return 0;
    long long g = gcdOf(a, b);
    long long x = a < 0 ? -(long long)a : (long long)a;
    long long y = b < 0 ? -(long long)b : (long long)b;
    return x / g * y;                // 先除后乘
}

  为什么 swap 不加 & 就完全没用：
    `void swapValues(int a, int b)` 里的 a、b 是实参的副本，压在栈帧里，
    函数一返回栈帧就没了，改动跟着一起消失。值传参不是「优化掉的拷贝」，
    它就是拷贝。凡是「要把改动留在调用方」的函数，签名里必须有 &（或者把结果 return 出去）。

  为什么第二个参数是 const string&：
    不加 & 每次调用都要拷贝一遍整个字符串；加了 & 但不加 const，调用方就无法
    传字面量、传临时量，而且看不出你到底会不会改它。const 引用同时解决这三件事：
    不拷贝、能接右值、并在你误写 `tag = "x"` 时由编译器当场报错。
    只读大对象的参数，一律 const T&，这是 C++ 里唯一不用纠结的写法。

  为什么 lcm 必须先除后乘：
    a * b / g 里，乘法先算，100000 * 99999 ≈ 1e10 在 int 里已经溢出（有符号溢出是 UB），
    除回去也救不回来。a / g * b 数学上等价，而且 a/g 一定整除，中间值更小。
    即便如此，1e10 这个结果本身也放不进 int，所以返回类型还得是 long long——
    类型是「算之前就要定对」的东西，不是「算完再补救」的东西。

  为什么重载跟返回类型无关，而 minOf(3, 4.0) 会歧义：
    编译器先按名字找到一组同名函数，再拿实参去匹配参数列表。返回类型是调用完
    才起作用的信息，没法用来区分「你指的是哪一个」。
    minOf(3, 4.0) 时，(int,int) 版要把 double 缩成 int，(double,double) 版要把 int 展成 double，
    两边都需要一次转换，优先级打平——编译器不会替你猜，直接 ambiguous call 报错。
    这不是缺点：让编译器猜「你想要哪个类型」这种事，猜错了就是 WA。
 * ========================================================================== */
