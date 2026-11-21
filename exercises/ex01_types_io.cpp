/* ============================================================================
 * ex01_types_io.cpp —— 类型 · 转换 · 整数除法 · 格式化输出
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 把「整数除法会截断」这件事练到条件反射
 *   2. 会写向上取整除法、数位拆分（枚举题的基本功）
 *   3. 意识到 int 什么时候会溢出，并用 long long 救回来
 *   4. 用 cout 控制小数位数与对齐
 *
 * 编译运行：
 *   make run/ex01
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex01_types_io.cpp \
 *          -o build/ex01.exe && ./build/ex01.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <cassert>
using namespace std;

/* ---------------------------------------------------------------------------
 * TODO 1：向上取整除法（a, b 都是正整数）
 *
 *   期望：
 *     ceilDiv(7, 2)  -> 4
 *     ceilDiv(8, 2)  -> 4
 *     ceilDiv(1, 5)  -> 1
 *     ceilDiv(10, 3) -> 4
 *
 *   提示：a / b 是向下取整。什么时候要 +1？想想 (a + b - 1) / b 这个经典写法。
 *         不许用 double 再转回来（精度会咬你）。
 * ------------------------------------------------------------------------- */
int ceilDiv(int a, int b)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：算一个非负整数的「数位和」
 *
 *   期望：
 *     digitSum(0)      -> 0
 *     digitSum(9)      -> 9
 *     digitSum(1234)   -> 10
 *     digitSum(999999) -> 54
 *
 *   提示：x % 10 拿到最后一位，x / 10 去掉最后一位，循环到 x == 0。
 *         这就是「逐位处理整数」的标准模板，后面判断回文数、
 *         统计数字出现次数都靠它。
 * ------------------------------------------------------------------------- */
int digitSum(int x)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 3：把「只含数字的字符串」转成整数，不许用 stoi
 *
 *   期望：
 *     toInt("0")        -> 0
 *     toInt("123")      -> 123
 *     toInt("007")      -> 7
 *     toInt("9999999999") -> 9999999999   ← 注意这已经超过 int 了
 *
 *   提示：核心是 res = res * 10 + (s[i] - '0')。
 *         想一想 res 该声明成什么类型，才能让最后一组用例不溢出。
 *         （这一条就是本练习的重点：先算范围再定类型）
 * ------------------------------------------------------------------------- */
long long toInt(const string& s)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 4：把「整数」转成「字符串」，不许用 to_string
 *
 *   期望：
 *     itoA(0)    -> "0"
 *     itoA(42)   -> "42"
 *     itoA(-7)   -> "-7"
 *     itoA(1000) -> "1000"
 *
 *   提示：从低位往高位取，得到的顺序是反的。
 *         要么先存进 string 再 reverse，要么每次插到开头（想清楚代价）。
 *         别忘了 0 这个特例——循环条件 x != 0 时它一次都不执行。
 * ------------------------------------------------------------------------- */
string itoA(int x)
{
    /* TODO 在这里写实现 */
    return "";
}

/* ---------------------------------------------------------------------------
 * TODO 5：算矩形面积，参数是 int，结果可能超过 int
 *
 *   期望：
 *     area(3, 4)          -> 12
 *     area(100000, 100000) -> 10000000000   ← 1e10，int 装不下
 *
 *   陷阱：写成 long long r = w * h; 是错的。
 *         为什么错？（提示：乘法先于赋值发生）
 * ------------------------------------------------------------------------- */
long long area(int w, int h)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：格式化输出辅助——把秒数变成 "H:MM:SS" 或 "MM:SS"
 *
 *   期望：
 *     formatTime(0)    -> "0:00:00"
 *     formatTime(59)   -> "0:00:59"
 *     formatTime(605)  -> "0:10:05"
 *     formatTime(3723) -> "1:02:03"
 *
 *   提示：需要 <sstream> 或 <iomanip> + ostringstream 把结果写进字符串。
 *         setw / setfill 每次都要重写，因为 setw 只作用一次。
 * ------------------------------------------------------------------------- */
string formatTime(int totalSeconds)
{
    /* TODO 在这里写实现 */
    return "";
}

