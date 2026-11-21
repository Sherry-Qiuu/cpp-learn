/* ============================================================================
 * report.cpp —— 排序、名次、排版的具体实现
 * ----------------------------------------------------------------------------
 * 这个文件负责什么：
 *   把 report.h 里那三条规则（谁在前、并列怎么算、每行多宽）落地。
 *   它不定义任何新的业务概念，也不碰 Student 的字段含义。
 *
 * 为什么这样拆：
 *   这个文件是项目里唯一 include <iostream> / <iomanip> / <sstream> 的地方。
 *   「输出」这件事集中在一处，将来要改成交互式（从 cin 读数据）或者写文件，
 *   只需要在这里动刀，student.* 一行都不用改 —— 这叫依赖方向单一。
 *
 * 打印约定（notes/14 强调过）：
 *   行尾一个多余空格都不许有。做法很简单：行尾那一列用右对齐（setw 把空格
 *   补在数字左边），谁也不要拿左对齐的列当最后一列。main.cpp 里有一条
 *   assert 专门把每一行翻出来检查这一点。
 * ========================================================================== */

#include "report.h"

#include <algorithm>        /* sort */
#include <iomanip>          /* setw / setfill / fixed / setprecision */
#include <iostream>         /* cout */
#include <sstream>          /* ostringstream：把一行拼成字符串再交出去 */
#include <string>
#include <vector>

/* ---------------------------------------------------------------------------
 * 比较函数：三个关键字，「逐层 if，最后一个 return」。
 * 每一层只回答一件事 —— 这一项分出了胜负吗？平局才把决定权交给下一层。
 * 三个关键字写两个 if，最后一个关键字直接 return（notes/08 第 3 节的模板）。
 *
 * 全程只出现 < 和 >，一次 <= 都没有。写成 ta >= tb 的话 beforeInReport(a, a)
 * 会返回 true，等于告诉 sort「a 该排在 a 自己前面」，严格弱序被破坏，
 * sort 内部循环停不下来 —— 小数据看着正常，大数据直接 RE。
 * main.cpp 里 assert(!beforeInReport(x, x)) 就是把这条铁律钉成可执行检查。
 * ------------------------------------------------------------------------- */
bool beforeInReport(const Student& a, const Student& b)
{
    const int ta = totalOf(a);
    const int tb = totalOf(b);
    if (ta != tb) return ta > tb;         // 第一关键字：总分降序
    if (a.name != b.name) return a.name < b.name;   // 第二关键字：姓名字典升序
    return a.id < b.id;                   // 第三关键字：学号升序（两个 Kevin 靠它分开）
}

void sortForReport(std::vector<Student>& v)
{
    /* 比较器直接传函数名 = 传函数指针，编译器自己会把它内联掉。
     * 这里排的是对象本身（8 个人，无所谓）。表大、对象又大、又要反复换规则排序时，
     * 改用 notes/08 第 5 节的「排下标」：只搬 int，不搬 string 和 vector。 */
    sort(v.begin(), v.end(), beforeInReport);
}

/* ---------------------------------------------------------------------------
 * 名次：总分相同就并列。
 * 规则只有一句 —— 位置变了就取「位置号」，位置和上一个人同分就沿用上一个人的名次。
 * 于是 1、2、2 之后下一个是 4（不是 3），这正是竞赛/考试里「并列跳名」的算法。
 *
 * 注意「同分」比的是总分，不是整个对象：两个 Kevin 分数分布完全不同，
 * 但总分一样，就该并列第 5。名次回答的是「你排第几」，不是「你是谁」。
 * ------------------------------------------------------------------------- */
std::vector<int> computeRanks(const std::vector<Student>& sorted)
{
    std::vector<int> ranks(sorted.size(), 0);
    for (int i = 0; i < (int)ranks.size(); i++) {
        if (i > 0 && totalOf(sorted[i]) == totalOf(sorted[i - 1]))
            ranks[i] = ranks[i - 1];      // 与上一个人同分：并列，名次不前进
        else
            ranks[i] = i + 1;             // 分高了：名次 = 下标 + 1（1-based）
    }
    return ranks;
}

/* ---------------------------------------------------------------------------
 * 逐科统计：外层按「人」遍历、内层按「科」累加。
 *
 * 为什么按行遍历（而不是「外层科目、内层学生」）？
 *   vector<Student> 在内存里是一个学生接一个学生排着（notes/07 第 2 节），
 *   按行走就是顺序扫内存，CPU 缓存一条直线命中；按列走每次都要跳过一整行，
 *   等于在内存里来回折返跑。这里只有 8 行，差别测不出来，
 *   但「一次行遍历、把 kSubjectCount 个累加器摊在一个数组上」这个写法本身
 *   就是标准答案：只扫一遍数据，而且天然支持「以后加一列统计」。
 *   （反过来按列扫，数据要读 kSubjectCount 遍。）
 * ------------------------------------------------------------------------- */
