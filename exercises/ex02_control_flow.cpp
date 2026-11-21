/* ============================================================================
 * ex02_control_flow.cpp —— 条件判断 · 循环 · break/continue · 区间模拟
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 把 if / else if 的「顺序即逻辑」练到条件反射，边界值一个都不写错
 *   2. 会写「区间翻转」这类最基础的模拟题（灯开关、翻转区间 counting）
 *   3. break 与 continue 各归其位：一个终止整层循环，一个只结束本轮
 *   4. 循环边界：off-by-one 和 unsigned 倒序，在竞赛里这就是 WA
 *
 * 编译运行：
 *   make run/ex02
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex02_control_flow.cpp \
 *          -o build/ex02.exe && ./build/ex02.exe
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

/* ---------------------------------------------------------------------------
 * TODO 1：分段计分（if / else if 的排列顺序）
 *
 *   期望：
 *     gradeOf(100) -> 'A'    gradeOf(90) -> 'A'   ← 恰好 90 也算 A
 *     gradeOf(89)  -> 'B'    gradeOf(80) -> 'B'    gradeOf(79) -> 'C'
 *     gradeOf(60)  -> 'C'    gradeOf(59) -> 'D'    gradeOf(0)  -> 'D'
 *
 *   提示：条件从上往下写，先判最严的那档。要是把 >= 60 写在最前面，
 *         90 分永远进不了 A 分支——因为第一个命中的分支就 return 了。
 * ------------------------------------------------------------------------- */
char gradeOf(int score)
{
    /* TODO 在这里写实现 */
    return '?';
}

/* ---------------------------------------------------------------------------
 * TODO 2：FizzBuzz 变体（复合条件必须写在前面）
 *
 *   规则：能同时被 3 和 5 整除 -> "FizzBuzz"
 *         只能被 3 整除         -> "Fizz"
 *         只能被 5 整除         -> "Buzz"
 *         两个都不行            -> 数字本身转成字符串
 *
 *   期望：
 *     fizzLike(1) -> "1"   fizzLike(3) -> "Fizz"   fizzLike(5) -> "Buzz"
 *     fizzLike(15) -> "FizzBuzz"    fizzLike(0) -> "FizzBuzz"  ← 0 能被任何非零整数整除
 *
 *   提示：先判「两个都能整除」，否则 "FizzBuzz" 这条分支永远轮不到。
 *         转字符串直接用 to_string，不要重新发明。
 * ------------------------------------------------------------------------- */
string fizzLike(int n)
{
    /* TODO 在这里写实现 */
    return "";
}

/* ---------------------------------------------------------------------------
 * TODO 3：n 盏灯，m 次区间翻转（经典模拟）
 *
 *   灯编号 1..n，初始全部为 0（灭）。每个区间 [l, r] 是**闭区间**、**1 起算**，
 *   表示把 l 到 r 的灯全翻一次。返回长度 n 的下标从 0 开始的最终状态表。
 *
 *   期望：
 *     flipLights(5, {{1,3},{2,2}})   -> {1,0,1,0,0}
 *     flipLights(5, {{1,5},{2,4},{3,3}}) -> {1,0,1,0,1}
 *     flipLights(3, {})              -> {0,0,0}
 *     flipLights(0, {{1,1}})         -> 空表
 *
 *   提示：0/1 翻转用 `a[i] ^= 1`，比 if 干净。
 *         题面是 1 起算、闭区间，数组是 0 起算——这两套下标混在一起就是
 *         off-by-one 的温床。写循环时先把「i 代表第几盏灯」想清楚。
 * ------------------------------------------------------------------------- */
vector<int> flipLights(int n, const vector<pair<int, int>>& ops)
{
    /* TODO 在这里写实现 */
    return {};
}

/* ---------------------------------------------------------------------------
 * TODO 4：数 1..n 里有几个质数（break 的正确用法）
 *
 *   期望：
 *     countPrimesUpTo(-5) -> 0     countPrimesUpTo(0) -> 0
 *     countPrimesUpTo(1)  -> 0     countPrimesUpTo(2) -> 1
 *     countPrimesUpTo(10) -> 4     countPrimesUpTo(30) -> 10
 *
 *   提示：外层枚举 i，内层试除 j。内层一旦发现因子就 break——
 *         但 break 只跳**一层**，跳出来后你还得告诉外层「这个 i 不是质数」，
 *         所以要么留一个 bool 标记，要么把「判质数」抽成一个小函数用 return。
 *         内层条件写 `j * j <= i` 最直观；n 很大时 j*j 会爆 int，
 *         更稳的写法是 `j <= i / j`。这里 n 很小，两种都对，你选哪个？
 * ------------------------------------------------------------------------- */
