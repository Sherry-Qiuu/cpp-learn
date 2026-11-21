/* ============================================================================
 * main.cpp —— 使用方 + 自检（跑内置样例，用 assert 把关键数字钉死）
 * ----------------------------------------------------------------------------
 * 这个文件负责什么：
 *   1. 演示「一个模块怎么被另一个模块使用」：只 include 两个 .h，
 *      完全不知道 sort  inside 用了什么、行是怎么拼出来的。
 *   2. 当测试驱动：每条规则都用 assert 核对一遍，跑通才打印最后一行。
 *
 * 为什么断言写在这里而不是「肉眼看着输出觉得差不多」：
 *   assert 失败会打印 文件:行号 和表达式本身，然后当场 abort，退出码非 0。
 *   这就是本仓库的验收方式 —— make p1 绿了就是真的对了，
 *   靠眼睛数表格列宽是数不出来的。
 *   （注意：assert 里的表达式不能有副作用，因为它在 -DNDEBUG 下会整条消失。
 *     所以本文件一律先把值算进变量，再 assert 那个变量。）
 *
 * 编译运行：
 *   make p1                 （在仓库根目录）
 *   或 cd projects/score_report && make run
 * ========================================================================== */

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "student.h"      /* 自己的头文件：双引号 */
#include "report.h"

using namespace std;

/* 浮点数不用 == 直接比（notes/04：0.1 二进制存不下）。
 * 判「足够接近」一律写成 fabs(a - b) < 1e-9。 */
static bool near(double a, double b)
{
    return fabs(a - b) < 1e-9;
}

/* ===========================================================================
 * 1. 数据模型：总分 / 平均分 / 最值 / 挂科数 + 空输入边界
 * =========================================================================== */
static void test_model()
{
    cout << "\n========== 1. 数据模型 ==========\n";

    vector<Student> cls = sampleStudents();
    assert(cls.size() == 8);
    for (const Student& s : cls)
        assert((int)s.scores.size() == kSubjectCount);      // 数据约定：每人 4 门
    cout << "  样例 8 人，每人 " << kSubjectCount << " 门  OK\n";

    /* cls 的下标就是 sampleStudents() 里的书写顺序：0 Alice 1 Bob 2 Cindy 3 David
     * 4 Eric 5 Fiona 6 Kevin(2024007) 7 Kevin(2024008) */
    assert(totalOf(cls[3]) == 380);                 // David 4 x 95
    assert(totalOf(cls[1]) == 352);                 // Bob
    assert(totalOf(cls[4]) == 283);                 // Eric（最低分那位）
    assert(totalOf(cls[0]) == 344);
    assert(totalOf(cls[6]) == 334 && totalOf(cls[7]) == 334);   // 两个 Kevin 同总分
    assert(cls[6].name == cls[7].name && cls[6].id < cls[7].id);// 同名不同学号，第三关键字有的测
    cout << "  总分、并列埋点  OK\n";

    /* 两种口径算同一个总量，必须相等：这是最便宜也最有效的一招自检。
     * 「按人加总」和「按科目加总」走的是完全不同的循环。 */
    long long byStudent = 0;
    for (const Student& s : cls) byStudent += totalOf(s);
    assert(byStudent == 2695);
    long long bySubject = 0;
    for (int j = 0; j < kSubjectCount; j++) {
        long long col = 0;
        for (const Student& s : cls) col += s.scores[j];
        bySubject += col;
    }
    assert(bySubject == byStudent);
    cout << "  全班的总分 = 2695，按人算与按科算一致  OK\n";

    assert(near(avgOf(cls[3]), 95.0));
    assert(near(avgOf(cls[6]), 83.5));              // 334 / 4，不是 83
    assert(near(avgOf(cls[4]), 70.75));             // 283 / 4：整数除法会把这里砍成 70
    assert(maxScoreOf(cls[2]) == 99);               // Cindy 的数学
    assert(minScoreOf(cls[4]) == 55);               // Eric 的数学
    assert(bestSubjectOf(cls[0]) == 1);             // Alice 最高分在第 1 科（92）
    assert(bestSubjectOf(cls[3]) == 0);             // 四科全 95：并列取最小下标
    assert(bestSubjectOf(cls[4]) == 3);             // Eric 最高是科学 90
    cout << "  平均分（含 334/4 这种非整数）、最高分科目  OK\n";

    assert(failedCount(cls[4], 60) == 1);           // Eric 只有 55 一门低于 60
    assert(failedCount(cls[0], 60) == 0);
    Student edge{"Edge", 1, {59, 60, 100, 0}};
    assert(failedCount(edge, 60) == 2);             // 59 和 0 算挂，60 恰好及格不算
    cout << "  挂科数：恰好等于及格线不算挂  OK\n";

    /* 空数据：每个函数都必须给出一个「明确的答案」而不是崩溃或垃圾值。 */
    Student none{"Nobody", 0, {}};
    assert(totalOf(none) == 0);
    assert(near(avgOf(none), 0.0));                 // 除数为 0 的情况在函数里挡掉了
    assert(maxScoreOf(none) == -1);
    assert(minScoreOf(none) == -1);                 // 用 -1 表示「无数据」，不是 0
    assert(bestSubjectOf(none) == -1);
    assert(failedCount(none, 60) == 0);
    assert(subjectName(0) == "Chinese");
    assert(subjectName(kSubjectCount - 1) == "Science");
    assert(subjectName(99) == "??");                // 坏下标也不能越界读
    cout << "  空成绩 / 坏科目下标都有明确返回  OK\n";
}

