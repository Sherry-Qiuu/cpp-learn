/* ============================================================================
 * ex06_strings.cpp —— string 常用操作 · 拆词 · stringstream · 字符处理
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 把 substr / find / += 这几个高频成员函数用熟，参数含义一次都不记错
 *   2. 会「扫一遍字符串，边扫边攒数」的套路：提取数字、统计、判断回文
 *   3. 会用 istringstream 拆单词、按行读入，知道它和 cin >> / getline 的对应关系
 *   4. 字典序比较直接用 < ，并且知道大写字母排在小写前面
 *
 * 编译运行：
 *   make run/ex06
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex06_strings.cpp \
 *          -o build/ex06.exe && ./build/ex06.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cassert>
using namespace std;

/* ---------------------------------------------------------------------------
 * TODO 1：把字符串里所有整数抠出来求和
 *
 *   规则：只有连续的数字算一个数；正负号一律忽略；非数字是分隔符。
 *
 *   期望：
 *     sumOfNumbers("")       -> 0
 *     sumOfNumbers("abc")    -> 0
 *     sumOfNumbers("a1b2")   -> 3
 *     sumOfNumbers("x12y3")  -> 15
 *     sumOfNumbers("007")    -> 7      ← 前导 0 自然会被吃掉
 *     sumOfNumbers("-5a3")   -> 8
 *     sumOfNumbers("100500") -> 100500
 *     sumOfNumbers("a1")     -> 1      ← 数字顶到字符串末尾，别忘了结算最后一次
 *     sumOfNumbers("2024年11月21日") -> 2056
 *
 *   提示：套路是「攒一个 cur，遇到分隔符就结算进 total 并清零」，
 *         循环结束后必须再判一次「最后一个字符是不是数字」，否则结尾那段丢算。
 *         判断数字写 c >= '0' && c <= '9'，别用 isdigit(c)：
 *         char 可能是负数（有符号），传给 isdigit 是未定义行为。
 *         顺带一提：汉字的 UTF-8 字节也都落在负数区，所以那组用例照样算得对。
 * ------------------------------------------------------------------------- */
int sumOfNumbers(const string& s)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：用 find 统计子串出现次数（不重叠）
 *
 *   期望：
 *     countNonOverlap("aaaa", "aa")     -> 2    ← 不重叠，所以不是 3
 *     countNonOverlap("aaa", "a")       -> 3
 *     countNonOverlap("abcabcabc", "abc") -> 3
 *     countNonOverlap("hello", "z")     -> 0
 *     countNonOverlap("", "a")          -> 0
 *     countNonOverlap("aaaa", "")       -> 0    ← 空模式必须挡掉
 *
 *   提示：find(pat, pos) 从 pos 开始找，找不到返回 string::npos（不是 -1）。
 *         下一轮从哪里开始，决定了「重叠计数」还是「不重叠计数」——
 *         pos + 1 是重叠版，pos + pat.size() 是本题要的不重叠版。
 *         陷阱：pat 为空时 find 每次都在原地命中，pos += 0 就是死循环。
 *         函数第一行先把这种输入挡掉，别指望调用方给合法参数。
 * ------------------------------------------------------------------------- */
int countNonOverlap(const string& text, const string& pat)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 3：拆单词，返回最长的那个；并列时返回字典序最小的
 *
 *   期望：
 *     longestWord("")                  -> ""
 *     longestWord("   ")               -> ""      ← 整串都是空白
 *     longestWord("hi")                -> "hi"
 *     longestWord("the quick brown fox") -> "brown"  ← quick 和 brown 都是 5 个字母
 *     longestWord("zebra apple banana")  -> "banana"
 *     longestWord("a\tb  c")           -> "a"     ← 并列取字典序最小，Tab 也算空白
 *
 *   提示：把 string 当成输入流来读，就是 istringstream 的用途：
 *         istringstream iss(line); while (iss >> word) { ... }
 *         它和 cin >> 的切分规则一模一样（跳过任意空白，读到下一个空白为止），
 *         所以空格、Tab、换行全都算分隔符，不用你手工处理。
 *         字典序直接用 word < best，别自己写逐字符循环。
 * ------------------------------------------------------------------------- */
