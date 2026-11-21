/* ============================================================================
 * report.h —— 报表模块的接口：排序规则、名次规则、排版规则
 * ----------------------------------------------------------------------------
 * 这个文件负责什么：
 *   student.h 只管「一个学生有哪些数」，这里管「一张表怎么变成人看得懂的东西」：
 *   谁排前面（比较规则）、并列怎么算名次（统计规则）、
 *   每行长什么样（排版规则）。三件事都不属于数据模型，所以另开一个模块。
 *
 * 为什么这样拆：
 *   改列宽、改标题、甚至把整张表换成 CSV，都应该只动 report.*；
 *   如果把它们写进 student.h，student.cpp 就得 include iostream，
 *   一个纯计算模块当场变成不可复用（嵌入式里等于把驱动和 UI 焊死在一起）。
 *
 * 注意本文件同时暴露了 beforeInReport 和 reportRow 这两个「本来可以只放在 .cpp 里」
 * 的函数 —— 唯一的原因是 main.cpp 要对它们逐条 assert（比较函数自反性、
 * 行尾有没有多余空格）。可测试性压倒「藏起来」的洁癖，这个取舍要心里有数。
 * ========================================================================== */

#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <vector>

#include "student.h"        /* 自己的头文件用引号；而且这里必须包 student.h，
                               因为下面的签名里出现了 Student —— 类型得先有定义。 */

/* ----------------------------------------------------------------------------
 * 表格列宽。写成常量而不是散在代码里的数字（notes/14：常量化，别留魔法数字），
 * 想调宽窄只改一处，reportRow 和表头一定同步 —— 表头和表格错位是多文件项目
 * 里最常见也最蠢的 bug，根源就是两处各写了一份宽度。
 * 宽度用 7 是因为科目名最长是 "Science"/"Chinese"（7 个字符）。
 * 为什么名字不用中文：终端按字节算宽度，一个汉字 3 字节，setw 会算错列宽，
 * 表格立刻歪掉（notes/03 第 3 节）。
 * ------------------------------------------------------------------------- */
constexpr int kRankWidth   = 4;
constexpr int kIdWidth     = 8;
constexpr int kNameWidth   = 8;
constexpr int kScoreWidth  = 7;
constexpr int kTotalWidth  = 7;
constexpr int kAvgWidth    = 7;

/* ---------------------------------------------------------------------------
 * 排序规则：总分降序 -> 同分按姓名字典升序 -> 再同学号升序。
 *
 * 单独声明出来（而不是藏在 sort 的 lambda 里）有两个理由：
 *   1. main.cpp 能直接 assert beforeInReport(x, x) == false，
 *      把「严格弱序」这条铁律钉成可执行的检查（notes/08 第 2 节）。
 *   2. 排序规则和排序动作分开，将来加一条「再按平均分」只改这一个函数。
 * 参数是 const Student&：比较函数在 sort 里被调 n log n 次，
 * 传值等于每次拷两个 string，n 一大就是「本地能跑、交题超时」。
 * ------------------------------------------------------------------------- */
bool beforeInReport(const Student& a, const Student& b);

/* 按上面那条规则原地排序。名字里不写 sort() 是为了不和 std::sort 撞名。 */
void sortForReport(std::vector<Student>& v);

/* 算名次：总分相同就并列（1,1,3 那种）。返回的下标 i 对应 v[i]，长度与 v 相同。
 * 前置条件：v 必须已经按 sortForReport 排好 —— 函数不检查，调用方负责，
 * 所以 main.cpp 里专门有一条 assert 在盯这个约定。 */
std::vector<int> computeRanks(const std::vector<Student>& sorted);

/* 逐科统计：三个函数都返回长度 kSubjectCount 的向量，按科目下标对齐。
 * 空表时：平均分成 0.0，max/min 给 -1 表示「无数据」。 */
std::vector<double> subjectAverages(const std::vector<Student>& v);
std::vector<int> subjectMax(const std::vector<Student>& v);
std::vector<int> subjectMin(const std::vector<Student>& v);

/* 把一行拼成字符串（不含换行）。拆成返回值而不是直接打印，
 * 是为了让 main.cpp 能检查「行尾一个空格都没有」——
 * 打印出去的东西就再也拿回来验了，拼成字符串才审得掉。 */
std::string reportRow(const Student& s, int rank);

/* 打印整张名次表 / 各科统计。ranks 必须与 sorted 等长。 */
void printReport(const std::vector<Student>& sorted, const std::vector<int>& ranks);
void printSubjectStats(const std::vector<Student>& v);

#endif /* REPORT_H */