int countPrimesUpTo(int n)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 5：倒着找最后一次出现的位置（本练习的陷阱题）
 *
 *   期望：
 *     lastIndexOf({}, 5)          -> -1
 *     lastIndexOf({1,2,5,3,5}, 5) -> 4
 *     lastIndexOf({1,2,5,3,5}, 2) -> 1
 *     lastIndexOf({1,2,3}, 9)     -> -1
 *
 *   提示：倒序遍历，第一个命中就是答案；没命中返回 -1（不要用 0，0 是合法下标）。
 *         危险写法：`for (size_t i = v.size() - 1; i >= 0; i--)`。
 *         空表时 size()-1 回绕成 18446744073709551615，越界读 + 死循环，
 *         而且 `i >= 0` 对无符号数恒真。循环变量用 int，并强转一次：
 *         `for (int i = (int)v.size() - 1; i >= 0; i--)`。
 * ------------------------------------------------------------------------- */
int lastIndexOf(const vector<int>& v, int target)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：整数回文（数位遍历 + while 的终止条件）
 *
 *   期望：
 *     isPalindromeNumber(0)         -> true
 *     isPalindromeNumber(7)         -> true
 *     isPalindromeNumber(121)       -> true
 *     isPalindromeNumber(123454321) -> true
 *     isPalindromeNumber(10)        -> false   ← 末尾的 0 会咬你
 *     isPalindromeNumber(-121)      -> false   ← 负号不是回文的一部分
 *
 *   提示：把 x 逐位反转成 rev，再和原值比。while 的终止条件写 x > 0，
 *         循环体里「先取位、再去位」，顺序反了就多算一位。
 *         本体的数都在 int 范围内，反转不会爆；真要保证不爆，
 *         就把 rev 声明成 long long——想想哪种习惯更省事。
 * ------------------------------------------------------------------------- */
bool isPalindromeNumber(int x)
{
    /* TODO 在这里写实现 */
    return false;
}