string longestWord(const string& line)
{
    /* TODO 在这里写实现 */
    return "";
}

/* ---------------------------------------------------------------------------
 * TODO 4：忽略大小写和标点的回文判断
 *
 *   规则：只保留数字和英文字母，统一转小写，再判断是不是回文。
 *
 *   期望：
 *     isPalindromeLoose("")                          -> true   ← 空串算回文
 *     isPalindromeLoose("a")                         -> true
 *     isPalindromeLoose("A man, a plan, a canal: Panama") -> true
 *     isPalindromeLoose("Was it a car or a cat I saw?")   -> true
 *     isPalindromeLoose("race a car")                -> false
 *     isPalindromeLoose("12321")                     -> true
 *     isPalindromeLoose("0P")                        -> false  ← '0' 和 'p' 不相等
 *
 *   提示：先「清洗」出一个新串 t，再双指针比首尾，比边清洗边比不容易写错。
 *         大写转小写写 char(c - 'A' + 'a')，或者对整个串用 tolower——
 *         用后者记得先 (unsigned char) 转一道，理由和任务 1 的 isdigit 相同。
 *         清洗完再判长度，别在空串上算 t.size() - 1（无符号回绕，ex05 任务 6 刚讲过）。
 * ------------------------------------------------------------------------- */
bool isPalindromeLoose(const string& s)
{
    /* TODO 在这里写实现 */
    return false;
}

/* ---------------------------------------------------------------------------
 * TODO 5：凯撒密码（大小写各自环绕，负偏移也要对）
 *
 *   规则：只移动英文字母，大小写保持不变，其它字符原样留下。
 *
 *   期望：
 *     caesar("abc", 1)            -> "bcd"
 *     caesar("xyz", 3)            -> "abc"     ← 要绕回头
 *     caesar("XyZ", 3)            -> "AbC"
 *     caesar("a", -1)             -> "z"       ← 负偏移
 *     caesar("abc", -2)           -> "yza"
 *     caesar("Hello, World!", 13) -> "Uryyb, Jbeyq!"
 *     caesar("z", 26)             -> "z"
 *     caesar("a", 100)            -> "w"
 *     caesar("", 5)               -> ""
 *
 *   提示：先 shift = ((k % 26) + 26) % 26 把偏移折到 [0, 26)。
 *         为什么要 +26 再 %26？（想想 C++ 里 -1 % 26 等于几——余数跟着被除数的符号）
 *         然后对大写和小写分别做 'a' + (c - 'a' + shift) % 26。
 *         拼接用 res += 或者 push_back，别写 res = res + c（每轮拷一整条新串）。
 * ------------------------------------------------------------------------- */
string caesar(const string& s, int k)
{
    /* TODO 在这里写实现 */
    return "";
}

/* ---------------------------------------------------------------------------
 * TODO 6：把一行 CSV 按逗号切开，空字段必须保留
 *
 *   期望：
 *     splitCsv("")        -> {""}         ← 空行也是「一个空字段」
 *     splitCsv("solo")    -> {"solo"}
 *     splitCsv("a,b,c")   -> {"a", "b", "c"}
 *     splitCsv("a,,b")    -> {"a", "", "b"}
 *     splitCsv("a,")      -> {"a", ""}    ← 行尾那个空字段最容易被吃掉
 *     splitCsv(",a")      -> {"", "a"}
 *     splitCsv("x,,,y")   -> {"x", "", "", "y"}
 *
 *   提示：用 find + substr 手写最稳：substr 第二个参数是**长度**不是终点下标，
 *         所以是 substr(start, p - start)。这一条是 string 里最高频的错误。
 *         看起来更简单的写法是 getline(iss, field, ',')，但它在
 *         "a," 这种结尾空字段上会漏掉最后一项（流已经 eof，那次读会失败）。
 *         本题要求手写 find 版本，顺便把这个差别记住。
 * ------------------------------------------------------------------------- */
