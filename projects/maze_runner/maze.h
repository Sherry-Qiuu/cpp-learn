/* ============================================================================
 * maze.h —— 迷宫求解器项目的「地图模型」接口
 * ----------------------------------------------------------------------------
 * 这个文件负责什么：
 *   1. 定义一张迷宫长什么样：字符的含义（# 墙 / . 空地 / S 起点 / T 终点）、
 *      行列数，以及「这一格能不能踩」「这个坐标在图里吗」这类只跟地图有关的问题。
 *   2. 声明四张内置样例地图：小而简单 / notes/11 用过的那张 5×7 / 无解 / 三个连通块。
 *   3. 把算出来的路径画回地图上（render），让人一眼看出走的是哪条道。
 *
 * 为什么这样拆：
 *   地图只管「格子是什么」，不管「路怎么走」。搜索算法全在 solver.* 里，
 *   它只能通过 at / passable / inBounds 这几个 const 函数看地图，一个字节都改不了。
 *   哪天你要把 DFS 换成 A*，maze.* 一个字都不用动 ——
 *   这就是 notes/06、notes/07 反复强调的「数据」和「算法」分家，
 *   和 score_report 里 student.* 不知道 report.* 存在是同一件事。
 *
 * 本项目不读键盘、也不读文件（notes/14 的验收要求：make run 必须非交互跑完）。
 * 地图全部写死在 maze.cpp 里；改了样例，main.cpp 那一堆 assert 会当场炸给你看。
 * ========================================================================== */

/* 头文件守卫：宏名跟着文件名走（MAZE_H，别的项目里也叫 HEADER_H 就会撞车）。
 * 原因见 score_report/student.h：#include 是原样把文件贴进来，贴两遍就是
 * 「redefinition of 'struct Maze'」。 */
#ifndef MAZE_H
#define MAZE_H

#include <string>       /* std::string / std::vector：头文件里写全 std::，
                           绝不写 using namespace std;（会污染所有包含者） */
#include <utility>      /* std::pair：坐标就用 pair<int,int>，first=行 second=列 */
#include <vector>

/* ----------------------------------------------------------------------------
 * 格子的字符含义。写成具名常量，代码里就再出现「魔法字符」
 * （notes/14：能命名的东西不要裸写）。solver.cpp 里判断墙就写 kWall，
 * 将来想把墙换成 'X' 只改这一行。
 * ------------------------------------------------------------------------- */
constexpr char kWall   = '#';   // 墙：不能踩
constexpr char kOpen   = '.';   // 空地：能踩
constexpr char kStart  = 'S';   // 起点：能踩
constexpr char kTarget = 'T';   // 终点：能踩
constexpr char kPath   = '*';   // render() 用来画路径的字符（只出现在渲染结果里）

/* ----------------------------------------------------------------------------
 * 一张迷宫。
 *
 * 为什么是 struct 而不是 class：C++ 里两者唯一的差别是默认访问权限，
 * 本仓库统一用 struct（notes/07 通篇如此），别在入门阶段纠结这个。
 *
 * 这里出现了本项目比 notes/07 多走的半步：**构造函数**。
 *   struct Maze { Maze(参数) { ... } }  —— 名字和类型同名、没有返回类型，
 *   作用是「建对象时自动把初始化干完」。好处是对象一出生就是合法的：
 *   rows_/cols_ 一定和 g_ 对齐，S/T 一定已经扫过一遍，
 *   不存在「忘了调用 init 就用」这种状态。嵌入式里叫「不要让全局变量裸奔」。
 *
 * 所有查询函数都挂了 const：承诺不改自己。正因为有这个承诺，
 * solver.cpp 里的参数才能写成 const Maze&（不拷贝、也改不坏）。
 * ------------------------------------------------------------------------- */
struct Maze {
    /* explicit：禁止 vector<string> 偷偷被当成 Maze 用。
     * 少了它，Maze m = {"S.", ".T"}; 这种隐式转换也能编过，
     * 更糟的是函数调用时传错类型也会被静默「凑」成一个 Maze。 */
    explicit Maze(const std::vector<std::string>& rows);

    /* 下面这几个是一行的取值函数，就地写在类里是常规做法；
     * 凡是要遍历整张图的（at 之外的那个构造、countPassable、render）都放 .cpp。 */
    int rows() const { return rows_; }
    int cols() const { return cols_; }

    /* 越界判断。注意三个条件全用 && 串起来，左边假了右边就不算，
     * 所以 rows_/cols_ 拿不到的时候也走不到 g_[r]。 */
    bool inBounds(int r, int c) const
    {
        return r >= 0 && r < rows_ && c >= 0 && c < cols_;
    }

    bool hasStart()  const { return start_.first  >= 0; }
    bool hasTarget() const { return target_.first >= 0; }

    /* 坐标一律 (r, c) = (行, 列)。notes/06 第 4 节那条铁律：
     * 下标顺序永远是 g[行][列]，变量名用 r/c 而不是 x/y，能躲掉一大半越界。 */
    std::pair<int, int> start()  const { return start_; }
    std::pair<int, int> target() const { return target_; }

    /* 取字符；界外一律返回 kWall（见 .cpp 里的解释：界外即墙）。 */
    char at(int r, int c) const;

    /* 能不能踩：界内且不是墙。S / T / . 都算能踩。界外返回 false。 */
    bool passable(int r, int c) const;

    /* 能踩的格子总数（空地 + S + T）。用来给连通块的「每块大小之和」做交叉验证。 */
    int countPassable() const;

    /* 把路径画回地图，返回一张新图（本对象不被改动，所以是 const）。
     * 规则：路径上的空地变成 kPath；S 和 T 原样保留（否则看不出头尾）；
     *       界外坐标和墙一律忽略 —— 传进来的路径再离谱也画不出鬼。
     * 默认参数 {}：不传路径就是「原样复制一张图」。 */
    std::vector<std::string> render(
        const std::vector<std::pair<int, int>>& path = std::vector<std::pair<int, int>>()) const;

private:
    std::vector<std::string> g_;      // 第 r 个 string 就是第 r 行，保证每行等长
    int rows_;
    int cols_;
    std::pair<int, int> start_;       // (-1,-1) 表示「图里没有 S」
    std::pair<int, int> target_;      // (-1,-1) 表示「图里没有 T」
};

/* ---------------------------------------------------------------------------
 * 四张内置样例。为什么用函数而不是全局变量？
 *   头文件里放**变量的定义** = 每个包含它的 .cpp 都生成一份，链接时
 *   multiple definition（score_report 里 subjectName() 同理）。
 *   包一层函数，数据本体藏在 maze.cpp 里，干干净净，每次调用给一张新图。
 *
 *   mazeTiny()    3×4：手就能看出来的一条道，用来核对所有算法的基准值
 *   mazeClassic() 5×7：notes/11 第 3 节 / ex12 实测过的那张，(0,0)->(4,6) 最短 10 步
 *   mazeNoWay()   4×4：被一堵横墙彻底隔开，考「不可达」这条分支有没有写对
 *   mazeIslands() 5×9：三间彼此封死的房，连通块 = 3，S 和 T 关在同一间里
 * ------------------------------------------------------------------------- */
Maze mazeTiny();
Maze mazeClassic();
Maze mazeNoWay();
Maze mazeIslands();

#endif /* MAZE_H */