std::vector<double> subjectAverages(const std::vector<Student>& v)
{
    std::vector<long long> sum(kSubjectCount, 0);   // 累加器用 long long：值域只会变大
    for (const Student& s : v) {                    // 外层按行：每个学生只读一次
        for (int j = 0; j < (int)s.scores.size() && j < kSubjectCount; j++)
            sum[j] += s.scores[j];                  // 内层：往「第 j 科」的格子里加
    }

    std::vector<double> avg(kSubjectCount, 0.0);
    if (v.empty()) return avg;                      // 空表：0.0，绝不除 0
    for (int j = 0; j < kSubjectCount; j++)
        avg[j] = static_cast<double>(sum[j]) / static_cast<double>(v.size());
    return avg;
}

std::vector<int> subjectMax(const std::vector<Student>& v)
{
    std::vector<int> best(kSubjectCount, -1);       // -1 = 这门课还没有成绩
    for (const Student& s : v)
        for (int j = 0; j < (int)s.scores.size() && j < kSubjectCount; j++)
            if (s.scores[j] > best[j]) best[j] = s.scores[j];
    return best;
}

std::vector<int> subjectMin(const std::vector<Student>& v)
{
    /* 这里不能像 max 那样填 -1：-1 比任何合法分数都小，永远换不掉。
     * 两个办法：填 kScoreMax + 1 当「正无穷」，或者用 -1 当「还没赋值」的标志，
     * 见到第一个值先无条件收下。这里选后者 —— 它不依赖分数上限是 100 这个假设。 */
    std::vector<int> worst(kSubjectCount, -1);
    for (const Student& s : v)
        for (int j = 0; j < (int)s.scores.size() && j < kSubjectCount; j++)
            if (worst[j] < 0 || s.scores[j] < worst[j]) worst[j] = s.scores[j];
    return worst;
}

/* ---------------------------------------------------------------------------
 * 一行表格。
 * setw 只对紧跟的那一项生效（notes/03 第 4 节的表），所以循环里每一格都要写 setw；
 * setfill/left/right 会一直生效，只在开头说一次就够。
 * 最后一格（平均分）是右对齐，空格全补在数字左边 —— 这就是「行尾零空格」的由来。
 * ------------------------------------------------------------------------- */
std::string reportRow(const Student& s, int rank)
{
    std::ostringstream os;
    os << std::right
       << std::setw(kRankWidth) << rank << "  "
       << std::setw(kIdWidth)   << s.id  << "  "
       << std::left  << std::setw(kNameWidth) << s.name << std::right;

    for (int j = 0; j < (int)s.scores.size() && j < kSubjectCount; j++)
        os << " " << std::setw(kScoreWidth) << s.scores[j];

    os << " " << std::setw(kTotalWidth) << totalOf(s)
       << " " << std::fixed << std::setprecision(2)
       << std::setw(kAvgWidth) << avgOf(s);
    return os.str();
}

/* 表头 + 分隔线。列宽全部取自 report.h 里那组常量，表头和表体永远对齐。 */
static void printTableHeader()
{
    std::cout << std::right
              << std::setw(kRankWidth) << "Rank" << "  "
              << std::setw(kIdWidth)   << "ID"   << "  "
              << std::left  << std::setw(kNameWidth) << "Name" << std::right;
    for (int j = 0; j < kSubjectCount; j++)
        std::cout << " " << std::setw(kScoreWidth) << subjectName(j);
    std::cout << " " << std::setw(kTotalWidth) << "Total"
              << " " << std::setw(kAvgWidth)   << "Avg" << "\n";
    std::cout << std::string(72, '-') << "\n";
}

void printReport(const std::vector<Student>& sorted, const std::vector<int>& ranks)
{
    printTableHeader();
    /* ranks 与 sorted 等长是调用方的责任（函数不做检查，这是 C++ 的常规取舍：
     * 每次都检查一遍就是白付时间）。main.cpp 里有一条 assert 守住这个约定。 */
    for (int i = 0; i < (int)sorted.size(); i++)
        std::cout << reportRow(sorted[i], ranks[i]) << "\n";
}

void printSubjectStats(const std::vector<Student>& v)
{
    const std::vector<double> avg = subjectAverages(v);
    const std::vector<int> best = subjectMax(v);
    const std::vector<int> worst = subjectMin(v);

    std::cout << std::string(30, '=') << " 各科统计 " << std::string(30, '=') << "\n";
    for (int j = 0; j < kSubjectCount; j++) {
        std::cout << "  " << std::left << std::setw(kScoreWidth) << subjectName(j)
                  << std::right << " avg " << std::fixed << std::setprecision(2)
                  << std::setw(6) << avg[j]
                  << "  max " << std::setw(3) << best[j]
                  << "  min " << std::setw(3) << worst[j] << "\n";
    }
    /* -1 是「这门课没人有成绩」的哨兵值，直接印出来比藏起来好：
     * 空表跑一遍，输出会明明白白写着 min -1，而不是一个看起来很合理的 0。 */
}