vector<string> splitCsv(const string& line)
{
    /* TODO 在这里写实现 */
    return {};
}

int main()
{
    cout << "========== 任务 1：抠数字求和 ==========\n";
    assert(sumOfNumbers("") == 0);
    assert(sumOfNumbers("abc") == 0);
    assert(sumOfNumbers("a1b2") == 3);
    assert(sumOfNumbers("x12y3") == 15);
    assert(sumOfNumbers("007") == 7);
    assert(sumOfNumbers("-5a3") == 8);
    assert(sumOfNumbers("100500") == 100500);
    assert(sumOfNumbers("a1") == 1);
    assert(sumOfNumbers("2024年11月21日") == 2024 + 11 + 21);
    cout << "  sumOfNumbers 全部通过\n";

    cout << "\n========== 任务 2：不重叠计数 ==========\n";
    assert(countNonOverlap("aaaa", "aa") == 2);
    assert(countNonOverlap("aaa", "a") == 3);
    assert(countNonOverlap("abcabcabc", "abc") == 3);
    assert(countNonOverlap("hello", "z") == 0);
    assert(countNonOverlap("", "a") == 0);
    assert(countNonOverlap("aaaa", "") == 0);
    cout << "  countNonOverlap 全部通过\n";

    cout << "\n========== 任务 3：最长单词 ==========\n";
    assert(longestWord("") == "");
    assert(longestWord("   ") == "");
    assert(longestWord("hi") == "hi");
    assert(longestWord("the quick brown fox") == "brown");
    assert(longestWord("zebra apple banana") == "banana");
    assert(longestWord("a\tb  c") == "a");
    cout << "  longestWord 全部通过\n";

    cout << "\n========== 任务 4：宽松回文 ==========\n";
    assert(isPalindromeLoose("") == true);
    assert(isPalindromeLoose("a") == true);
    assert(isPalindromeLoose("A man, a plan, a canal: Panama") == true);
    assert(isPalindromeLoose("Was it a car or a cat I saw?") == true);
    assert(isPalindromeLoose("race a car") == false);
    assert(isPalindromeLoose("12321") == true);
    assert(isPalindromeLoose("0P") == false);
    assert(isPalindromeLoose("ab") == false);
    cout << "  isPalindromeLoose 全部通过\n";

    cout << "\n========== 任务 5：凯撒密码 ==========\n";
    assert(caesar("abc", 1) == "bcd");
    assert(caesar("xyz", 3) == "abc");
    assert(caesar("XyZ", 3) == "AbC");
    assert(caesar("a", -1) == "z");
    assert(caesar("abc", -2) == "yza");
    assert(caesar("Hello, World!", 13) == "Uryyb, Jbeyq!");
    assert(caesar("z", 26) == "z");
    assert(caesar("a", 100) == "w");
    assert(caesar("", 5) == "");
    cout << "  caesar 全部通过\n";

    cout << "\n========== 任务 6：CSV 切分 ==========\n";
    assert(splitCsv("") == vector<string>({""}));
    assert(splitCsv("solo") == vector<string>({"solo"}));
    assert(splitCsv("a,b,c") == vector<string>({"a", "b", "c"}));
    assert(splitCsv("a,,b") == vector<string>({"a", "", "b"}));
    assert(splitCsv("a,") == vector<string>({"a", ""}));
    assert(splitCsv(",a") == vector<string>({"", "a"}));
    assert(splitCsv("x,,,y") == vector<string>({"x", "", "", "y"}));
    cout << "  splitCsv 全部通过\n";

    cout << "\n========== 观察题：不改代码，先猜再跑 ==========\n";
    string s = "abcdef";
    cout << "  \"abcdef\".substr(1, 3) = [" << s.substr(1, 3)
         << "]   ← 起点 1、长度 3，不是 [1, 3)\n";
    cout << "  \"abcdef\".substr(2)    = [" << s.substr(2) << "]   ← 省略长度就取到结尾\n";
    cout << "  find 找不到返回 npos（值 = " << string::npos
         << "），判据只能写 s.find(x) != string::npos，拿它跟 -1 比是碰运气\n";

    cout << "  字典序是逐字符比 ASCII 码：string(\"Z\") < string(\"a\") 是 "
         << (string("Z") < string("a"))
         << "，string(\"ab\") < string(\"abc\") 是 " << (string("ab") < string("abc"))
         << "，string(\"B10\") < string(\"B9\") 是 " << (string("B10") < string("B9")) << "\n";
    /*  上面每一项都必须套一层 string。
     *  直接写 "Z" < "a" 比的是两个字符数组的**地址**，结果由编译器随便安排，
     *  -Wall 也会跳出来骂你一句 comparison between two arrays。
     */
    cout << "  比较的两侧都得是 string 对象；\"Z\" < \"a\" 这种写法比的是地址，不是内容。\n";

    string zh = "中文";
    cout << "  \"中文\".size() = " << zh.size()
         << "（UTF-8 下一个汉字 3 字节）——size() 是字节数不是字数，别按下标遍历中文\n";

    cout << "  按行读入（getline 版，行为和你从键盘敲进去完全一样）：\n";
    istringstream iss("first line\nsecond\n\nfourth\n");
    string line;
    int no = 0;
    while (getline(iss, line)) {
        no++;
        cout << "    第 " << no << " 行 [" << line << "] 长度 " << line.size() << "\n";
    }
    cout << "  getline 读到的行**不含换行符**，空行会给你一个空串，所以这里共 4 行。\n";
    /*  从键盘读的时候还有一个必翻车点：
     *      int n;  cin >> n;  getline(cin, s);      // s 是空串！
     *  cin >> n 只吃掉数字，那个换行符还留在缓冲区，getline 一读就读到空行。
     *  中间插一句 cin.ignore(); 把换行吃掉再读。先读个数再读 n 行的题必写。
     */
    cout << "  cin >> n 之后紧跟 getline 一定先写 cin.ignore()，否则读到空串。\n";

    cout << "  stoi 越界会抛异常，不是悄悄给个错值：\n";
    try {
        cout << "    stoi(\"123\") = " << stoi("123") << "\n";
        int huge = stoi("99999999999999999999");       // 这一句当场抛出来
        cout << "    这一行走不到：" << huge << "\n";
    } catch (const out_of_range&) {
        cout << "    stoi(\"99999999999999999999\") 抛 out_of_range。\n"
             << "    超过 int 的该用 stoll，或者照 ex01 的 toInt 逐位累加到 long long。\n";
    }
    /*  拼接效率：
     *      res = res + c;   // 每一轮都新建一条临时串，n 次下来是 O(n^2)
     *      res += c;        // 往自己的尾部写，均摊 O(1)
     *  已经知道最终长度时再补一句 res.reserve(n)，一次分配都不多。
     */
    cout << "  拼接一律用 += / push_back；res = res + c 每轮拷一整条串，是 O(n^2)。\n";

    cout << "\n>>> ex06 全部通过。下一站：ex07_structs_sorting.cpp\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

int sumOfNumbers(const string& s)
{
    int total = 0;
    int cur = 0;
    bool inNum = false;
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            cur = cur * 10 + (c - '0');
            inNum = true;
        } else if (inNum) {
            total += cur;
            cur = 0;
            inNum = false;
        }
    }
    if (inNum) total += cur;        // 结尾还挂着没结算的一段
    return total;
}