int main()
{
    cout << "========== 任务 1：向上取整除法 ==========\n";
    assert(ceilDiv(7, 2) == 4);
    assert(ceilDiv(8, 2) == 4);
    assert(ceilDiv(1, 5) == 1);
    assert(ceilDiv(10, 3) == 4);
    assert(ceilDiv(999999, 1000) == 1000);
    cout << "  ceilDiv 全部通过\n";

    cout << "\n========== 任务 2：数位和 ==========\n";
    assert(digitSum(0) == 0);
    assert(digitSum(9) == 9);
    assert(digitSum(1234) == 10);
    assert(digitSum(999999) == 54);
    cout << "  digitSum 全部通过\n";

    cout << "\n========== 任务 3：字符串转整数 ==========\n";
    assert(toInt("0") == 0);
    assert(toInt("123") == 123);
    assert(toInt("007") == 7);
    assert(toInt("9999999999") == 9999999999LL);
    cout << "  toInt 全部通过\n";

    cout << "\n========== 任务 4：整数转字符串 ==========\n";
    assert(itoA(0) == "0");
    assert(itoA(42) == "42");
    assert(itoA(-7) == "-7");
    assert(itoA(1000) == "1000");
    assert(itoA(-2147483648) == "-2147483648");
    cout << "  itoA 全部通过\n";

    cout << "\n========== 任务 5：溢出 ==========\n";
    assert(area(3, 4) == 12);
    assert(area(100000, 100000) == 10000000000LL);
    cout << "  area 全部通过\n";
    unsigned int wrap = 4294967295u;      // unsigned int 的最大值
    wrap = wrap + 1u;                     // 无符号溢出 = 回绕，行为完全确定
    cout << "  无符号回绕：4294967295u + 1u = " << wrap
         << "（绕回 0，这是「确定行为」，不是 bug）\n";

    /* --- 有符号溢出是 UB，所以这行故意留在注释里，别取消注释跑 ---
     * int big = 100000;
     * int bad = big * big;               // 100000*100000 放不进 int，编译器可干任何事
     * 正确写法是 area() 那样：先把其中一个操作数转成 long long，
     * 让「乘法」在 64 位里做，而不是在 32 位里做完再扩展。
     */
    cout << "  对比：100000 * 100000 如果两边都是 int，结果放不进 32 位 —— 那是 UB。\n"
         << "        area() 用 1LL * 把乘法抬到 64 位，所以答案才是对的。\n";

    cout << "\n========== 任务 6：格式化 ==========\n";
    assert(formatTime(0) == "0:00:00");
    assert(formatTime(59) == "0:00:59");
    assert(formatTime(605) == "0:10:05");
    assert(formatTime(3723) == "1:02:03");
    cout << "  formatTime 全部通过\n";

    cout << "\n========== 观察题：不改代码，先猜再跑 ==========\n";
    cout << "  7 / 2 = " << 7 / 2 << "        （期望你猜 3）\n";
    cout << "  -7 / 2 = " << -7 / 2 << "       （向 0 取整，不是 -4）\n";
    cout << "  -7 % 2 = " << -7 % 2 << "       （余数符号跟被除数）\n";
    cout << "  (double)7 / 2 = " << (double)7 / 2 << "\n";
    cout << fixed << setprecision(2);
    cout << "  保留两位：" << 3.14159 << "  " << 2.0 << "\n";
    cout << "  宽度对齐：" << setw(6) << 42 << "|" << setw(6) << 7 << "|\n";

    cout << "\n>>> ex01 全部通过。下一站：ex02_control_flow.cpp\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

int ceilDiv(int a, int b)
{
    return (a + b - 1) / b;
}

int digitSum(int x)
{
    int s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

long long toInt(const string& s)
{
    long long res = 0;
    for (char c : s) res = res * 10 + (c - '0');
    return res;
}

string itoA(int x)
{
    if (x == 0) return "0";
    bool neg = x < 0;
    unsigned int u = neg ? 0u - (unsigned int)x : (unsigned int)x;   // 处理 INT_MIN
    string s;
    while (u > 0) {
        s += char('0' + u % 10);
        u /= 10;
    }
    if (neg) s += '-';
    reverse(s.begin(), s.end());
    return s;
}

long long area(int w, int h)
{
    return 1LL * w * h;
}

string formatTime(int totalSeconds)
{
    ostringstream os;
    int h = totalSeconds / 3600;
    int m = totalSeconds % 3600 / 60;
    int s = totalSeconds % 60;
    os << h << ':' << setw(2) << setfill('0') << m << ':' << setw(2) << setfill('0') << s;
    return os.str();
}

  为什么 (a + b - 1) / b 是向上取整：
    a / b 向下取整。若 a 能被 b 整除，加 b-1 不足以上一个台阶，结果不变；
    若不能整除，余数至少 1，加上 b-1 刚好凑够一个 b，商进一位。
    注意 a、b 必须为正，否则截断方向一变就不成立。

  为什么 toInt 的 res 必须是 long long：
    "9999999999" 有 10 位，超过 int 上限 2147483647。累加过程中就已经溢出了，
    最后再赋给 long long 也救不回来。这是「类型要在运算之前就对」的典型例子。

  为什么 itoA 里不能直接 x = -x：
    INT_MIN = -2147483648，取负后是 2147483648，超出 int 上限，属于有符号溢出（UB）。
    所以要先转成 unsigned int 再取负：0u - (unsigned int)x 的结果是确定的模 2^32 运算。
    这个技巧在任何「处理 INT_MIN」的题里都用得上。

  为什么 setw 后面还要重复写 setfill：
    setw 是「一次性」的，只对紧跟的输出项生效；setfill 是「粘性」的，设置后一直生效。
    所以两个 setw(2) 之间可以不重复写 setfill，但 setw 必须每次写。

  用 ostringstream 拼字符串的好处：
    它的用法和 cout 完全一样（<< 链式），但结果进字符串而不是屏幕。
    需要「先构造一段格式化文本再输出/比较」时比 sprintf 安全，比手工拼接清楚。
 * ========================================================================== */