/* ===========================================================================
 * 2. 比较函数：严格弱序 + 「全员同分」这个必错边界
 * =========================================================================== */
static void test_comparator()
{
    cout << "\n========== 2. 比较规则 ==========\n";

    vector<Student> cls = sampleStudents();

    /* 自己不能排在自己前面 —— 违反这条，sort 在大数据上直接 RE。 */
    for (const Student& s : cls)
        assert(!beforeInReport(s, s));
    cout << "  cmp(x, x) 恒为 false（严格弱序）  OK\n";

    /* 全员同分：这时候排序完全由第二、第三关键字决定，
     * 是「只写了总分比较」的代码最容易翻车的一组数据（notes/08 第 8 节）。 */
    vector<Student> same = {
        {"Zoe",  3, {60, 60, 60, 60}},
        {"Amy",  1, {90, 30, 60, 60}},
        {"Bob",  2, {40, 80, 60, 60}},
    };
    sortForReport(same);
    assert(same[0].name == "Amy" && same[1].name == "Bob" && same[2].name == "Zoe");
    assert(totalOf(same[0]) == 240 && totalOf(same[2]) == 240);
    cout << "  同分 240 的三人按名字典序：Amy, Bob, Zoe  OK\n";

    /* 同名同分：只有学号能把他们分开，且必须升序。 */
    vector<Student> twin = {
        {"Kevin", 2024008, {70, 80, 90, 94}},
        {"Kevin", 2024007, {85, 85, 84, 80}},
    };
    sortForReport(twin);
    assert(twin[0].id == 2024007 && twin[1].id == 2024008);
    cout << "  同名同分按学号升序  OK\n";

    /* 空表排序不能崩，也不能写出任何东西。 */
    vector<Student> empty;
    sortForReport(empty);
    assert(empty.empty());
    assert(beforeInReport(cls[0], cls[3]) == false);    // Alice 344 不该排在 David 380 前
    assert(beforeInReport(cls[3], cls[0]) == true);
    cout << "  空表安全；David(380) 排在 Alice(344) 之前  OK\n";
}

/* ===========================================================================
 * 3. 名次：并列 + 跳名
 * =========================================================================== */
static void test_ranks()
{
    cout << "\n========== 3. 名次 ==========\n";

    vector<Student> three = {
        {"Ann",  1, {60, 60, 60, 60}},
        {"Bob",  2, {80, 40, 60, 60}},
        {"Cathy",3, {70, 70, 50, 50}},     // 240，和上面两个同分
        {"Dan",  4, {50, 50, 50, 50}},     // 200，被三个 240 顶到第 4 名
    };
    sortForReport(three);
    vector<int> r = computeRanks(three);
    assert(r.size() == 4);
    assert(r[0] == 1 && r[1] == 1 && r[2] == 1);      // 三个人并列第 1
    assert(r[3] == 4);                                 // 下一个是第 4，不是第 2
    cout << "  三人同分 -> 名次 1,1,1,4  OK\n";

    vector<Student> one = {{"Solo", 9, {100, 100, 100, 100}}};
    vector<int> r1 = computeRanks(one);
    assert(r1.size() == 1 && r1[0] == 1);
    vector<Student> none;
    vector<int> r0 = computeRanks(none);
    assert(r0.empty());
    cout << "  单人 -> 1；空表 -> 空  OK\n";
}

/* ===========================================================================
 * 4. 在真实样例上跑完整流程：顺序、名次、统计、排版 全部 assert，
 *    然后把表打出来给你看。
 * =========================================================================== */