int countNonOverlap(const string& text, const string& pat)
{
    if (pat.empty()) return 0;      // 不挡掉这里就是死循环
    int cnt = 0;
    size_t pos = 0;
    while ((pos = text.find(pat, pos)) != string::npos) {
        cnt++;
        pos += pat.size();          // 写 pos + 1 就变成「重叠计数」
    }
    return cnt;
}

string longestWord(const string& line)
{
    istringstream iss(line);
    string word, best;
    while (iss >> word) {
        if (word.size() > best.size()) best = word;
        else if (word.size() == best.size() && word < best) best = word;
    }
    return best;
}

bool isPalindromeLoose(const string& s)
{
    string t;
    for (char c : s) {
        if (c >= '0' && c <= '9') t += c;
        else if (c >= 'a' && c <= 'z') t += c;
        else if (c >= 'A' && c <= 'Z') t += char(c - 'A' + 'a');
    }
    for (int i = 0, j = (int)t.size() - 1; i < j; i++, j--) {
        if (t[i] != t[j]) return false;
    }
    return true;
}

string caesar(const string& s, int k)
{
    string res;
    res.reserve(s.size());
    int shift = ((k % 26) + 26) % 26;           // 折到 [0, 26)，负偏移也够用
    for (char c : s) {
        if (c >= 'a' && c <= 'z') res += char('a' + (c - 'a' + shift) % 26);
        else if (c >= 'A' && c <= 'Z') res += char('A' + (c - 'A' + shift) % 26);
        else res += c;
    }
    return res;
}

