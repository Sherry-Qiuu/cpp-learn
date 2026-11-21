/* ============================================================================
 * student.cpp —— 数据模型的实现
 * ----------------------------------------------------------------------------
 * 这个文件负责什么：
 *   把 student.h 里声明的每个函数填上函数体，外加那份内置样例数据。
 *
 * 为什么这样拆：
 *   1. 声明与定义分离，才能分离编译。改一行实现，只有 student.o 需要重编，
 *      report.o / main.o 一个字节都不用动（Makefile 里那三条 [CXX] 就是证据）。
 *   2. 这个文件一行 cout 都没有。数据层一旦混进打印，就没法在别的场合
 *      （写文件、发到串口、塞进 GUI）复用，也没法用 assert 干净地测。
 *   3. 只 include 自己的头文件和标准库，绝不 include report.h ——
 *      数据不知道报表的存在，依赖方向永远是「报表 -> 数据」。
 * ========================================================================== */

#include "student.h"

/* ---------------------------------------------------------------------------
 * 科目名表。文件内的 static = 内部链接，只有 student.cpp 看得见，
 * 不会和别的 .cpp 里的同名符号打架（notes/07 讲过 static 的两个含义）。
 * ------------------------------------------------------------------------- */
static const std::string kSubjectNames[] = {"Chinese", "Math", "English", "Science"};
static const std::string kNoSubject = "??";

const std::string& subjectName(int index)
{
    /* 坏参数不崩溃、也不返回垃圾引用，给一个明确的「查无此科」。
     * 返回 const string& 而不是 string：这个名字表是全局只读的，
     * 每次调用都拷一个新 string 纯属浪费。 */
    if (index < 0 || index >= kSubjectCount) return kNoSubject;
    return kSubjectNames[index];
}

int totalOf(const Student& s)
{
    int sum = 0;
    for (int sc : s.scores) sum += sc;      // 只读遍历，值拷贝一个 int 无所谓
    return sum;
}

double avgOf(const Student& s)
{
    if (s.scores.empty()) return 0.0;       // 先挡掉除 0，再谈平均
    /* 两次显式转换，一个都不能省：totalOf 和 size() 都是整数，
     * 直接相除是整除（7/2 得 3），换成 double 才拿到 3.5。
     * 更麻烦的是别写成 (double)(total / size) —— 那是先整除再转，错得很隐蔽。 */
    return static_cast<double>(totalOf(s)) / static_cast<double>(s.scores.size());
}

int maxScoreOf(const Student& s)
{
    if (s.scores.empty()) return -1;
    int best = s.scores[0];                 // notes/14 的铁律：最值用第一个元素初始化，不用 0
    for (int sc : s.scores)
        if (sc > best) best = sc;
    return best;
}

int minScoreOf(const Student& s)
{
    if (s.scores.empty()) return -1;
    int worst = s.scores[0];                // 同样不能写 0 起步，也不能写 kScoreMax 起步
    for (int sc : s.scores)
        if (sc < worst) worst = sc;         // 全 0 分的数据会教「最值初始化成 0」做人
    return worst;
}

int bestSubjectOf(const Student& s)
{
    if (s.scores.empty()) return -1;
    int bestIdx = 0;
    for (int i = 1; i < (int)s.scores.size(); i++)   // (int) 是消掉 signed/unsigned 比较警告的
        if (s.scores[i] > s.scores[bestIdx]) bestIdx = i;   // 严格 > ：并列时保留更小的下标
    return bestIdx;
}

int failedCount(const Student& s, int passLine)
{
    int cnt = 0;
    for (int sc : s.scores)
        if (sc < passLine) cnt++;           // 恰好等于及格线的不算挂：这就是边界
    return cnt;
}

/* ---------------------------------------------------------------------------
 * 内置样例数据（8 人 x 4 门）。
 *
 * 外层 { ... } 是 vector 的列表初始化，内层 {"名字", 学号, {分数...}}
 * 是 Student 的聚合初始化，顺序必须和结构体里的字段顺序一模一样
 * （notes/07 第 1 节）。调换字段顺序 = 全班数据串列，而且编译器一声不吭。
 *
 * 埋进去的四组测试点：
 *   David  380 单独第一            —— 名次 1
 *   Bob / Fiona 同 352             —— 并列第 2，且要按姓名字典序 Bob 在前
 *   Alice  344                     —— 排在两个第 2 之后，名次是 4 不是 3
 *   Kevin(2024007) / Kevin(2024008)—— 同名同总分，只有第三关键字学号能分开他们
 * -------------------------------------------------------------------------- */
std::vector<Student> sampleStudents()
{
    return {
        {"Alice", 2024001, {88, 92, 79, 85}},
        {"Bob",   2024002, {95, 78, 88, 91}},
        {"Cindy", 2024003, {72, 99, 65, 80}},
        {"David", 2024004, {95, 95, 95, 95}},
        {"Eric",  2024005, {60, 55, 78, 90}},
        {"Fiona", 2024006, {90, 88, 94, 80}},
        {"Kevin", 2024007, {85, 85, 84, 80}},
        {"Kevin", 2024008, {70, 80, 90, 94}},
    };
}