int main()
{
    cout << "========== 任务 1：分段计分 ==========\n";
    assert(gradeOf(100) == 'A');
    assert(gradeOf(90) == 'A');
    assert(gradeOf(89) == 'B');
    assert(gradeOf(80) == 'B');
    assert(gradeOf(79) == 'C');
    assert(gradeOf(60) == 'C');
    assert(gradeOf(59) == 'D');
    assert(gradeOf(0) == 'D');
    cout << "  gradeOf 全部通过\n";

    cout << "\n========== 任务 2：FizzBuzz 变体 ==========\n";
    assert(fizzLike(1) == "1");
    assert(fizzLike(3) == "Fizz");
    assert(fizzLike(5) == "Buzz");
    assert(fizzLike(15) == "FizzBuzz");
    assert(fizzLike(30) == "FizzBuzz");
    assert(fizzLike(0) == "FizzBuzz");
    cout << "  fizzLike 全部通过\n";

    cout << "\n========== 任务 3：n 盏灯 m 次区间翻转 ==========\n";
    {
        vector<pair<int, int>> ops;
        ops = {{1, 3}, {2, 2}};
        assert(flipLights(5, ops) == vector<int>({1, 0, 1, 0, 0}));

        ops = {{1, 5}, {2, 4}, {3, 3}};
        assert(flipLights(5, ops) == vector<int>({1, 0, 1, 0, 1}));

        ops.clear();
        assert(flipLights(3, ops) == vector<int>({0, 0, 0}));
        assert(flipLights(0, ops) == vector<int>{});
    }
    cout << "  flipLights 全部通过\n";

    cout << "\n========== 任务 4：数质数 ==========\n";
    assert(countPrimesUpTo(-5) == 0);
    assert(countPrimesUpTo(0) == 0);
    assert(countPrimesUpTo(1) == 0);
    assert(countPrimesUpTo(2) == 1);
    assert(countPrimesUpTo(4) == 2);
    assert(countPrimesUpTo(10) == 4);
    assert(countPrimesUpTo(30) == 10);
    cout << "  countPrimesUpTo 全部通过\n";

    cout << "\n========== 任务 5：倒序查找 ==========\n";
    assert(lastIndexOf({}, 5) == -1);
    assert(lastIndexOf({1, 2, 5, 3, 5}, 5) == 4);
    assert(lastIndexOf({1, 2, 5, 3, 5}, 2) == 1);
    assert(lastIndexOf({1, 2, 3}, 9) == -1);
    assert(lastIndexOf({7}, 7) == 0);
    cout << "  lastIndexOf 全部通过\n";

    cout << "\n========== 任务 6：整数回文 ==========\n";
    assert(isPalindromeNumber(0) == true);
    assert(isPalindromeNumber(7) == true);
    assert(isPalindromeNumber(121) == true);
    assert(isPalindromeNumber(123454321) == true);
    assert(isPalindromeNumber(10) == false);
    assert(isPalindromeNumber(1001) == true);
    assert(isPalindromeNumber(-121) == false);
    cout << "  isPalindromeNumber 全部通过\n";

    cout << "\n========== 观察题：不改代码，先猜再跑 ==========\n";
    /* --- 悬挂 else 的陷阱，故意留在注释里（编译器 -Wdangling-else 也会提醒你）---
     * int a = 0, b = 0;
     * if (a)
     *     if (b) cout << "分支 1";
     *     else   cout << "分支 2";        // 这个 else 配的是 if (b)，不是 if (a)
     * a == 0 时整块一句都不会输出——你以为「a 不成立就走 else」，
     * 但那个 else 早被内层 if 抢走了。永远写花括号。
     */
    int a = 0, b = 0;
    if (a) {
        if (b) cout << "  花括号版：分支 1\n";
        else   cout << "  花括号版：分支 2\n";
    } else {
        cout << "  花括号版：a==0，明确走这里（和上面注释里那段的行为不一样）\n";
    }

    int month = 11;
    string season = "?";
    switch (month) {
        case 1: case 2: case 12: season = "冬"; break;
        case 3: case 4: case 5:  season = "春"; break;
        case 6: case 7: case 8:  season = "夏"; break;
        default:                 season = "秋";   // 放在最后可以省 break；坑在上面那些漏 break 的 case
    }
    cout << "  switch 贯穿：month = " << month << " -> " << season
         << "（default 在最后也要 break 吗？它后面没语句了，可以省）\n";

    cout << "  死循环三成因（都在注释里，别真跑）：\n";
    /*  ① while 里忘了推进循环变量
     *  int i = 0;
     *  while (i < n) { cout << i; }              // i 永远不变
     *  ② 无符号数倒序：减到 0 再减变成巨大正数，条件永不成立
     *  for (size_t j = 5; j >= 0; j--) { }
     *  ③ 浮点数判等当终止条件：0.1 存不精确，永远不等于 1
     *  double x = 0.0;
     *  while (x != 1.0) { x += 0.1; }
     */
    cout << "  while 里用 continue 要先推进循环变量，否则卡在同一轮里出不来。\n";

    cout << "\n>>> ex02 全部通过。下一站：ex03_functions.cpp\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

char gradeOf(int score)
{
    char g;
    if (score >= 90)      g = 'A';
    else if (score >= 80) g = 'B';
    else if (score >= 60) g = 'C';
    else                  g = 'D';
    return g;
}

string fizzLike(int n)
{
    if (n % 3 == 0 && n % 5 == 0) return "FizzBuzz";
    if (n % 3 == 0) return "Fizz";
    if (n % 5 == 0) return "Buzz";
    return to_string(n);
}

vector<int> flipLights(int n, const vector<pair<int, int>>& ops)
{
    vector<int> lamp(n, 0);
    for (const auto& op : ops) {
        int l = op.first;
        int r = op.second;
        for (int i = l; i <= r; i++) lamp[i - 1] ^= 1;
    }
    return lamp;
}

int countPrimesUpTo(int n)
{
    int cnt = 0;
    for (int i = 2; i <= n; i++) {
        bool prime = true;
        for (int j = 2; j <= i / j; j++) {
            if (i % j == 0) {
                prime = false;
                break;                      // 只跳出内层这一层
            }
        }
        if (prime) cnt++;
    }
    return cnt;
}

int lastIndexOf(const vector<int>& v, int target)
{
    for (int i = (int)v.size() - 1; i >= 0; i--) {
        if (v[i] == target) return i;
    }
    return -1;
}

bool isPalindromeNumber(int x)
{
    if (x < 0) return false;                // 负号没法对称，直接否掉
    int orig = x;
    long long rev = 0;                      // 反转过程可能超过 int，先抬到 64 位
    while (x > 0) {
        rev = rev * 10 + x % 10;
        x /= 10;
    }
    return rev == orig;
}

  为什么区间翻转的循环写成 `for (int i = l; i <= r; i++)`：
    题面给的是「第 l 盏到第 r 盏」，闭区间、1 起算。数组下标是 0 起算，
    所以翻转时访问 lamp[i - 1]。要么循环跟着题面走（i 从 l 到 r，用 i-1 访问），
    要么先把区间换成 0 起算（l-1 到 r-1，直接访问 i）。**一个函数里只能用一套**，
    混着写就是 off-by-one。判据：i <= r 而不是 i < r，因为 r 这盏灯也要翻。

  为什么 break 之后还要一个 bool 标记：
    break 结束的是「当前这一层」循环。内层试除发现因子后 break，程序回到外层，
    外层并不知道刚才为什么 break——它只看到 j 停了。所以必须把结论用变量带出来
    （或者干脆把判定抽成 bool isPrime(int)，用 return 一次跳出，可读性最好）。
    这也是「嵌套循环想跳两层就抽函数」这条经验的具体形态。

  为什么倒序循环的变量必须是 int：
    size_t 是无符号类型，v.size() == 0 时 size() - 1 不是 -1，而是回绕成的巨大正数，
    第一下就越界；而且 `i >= 0` 对无符号恒真，循环永远退不出去。
    改成 int 之后空表时 i 从 -1 开始，条件立即为假，一次都不执行——这才是安全写法。
    「(int)v.size()」这个强转在本仓库里到处都有，就是为了让 -Wall 零警告。

  为什么 10 不是回文数、-121 也不是：
    逐位反转时末尾的 0 会丢掉（10 反转成 1），所以和原值不相等，判定正确；
    但如果用「双指针比较首尾位」的写法，就必须单独处理末尾 0，否则 10 会被误判。
    负数的符号位没法对称出现，约定一律返回 false，别把 -121 反转成 121 再比。
 * ========================================================================== */