vector<string> splitCsv(const string& line)
{
    vector<string> out;
    size_t start = 0;
    while (true) {
        size_t p = line.find(',', start);
        if (p == string::npos) {
            out.push_back(line.substr(start));  // 最后一段，没有逗号收尾
            break;
        }
        out.push_back(line.substr(start, p - start));   // 第二参数是长度
        start = p + 1;
    }
    return out;
}

  为什么「攒 cur + 分隔符结算」要比「先 find 再 substr」好用：
    前者只扫一遍字符串，O(n)，而且「结尾那段要不要结算」这种边界只剩一个 if。
    后者要不断移动下标、判断 npos、算 substr 的长度，三个地方都能写错。
    凡是「从一串文本里按某个规则切出若干段」的题，都先想这个套路。

  为什么空模式必须在函数第一行挡掉：
    text.find("", pos) 对任何 pos <= size() 都返回 pos 本身，也就是「原地命中」，
    于是 cnt 一直加、pos 一直不动，循环永远出不来。
    标准库不会替你校验参数，防御代码写在自己函数第一行，成本一行、收益是一个 RE。

  为什么 substr(1, 3) 得到 "bcd"：
    原型是 substr(起点, 长度)。这个约定和 STL 到处用的「起点 + 长度」是一对
    （compare、erase、insert 也一样），跟「[起点, 终点)」那种区间写法是两套。
    记法：想要 [1, 4) 就写 substr(1, 4 - 1)。想取到结尾就省掉第二个参数。

  为什么 caesar 里要先 ((k % 26) + 26) % 26：
    C++ 的 % 结果符号跟着**被除数**，-1 % 26 == -1，不是 25。
    直接拿它去加，字符会往前跑出字母表，(c - 'a' + shift) 变成负数再 % 26 还是负的，
    最后 char('a' + 负数) 就是乱码。先加一个 26 再取模，把偏移统一搬到 [0, 26)，
    之后所有分支只用一种逻辑——这是处理「环形下标」的通用写法。

  为什么 splitCsv 不用 getline(iss, f, ',')：
    那个写法在 "a," 上只会返回一个 "a"：读完 "a" 之后流继续找下一个逗号，
    一路撞到 EOF，这次 getline 直接置失败位，结尾那个空字段就被吞了。
    题目要求「空字段全部保留」时，手工 find + substr 才是对的。
    两种写法都对的场景也存在（字段一定非空时），但要清楚差在哪，别当成等价。
 * ========================================================================== */