static void test_full_report()
{
    cout << "\n========== 4. 完整报表 ==========\n";

    vector<Student> cls = sampleStudents();
    sortForReport(cls);

    const char* wantName[] = {"David", "Bob", "Fiona", "Alice",
                              "Kevin", "Kevin", "Cindy", "Eric"};
    const int   wantId[]   = {2024004, 2024002, 2024006, 2024001,
                              2024007, 2024008, 2024003, 2024005};
    const int   wantRank[] = {1, 2, 2, 4, 5, 5, 7, 8};
    for (int i = 0; i < (int)cls.size(); i++)
        assert(cls[i].name == wantName[i] && cls[i].id == wantId[i]);   // 8 项逐条比
    cout << "  排序结果与预期名单逐一相符  OK\n";

    vector<int> ranks = computeRanks(cls);
    assert(ranks.size() == cls.size());               // printReport 的前置条件
    for (int i = 0; i < (int)ranks.size(); i++) {
        assert(ranks[i] == wantRank[i]);
        if (i > 0) {
            assert(ranks[i] >= ranks[i - 1]);          // 名次只可能不变或前进
            assert(ranks[i] <= i + 1);                 // 不可能比自己所处的位置更差
            if (totalOf(cls[i]) == totalOf(cls[i - 1]))
                assert(ranks[i] == ranks[i - 1]);      // 同分必并列
            else
                assert(ranks[i] == i + 1);             // 分高了就落到「位置号」上 -> 2,2 之后是 4
        }
    }
    cout << "  名次 1,2,2,4,5,5,7,8（并列跳名）  OK\n";

    /* 排好序之后必须「相邻同分」，否则说明比较器和名次规则对不上。 */
    for (int i = 1; i < (int)cls.size(); i++)
        assert(totalOf(cls[i]) <= totalOf(cls[i - 1]));
    vector<Student> backup = sampleStudents();
    sort(backup.begin(), backup.end(), [](const Student& a, const Student& b) {
        return a.id < b.id;                       // 换一套规则：按学号排
    });
    assert(backup.front().id == 2024001 && backup.back().id == 2024008);
    cout << "  同一份数据换规则重排也正常（排的是副本）  OK\n";

    vector<double> avg = subjectAverages(cls);        // 与顺序无关，但照样核一遍
    assert(avg.size() == (size_t)kSubjectCount);
    assert(near(avg[0], 81.875));                   // Chinese 655 / 8
    assert(near(avg[1], 84.0));                     // Math    672 / 8
    assert(near(avg[2], 84.125));                   // English 673 / 8
    assert(near(avg[3], 86.875));                   // Science 695 / 8
    double sumAvg = 0.0;
    for (double x : avg) sumAvg += x;
    assert(near(sumAvg * (double)cls.size(), 2695.0));   // 又一次两条路径互相验证
    cout << "  各科平均分 81.875 / 84 / 84.125 / 86.875  OK\n";

    vector<int> best = subjectMax(cls);
    vector<int> worst = subjectMin(cls);
    assert(best[0] == 95 && best[1] == 99 && best[2] == 95 && best[3] == 95);
    assert(worst[0] == 60 && worst[1] == 55 && worst[2] == 65 && worst[3] == 80);
    for (int j = 0; j < kSubjectCount; j++) assert(worst[j] <= best[j]);

    vector<Student> none;
    vector<double> eavg = subjectAverages(none);
    vector<int> ebest = subjectMax(none), eworst = subjectMin(none);
    for (int j = 0; j < kSubjectCount; j++)
        assert(near(eavg[j], 0.0) && ebest[j] == -1 && eworst[j] == -1);
    cout << "  各科最高/最低分；空表统计返回 0 与 -1  OK\n";

    /* 排版：每行等长（列对齐）+ 行尾零空格（notes/14 的提交清单第 7 条）。 */
    size_t rowLen = 0;
    for (int i = 0; i < (int)cls.size(); i++) {
        string line = reportRow(cls[i], ranks[i]);
        assert(!line.empty());
        assert(line[line.size() - 1] != ' ');           // 最后一个字符不能是空格
        if (i == 0) rowLen = line.size();
        else assert(line.size() == rowLen);            // 名字长度不同也要对齐到同一列
    }
    cout << "  8 行等长（" << rowLen << " 字符）且行尾无空格  OK\n";

    cout << "\n";
    printReport(cls, ranks);
    cout << "\n";
    printSubjectStats(cls);
}

int main()
{
    cout << "score_report —— 学生成绩报表（多文件 / 分离编译）\n";

    test_model();
    test_comparator();
    test_ranks();
    test_full_report();

    /* 走到这一行，说明上面每一条 assert 都过了。断言失败根本到不了这里，
     * 屏幕上会留下 assert 打印的 文件:行号:《表达式》 —— 那就是验收信号。 */
    cout << "\n>>> score_report 全部通过\n";
    return 0;
}
