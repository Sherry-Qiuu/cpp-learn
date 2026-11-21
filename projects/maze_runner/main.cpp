/* ============================================================================
 * main.cpp —— 使用方 + 自检：跑四张内置迷宫，把每个关键数字用 assert 钉死
 * ----------------------------------------------------------------------------
 * 这个文件负责什么：
 *   1. 演示「算法模块怎么被使用」：只 include 两个 .h，
 *      完全不知道 BFS 里面是 queue 还是 vector 当队列。
 *   2. 当测试驱动：可达性、步数、路径的每一格、连通块大小，逐条 assert。
 *   3. 打印人类看得懂的结果：地图、画回地图的最短路径、五个迷宫的汇总表。
 *
 * 为什么断言写在这里而不是「肉眼看着输出觉得差不多」：
 *   「BFS 找到 10 步」这种话，肉眼只能看出「大概是个数」。
 *   而本项目真正要防的错都很阴：路径里少pop一次就变成一串访问顺序、
 *   出队才标记就重复入队、连通块把墙也算成一块 —— 三种错都不会让输出「看起来怪」，
 *   但每一条 assert 都会当场停住，并打印 文件:行号:《表达式》。
 *   （规矩同 score_report：assert 里的表达式不能有副作用，
 *     加 -DNDEBUG 时整条语句会消失。所以一律先算进变量，再 assert 那个变量。）
 *
 * 编译运行：
 *   make p2                 （在仓库根目录）
 *   或 cd projects/maze_runner && make run
 * ========================================================================== */

#include <cassert>
#include <iomanip>      /* setw：汇总表那几列对齐 */
#include <iostream>
#include <string>
#include <utility>      /* pair */
#include <vector>

#include "maze.h"       /* 自己的头文件：双引号 */
#include "solver.h"

using namespace std;

/* 坐标比相等。pair 自带 ==，但写成函数读起来顺一点：cellEq(a, b)。
 * 为什么不直接写 a == b？因为 assert 失败时打印的表达式里，
 * "d.path.front() == make_pair(0, 0)" 比 "cellEq(...)" 更难一眼看懂吗？
 * 反过来说：make_pair 写起来更长。这里选函数，纯粹为了少打字。 */
static bool cellEq(pair<int, int> a, pair<int, int> b)
{
    return a.first == b.first && a.second == b.second;
}

/* ---------------------------------------------------------------------------
 * 本项目最有价值的一个函数：拿到一条路径，一格一格验收它到底合不合法。
 *
 * 它检查四件事，每件都对应一种真实会犯的错：
 *   ① 非空          —— 忘了 push_back，返回一条空路径也敢说自己走到了；
 *   ② 每格都在图里  —— 前驱表回退时写错方向，跑出地图边界；
 *   ③ 每格都能踩    —— **穿墙**。路径从墙格里「抄近道」，步数看着还挺短，
 *                       这是迷宫题最典型的假 AC；
 *   ④ 相邻两格是四邻 —— 斜着穿、或者一次跳两格。
 *      判据是 dr*dr + dc*dc == 1：只有 (±1,0) 和 (0,±1) 满足；
 *      斜向 (±1,±1) 会得到 2，跳格 (±2,0) 会得到 4，全部拒掉。
 *      （哪天做 8 邻域的题，把这里改成 <= 2 就是全部工作量 —— 见 README 任务 1。）
 *
 * 为什么这个函数值得单独写：可达性和步数都是「一个数字」，错了也就是错一个数；
 * 路径是「一串格子」，它有 5 种不同的坏法。数字对不对要靠 assert 的期望值，
 * 而路径合不合法可以**无条件**检查 —— 不管哪张图、不管哪条路，规则都一样。
 * 这类「不依赖期望值的性质检查」是搜索题里最值得抄进模板的东西。
 * ------------------------------------------------------------------------- */
static bool pathIsLegal(const Maze& m, const vector<pair<int, int>>& path)
{
    if (path.empty()) return false;

    for (size_t i = 0; i < path.size(); i++) {
        const int r = path[i].first;
        const int c = path[i].second;
        if (!m.inBounds(r, c)) return false;       /* ② 界外 */
        if (!m.passable(r, c)) return false;       /* ③ 穿墙 / 踩到界外 */
        if (i == 0) continue;
        const int dr = r - path[i - 1].first;
        const int dc = c - path[i - 1].second;
        if (dr * dr + dc * dc != 1) return false;  /* ④ 必须走一格、只能四方向 */
    }
    return true;
}

/* 路径上的格子有没有重复。最短路径不可能绕圈：一旦回到同一个格子，
 * 把中间那段剪掉会更短。这条性质和 pathIsLegal 合起来，
 * 基本就把「假最短路」全堵住了。 */
static bool pathHasNoRepeat(const vector<pair<int, int>>& path)
{
    for (size_t i = 0; i < path.size(); i++)
        for (size_t j = i + 1; j < path.size(); j++)
            if (path[i] == path[j]) return false;
    return true;
}

/* dist 里被 BFS 走到（!= -1）的格子数。用它去和 DFS 的 visitedCells 对账。 */
static int countReached(const vector<vector<int>>& dist)
{
    int n = 0;
    for (size_t r = 0; r < dist.size(); r++)
        for (size_t c = 0; c < dist[r].size(); c++)
            if (dist[r][c] != -1) n++;
    return n;
}

/* dist 里最大的那个层号 = 「最远的那一格要走几步」。
 * 腐烂的橘子、多源扩散这类题要求的答案就是这个最大值（-1 表示一格都没走到）。 */
static int maxDist(const vector<vector<int>>& dist)
{
    int best = -1;
    for (size_t r = 0; r < dist.size(); r++)
        for (size_t c = 0; c < dist[r].size(); c++)
            if (dist[r][c] > best) best = dist[r][c];
    return best;
}

/* 两个距离里较近的那个，-1 当「到不了（无穷大）」处理。
 * 多源 BFS 的正确答案就该等于 min(到源点A, 到源点B)，这条性质用来对账。 */
static int nearerOne(int a, int b)
{
    if (a < 0) return b;
    if (b < 0) return a;
    if (a < b) return a;
    return b;
}

static void printGrid(const vector<string>& g)
{
    if (g.empty()) { cout << "  (空图)\n"; return; }
    const string sep = "  +" + string(g[0].size(), '-') + "+";
    cout << sep << "\n";
    for (const string& row : g)
        cout << "  |" << row << "|\n";
    cout << sep << "\n";
}

static string pathToString(const vector<pair<int, int>>& p)
{
    string s;
    for (size_t i = 0; i < p.size(); i++) {
        if (i > 0) s += "->";
        s += "(" + to_string(p[i].first) + "," + to_string(p[i].second) + ")";
    }
    return s;
}

/* ===========================================================================
 * 1. 地图模型：构造、越界、字符含义、S/T 扫描、render
 * =========================================================================== */
static void test_model()
{
    cout << "\n========== 1. 地图模型 ==========\n";

    Maze t = mazeTiny();
    assert(t.rows() == 3 && t.cols() == 4);
    assert(t.at(0, 0) == kStart && t.at(2, 3) == kTarget);
    assert(t.at(1, 2) == kOpen  && t.at(0, 3) == kWall);
    cout << "  tiny 3x4，S/T/.# 四种字符各就各位  OK\n";

    /* 边界：四条边外面一格都不能算 inBounds，负数尤其容易漏。 */
    assert(t.inBounds(0, 0) && t.inBounds(2, 3));
    assert(!t.inBounds(3, 0) && !t.inBounds(0, 4));      // 行/列各多一格
    assert(!t.inBounds(-1, 0) && !t.inBounds(0, -1));    // 负下标
    assert(!t.inBounds(-1, -1) && !t.inBounds(99, 99));
    cout << "  inBounds 把负数和外侧全挡住了  OK\n";

    /* 界外一律是墙：这一条让整个 solver.* 不需要在四个地方各写一遍越界判断。 */
    assert(t.at(99, 99) == kWall);
    assert(t.at(-1, 2) == kWall);
    assert(!t.passable(0, 3));          // 墙
    assert(!t.passable(9, 9));          // 界外
    assert(!t.passable(-1, 2));         // 负数界外
    assert(t.passable(0, 0) && t.passable(2, 3));   // S 和 T 都能踩，别把它们当墙
    cout << "  at(界外) = '#'，passable 对墙 / 界外 / 负数一律 false  OK\n";

    assert(cellEq(t.start(), make_pair(0, 0)));
    assert(cellEq(t.target(), make_pair(2, 3)));
    assert(t.hasStart() && t.hasTarget());

    /* 8 个能踩的 + 4 堵墙 = 12 格 = rows*cols。两个口径互为验证。 */
    assert(t.countPassable() == 8);
    assert(t.rows() * t.cols() - t.countPassable() == 4);

    Maze c = mazeClassic();
    assert(c.rows() == 5 && c.cols() == 7);
    assert(c.countPassable() == 27 && c.rows() * c.cols() - c.countPassable() == 8);
    Maze nw = mazeNoWay();
    assert(nw.countPassable() == 7);
    Maze is = mazeIslands();
    assert(is.countPassable() == 29 && is.rows() * is.cols() == 45);
    cout << "  四张图的格子数：tiny 8 / classic 27 / noWay 7 / islands 29  OK\n";

    /* 行参差不齐：矮的那几行被补成墙，从此不可能发生「行内越界」。 */
    Maze ragged(vector<string>{"S.", "."});
    assert(ragged.rows() == 2 && ragged.cols() == 2);
    assert(ragged.at(1, 1) == kWall);
    assert(!ragged.passable(1, 1));
    assert(ragged.countPassable() == 3);         // 补出来的那一格是墙，不算能踩
    assert((ragged.render()[1] == ".#"));        // 渲染结果里看得见补墙的痕迹
    cout << "  少写一格的地图被补成矩形，补的是墙不是路  OK\n";

    /* 1×1：最小的合法地图。没有 T，所以要能明确说「终点不存在」。 */
    Maze one(vector<string>{"S"});
    assert(one.rows() == 1 && one.cols() == 1 && one.countPassable() == 1);
    assert(one.hasStart() && !one.hasTarget());
    assert(one.target().first == -1 && one.target().second == -1);
    assert(cellEq(one.start(), make_pair(0, 0)));

    /* 一个 S 一个 T 都没有：不崩，两个都报 -1。 */
    Maze yard(vector<string>{".#.", "..."});
    assert(!yard.hasStart() && !yard.hasTarget());
    assert(yard.countPassable() == 5);

    /* 彻底空的图。 */
    Maze nothing(vector<string>{});
    assert(nothing.rows() == 0 && nothing.cols() == 0 && nothing.countPassable() == 0);
    assert(!nothing.passable(0, 0) && nothing.at(0, 0) == kWall);
    cout << "  1x1 / 没有 S T / 空图，都有明确返回而不是垃圾值  OK\n";

    /* render：把路径画回去。故意往里塞一个界外坐标 (9,9) 和一堵墙 (0,3)，
     * 看它是不是真的「再离谱也画不出鬼」。S 和 T 必须原样保留。 */
    vector<pair<int, int>> fake;
    fake.push_back(make_pair(0, 0));
    fake.push_back(make_pair(0, 1));
    fake.push_back(make_pair(1, 2));
    fake.push_back(make_pair(2, 2));
    fake.push_back(make_pair(2, 3));
    fake.push_back(make_pair(9, 9));       // 界外：必须被忽略
    fake.push_back(make_pair(0, 3));       // 墙：必须不被改成 '*'
    vector<string> pic = t.render(fake);
    assert(pic.size() == 3);
    assert(pic[0] == "S*.#");
    assert(pic[1] == "##*#");
    assert(pic[2] == "..*T");
    assert(t.at(0, 1) == kOpen);           // 原图一个字节没被改：const 成员函数的承诺
    assert(t.render().size() == 3);        // 不传路径 = 原样复制一张
    assert(t.render()[1] == "##.#");
    cout << "  render 画路径（界外和墙都不受影响，S/T 保留），且不改原图  OK\n";
}

/* ===========================================================================
 * 2. DFS：判可达 + 数踩到的格子 + 顺手找到的第一条路
 * =========================================================================== */
static void test_dfs()
{
    cout << "\n========== 2. DFS ==========\n";

    Maze t = mazeTiny();
    DfsResult dt = dfsReachable(t);
    assert(dt.reachable);
    assert(dt.visitedCells == 8);                  // tiny 整个连通块就 8 格
    assert(dt.steps == 5);
    assert((int)dt.path.size() == 6);              // 格子数 = 步数 + 1
    assert(pathIsLegal(t, dt.path));
    assert(cellEq(dt.path.front(), t.start()));
    assert(cellEq(dt.path.back(), t.target()));
    cout << "  tiny: 可达，踩到 " << dt.visitedCells << " 格，DFS 第一条路 "
         << dt.steps << " 步  OK\n";

    Maze c = mazeClassic();
    DfsResult dc = dfsReachable(c);
    PathResult bc = bfsShortest(c);
    assert(dc.reachable && bc.reachable);
    assert(dc.visitedCells == 27);                 // 27 = 5x7 减去 8 堵墙
    assert(dc.steps == 12);                        // DFS 实测：同样的图它绕到 12 步
    assert((int)dc.path.size() == 13);
    assert(dc.steps >= bc.steps);                  // ★ 铁律：DFS 那条一定不短于 BFS
    assert(pathIsLegal(c, dc.path));
    assert(pathHasNoRepeat(dc.path));
    cout << "  classic: DFS 踩到 " << dc.visitedCells << " 格；DFS " << dc.steps
         << " 步 vs BFS " << bc.steps << " 步 —— 同一条图，DFS 多绕 "
         << (dc.steps - bc.steps) << " 步  OK\n";

    /* 无解图：reachable 必须 false，steps 必须 -1，path 必须空。
     * 但 visitedCells 照样是 5 —— 「走不到终点」和「一个格子也走不到」是两件事，
     * 把这两个概念混在一起写，是所有「不可达时输出 0」这类 WA 的根源。 */
    Maze nw = mazeNoWay();
    DfsResult dn = dfsReachable(nw);
    assert(!dn.reachable);
    assert(dn.steps == -1);
    assert(dn.path.empty());
    assert(dn.visitedCells == 5);
    cout << "  noWay: 不可达 -> steps -1 + 空路径，但起点那一侧照样有 5 格  OK\n";

    /* 三个连通块：DFS 只能报告自己这一块的 9 格，另外 20 格对它不存在。 */
    Maze is = mazeIslands();
    DfsResult di = dfsReachable(is);
    assert(di.reachable);
    assert(di.visitedCells == 9);
    assert(di.steps == 4);
    assert(pathIsLegal(is, di.path));
    cout << "  islands: DFS 只看得见 S 那一块的 " << di.visitedCells << " 格"
         "（另外两块 12 + 8 格它一步都没踩）  OK\n";

    /* 起点 == 终点：0 步，路径就一个格子。 */
    Maze one(vector<string>{"S"});
    DfsResult d1 = dfsExplore(one, make_pair(0, 0), make_pair(0, 0));
    assert(d1.reachable && d1.steps == 0 && d1.visitedCells == 1);
    assert(d1.path.size() == 1 && cellEq(d1.path[0], make_pair(0, 0)));
    DfsResult d1b = dfsExplore(t, make_pair(0, 0), make_pair(0, 0));
    assert(d1b.reachable && d1b.steps == 0);
    assert(d1b.visitedCells == 8);                 // 停在起点，但整块还是要走满
    cout << "  起点==终点: 0 步 / 路径 1 格，但 visitedCells 仍是整块  OK\n";

    /* 起点是墙 / 起点界外：一个格也走不到，且不能和「走不到终点」混。 */
    DfsResult dwall = dfsExplore(t, make_pair(0, 3), make_pair(2, 3));
    assert(!dwall.reachable && dwall.visitedCells == 0 && dwall.steps == -1);
    assert(dwall.path.empty());
    DfsResult doob = dfsExplore(t, make_pair(-1, 0), make_pair(2, 3));
    assert(!doob.reachable && doob.visitedCells == 0);
    DfsResult dob2 = dfsExplore(t, make_pair(99, 99), make_pair(2, 3));
    assert(!dob2.reachable && dob2.visitedCells == 0);
    cout << "  起点是墙 / 界外：visitedCells = 0（不是 5，也不是崩溃）  OK\n";

    /* 终点是墙 / 终点界外：整块照走，可达性 false。 */
    DfsResult dwt = dfsExplore(t, make_pair(0, 0), make_pair(0, 3));
    assert(!dwt.reachable && dwt.steps == -1 && dwt.visitedCells == 8);
    DfsResult dwo = dfsExplore(t, make_pair(0, 0), make_pair(9, 9));
    assert(!dwo.reachable && dwo.visitedCells == 8);
    cout << "  终点是墙 / 界外：走满 8 格，但 reachable = false、steps = -1  OK\n";

    /* 空图：0 格、不可达，不崩。 */
    Maze nothing(vector<string>{});
    DfsResult dnull = dfsReachable(nothing);
    assert(!dnull.reachable && dnull.visitedCells == 0 && dnull.steps == -1);
    cout << "  空图安全  OK\n";

    /* 两条独立路径算同一个量：DFS 数的格子数，必须等于「以 S 为唯一源点的
     * BFS 走到的格子数」。DFS 用递归栈、BFS 用队列，代码毫无重叠，
     * 一旦哪天其中一个写错，这条 assert 立刻炸 —— 这是本项目最划算的一条断言。 */
    Maze all[4] = {mazeTiny(), mazeClassic(), mazeNoWay(), mazeIslands()};
    const int wantReached[4] = {8, 27, 5, 9};
    for (int i = 0; i < 4; i++) {
        DfsResult a = dfsReachable(all[i]);
        vector<pair<int, int>> onlyStart;
        onlyStart.push_back(all[i].start());
        vector<vector<int>> dist = bfsMultiSource(all[i], onlyStart);
        assert(a.visitedCells == countReached(dist));
        assert(a.visitedCells == wantReached[i]);
        PathResult b = bfsShortest(all[i]);
        assert(a.reachable == b.reachable);        // 可达性：两种搜索必须给同一个答案
        if (a.reachable) assert(a.steps >= b.steps);
    }
    cout << "  四张图交叉验证：DFS 踩到的格数 == 单源 BFS 走到的格数；"
         << "可达性一致；DFS 步数 >= BFS 步数  OK\n";
}

/* ===========================================================================
 * 3. BFS：最短步数 + 前驱表还原路径
 * =========================================================================== */
static void test_bfs()
{
    cout << "\n========== 3. BFS 最短路 + 路径还原 ==========\n";

    /* 讲义 notes/11 第 3 节写的是「ex12 实测 5x7 迷宫 (0,0)->(4,6) 最短 10 步」。
     * 本项目把那张图的 '.' 换成 S 和 T，墙一根没动，所以 10 必须原样复现。
     * 这条 assert 同时也是在验收「本项目的地图和讲义里是同一张图」。 */
    Maze c = mazeClassic();
    PathResult bc = bfsShortest(c);
    assert(bc.reachable);
    assert(bc.steps == 10);
    assert((int)bc.path.size() == 11);
    cout << "  classic (0,0)->(4,6) = " << bc.steps << " 步，和 notes/11 的实测值一致  OK\n";

    Maze t = mazeTiny();
    assert(bfsShortest(t).steps == 5);
    assert(bfsShortest(mazeIslands()).steps == 4);
    cout << "  tiny 5 步 / islands 4 步  OK\n";

    /* ex12 里另外三个探针，原图原坐标搬过来，期望值一条不改：
     * 这一步能同时证明「地图抄对了」和「BFS 写对了」，两个都可能错的东西互相对账。 */
    assert(bfsPath(c, make_pair(1, 4), make_pair(3, 3)).steps == 3);
    assert(bfsPath(c, make_pair(4, 0), make_pair(0, 6)).steps == 10);
    assert(bfsPath(c, make_pair(0, 0), make_pair(0, 0)).steps == 0);
    cout << "  ex12 的三个探针 (1,4)->(3,3)=3 / (4,0)->(0,6)=10 / 原地不动=0 全部复现  OK\n";

    /* 无解：-1 + 空路径。这一步是「不可达」三条分支里最容易写错的一条：
     * 忘了初始化 steps 的话，这里可能拿到一个垃圾值而不是 -1。 */
    Maze nw = mazeNoWay();
    PathResult bn = bfsShortest(nw);
    assert(!bn.reachable && bn.steps == -1 && bn.path.empty());
    cout << "  noWay: 不可达 -> steps -1、path 空  OK\n";

    /* 起终点重合：1 个格子、0 步。注意 reachable 是 **true** ——
     * 起点就是终点，「走到了」这件事天然成立，和「走不到」是两码事。 */
    PathResult b0 = bfsPath(t, make_pair(0, 0), make_pair(0, 0));
    assert(b0.reachable && b0.steps == 0 && b0.path.size() == 1);
    assert(cellEq(b0.path[0], make_pair(0, 0)));
    PathResult b0one = bfsShortest(Maze(vector<string>{"S"}));   // 只有 S，没有 T
    assert(!b0one.reachable && b0one.steps == -1 && b0one.path.empty());
    cout << "  起点==终点 -> 0 步 1 格（可达）；没有 T -> 不可达（终点是界外）  OK\n";

    /* 起点/终点是墙或界外：一律明确「不可达」，不许崩、不许返回 0。 */
    assert(!bfsPath(t, make_pair(0, 3), make_pair(2, 3)).reachable);   // 起点是墙
    assert(!bfsPath(t, make_pair(0, 0), make_pair(0, 3)).reachable);   // 终点是墙
    assert(!bfsPath(t, make_pair(-1, -1), make_pair(2, 3)).reachable); // 起点界外
    assert(!bfsPath(t, make_pair(0, 0), make_pair(3, 4)).reachable);   // 终点界外
    /* 终点这一格能踩，但被墙隔在另一个连通块里 —— 「能踩」不等于「能到」。 */
    PathResult bother = bfsPath(nw, make_pair(0, 0), make_pair(3, 2));
    assert(!bother.reachable && bother.steps == -1 && bother.path.empty());
    PathResult bw = bfsPath(t, make_pair(0, 3), make_pair(2, 3));
    assert(bw.path.empty() && bw.steps == -1);
    cout << "  墙 / 界外 / 隔壁连通块三种「走不到」都返回明确的 false + -1  OK\n";

    /* 八条性质：对每张图、每条路径都成立，而且**不需要知道期望值**就能查。
     * 上面那些 assert 钉的是「这张图应该是 10 步」，换一张图就得重算；
     * 这一段钉的是「不管什么图，一条合法的路径长什么样」—— 它才是搜索题的护身符。 */
    Maze all[4] = {mazeTiny(), mazeClassic(), mazeNoWay(), mazeIslands()};
    for (int i = 0; i < 4; i++) {
        const Maze& m = all[i];
        PathResult b = bfsShortest(m);
        if (!b.reachable) { assert(b.path.empty()); continue; }
        assert(pathIsLegal(m, b.path));            // ①非空 ②在图里 ③不穿墙 ④只走四邻
        assert(pathHasNoRepeat(b.path));           // ⑤不绕圈
        assert((int)b.path.size() == b.steps + 1); // ⑥格子数 = 步数 + 1
        assert(cellEq(b.path.front(), m.start())); // ⑦首格是 S
        assert(cellEq(b.path.back(), m.target())); // ⑧末格是 T

        /* ⑨每一格的 BFS 距离恰好等于它在路径里的下标 —— 这一条才是「最短」的证明：
         * 路径合法 + 每格层号连续 => 没有任何一格能更短地到达。
         * dist 由 bfsMultiSource（另一个函数）算，等于让两个函数互相批改作业。 */
        vector<vector<int>> dist = bfsMultiSource(m, vector<pair<int, int>>{m.start()});
        for (size_t k = 0; k < b.path.size(); k++)
            assert(dist[(size_t)b.path[k].first][(size_t)b.path[k].second] == (int)k);
        assert(dist[(size_t)b.path.back().first][(size_t)b.path.back().second] == b.steps);
    }
    cout << "  四张图的 BFS 路径逐格验收：合法、不重复、首尾正确、层号 == 下标  OK\n";

    /* 同一条最短路上「反着走」步数一样：迷宫的边是无向的（能 A 到 B 就能 B 到 A）。
     * 如果哪天你把某处写成有向（比如只让下、右），这条会第一个发现。 */
    PathResult rev = bfsPath(c, c.target(), c.start());
    assert(rev.reachable && rev.steps == bc.steps);
    assert(rev.path.front() == bc.path.back() && rev.path.back() == bc.path.front());
    cout << "  反向走 (4,6)->(0,0) 也是 " << rev.steps << " 步（无向图）  OK\n";
}

/* ===========================================================================
 * 4. 连通块
 * =========================================================================== */
static void test_components()
{
    cout << "\n========== 4. 连通块 ==========\n";

    assert(countComponents(mazeTiny()) == 1);
    assert(countComponents(mazeClassic()) == 1);      // 墙很多，但只有一块
    assert(countComponents(mazeNoWay()) == 2);        // 一堵横墙切成两半
    assert(countComponents(mazeIslands()) == 3);      // 三间封死的房
    cout << "  块数 tiny 1 / classic 1 / noWay 2 / islands 3  OK\n";

    /* 每块多大：扫描顺序是「从上到下、从左到右」，所以第一块一定是
     * 左上角那一块 —— tiny / classic / noWay / islands 的 S 都写在 (0,0)，
     * 于是 sizes[0] 就是 DFS 从 S 出发数出来的那个数。两个函数对账。
     * ⚠️ 外层多套了一对括号：assert 是**宏**，宏只认圆括号不认大括号，
     *    `vector<int>{5, 2}` 里那个逗号会被它当成「第二个参数」，
     *    报出一句完全看不懂的 "macro passed N arguments"。套一层括号就好了。 */
    assert((componentSizes(mazeTiny()) == vector<int>{8}));
    assert((componentSizes(mazeClassic()) == vector<int>{27}));
    assert((componentSizes(mazeNoWay()) == vector<int>{5, 2}));
    assert((componentSizes(mazeIslands()) == vector<int>{9, 12, 8}));
    assert(componentSizes(mazeIslands())[0] == dfsReachable(mazeIslands()).visitedCells);
    assert(componentSizes(mazeNoWay())[0] == dfsReachable(mazeNoWay()).visitedCells);
    cout << "  尺寸 [8] / [27] / [5,2] / [9,12,8]，且第一块 == DFS 从 S 数到的格数  OK\n";

    /* 各块大小之和 == 能踩的格子总数：墙一块都不能算进去，一块也不能漏。
     * 这条是「连通块把墙数进去了」这个经典错误的探测器。 */
    Maze all[4] = {mazeTiny(), mazeClassic(), mazeNoWay(), mazeIslands()};
    for (int i = 0; i < 4; i++) {
        vector<int> sizes = componentSizes(all[i]);
        int sum = 0;
        for (size_t j = 0; j < sizes.size(); j++) {
            assert(sizes[j] > 0);                    // 空块不算块
            sum += sizes[j];
        }
        assert(sum == all[i].countPassable());
        assert(countComponents(all[i]) == (int)sizes.size());
    }
    cout << "  每块都非空，块大小之和 == countPassable  OK\n";

    /* 极端图：全墙 -> 0 块；1×1 -> 1 块；空图 -> 0 块。
     * 「全墙 0 块」这条特别值得写：如果 flood fill 的条件写成
     * `if (vis[r][c]) continue;` 而忘了判墙，墙会被当成一块。 */
    assert(countComponents(Maze(vector<string>{"##", "##"})) == 0);
    assert(componentSizes(Maze(vector<string>{"##", "##"})).empty());
    assert(countComponents(Maze(vector<string>{"S"})) == 1);
    assert(countComponents(Maze(vector<string>{})) == 0);
    assert(countComponents(Maze(vector<string>{"...."})) == 1);   // 一整条直路，一块
    assert(countComponents(Maze(vector<string>{".#.#."})) == 3);  // 三个孤点
    cout << "  全墙 0 块 / 1x1 一块 / 空图 0 块 / \".#.#.\" 三块  OK\n";

    /* 加一堵墙会不会让块数变多？unNoWay 那张图里把唯一的通路砸掉，
     * 块数必须增加 —— 这就是「砸墙」类题目的最小模型（README 任务 3）。 */
    Maze before(vector<string>{"...", "...", "..."});
    Maze after(vector<string>{"...", "###", "..."});
    assert(countComponents(before) == 1 && countComponents(after) == 2);
    assert(before.countPassable() == 9 && after.countPassable() == 6);
    assert((componentSizes(before) == vector<int>{9}));
    assert((componentSizes(after) == vector<int>{3, 3}));   // 墙上、下各剩一间
    cout << "  3x3 空地一块 [9]；中间横一堵墙 -> 两块 [3, 3]，能踩的格子从 9 变 6  OK\n";
}

/* ===========================================================================
 * 5. 多源 BFS
 * =========================================================================== */
static void test_multi_source()
{
    cout << "\n========== 5. 多源 BFS ==========\n";

    Maze c = mazeClassic();

    /* 只给一个源点时，多源 BFS 必须退化成一个普通的单源 BFS：
     * dist[S] == 0，dist[T] == bfsShortest 的步数。两个函数算同一个数。 */
    vector<vector<int>> dS = bfsMultiSource(c, vector<pair<int, int>>{c.start()});
    assert(dS[(size_t)c.start().first][(size_t)c.start().second] == 0);
    assert(dS[(size_t)c.target().first][(size_t)c.target().second] == 10);
    assert(countReached(dS) == 27);
    assert(maxDist(dS) == 10);

    /* 反过来从终点推：到起点的距离也是 10。图的边是无向的，距离必须对称。 */
    vector<vector<int>> dT = bfsMultiSource(c, vector<pair<int, int>>{c.target()});
    assert(dT[(size_t)c.start().first][(size_t)c.start().second] == 10);
    cout << "  单源退化检查：dist[S][..]=0、dist[T]=10，反向也是 10  OK\n";

    /* 两个源点同时扩散：每个格子的答案 = min(到 S, 到 T)。
     * 这条性质是「多源 BFS 到底在算什么」的定义本身，拿它做全图对账，
     * 比记十个具体数字强得多 —— 换任何一张图都成立。 */
    vector<pair<int, int>> two;
    two.push_back(c.start());
    two.push_back(c.target());
    vector<vector<int>> dBoth = bfsMultiSource(c, two);
    for (int r = 0; r < c.rows(); r++)
        for (int col = 0; col < c.cols(); col++)
            assert(dBoth[(size_t)r][(size_t)col] == nearerOne(dS[(size_t)r][(size_t)col],
                                                              dT[(size_t)r][(size_t)col]));
    assert(dBoth[0][0] == 0 && dBoth[4][6] == 0);           // 两个源点自己都是 0
    assert(dBoth[0][2] == 4);                               // (0,2) 离 S 4 步、离 T 8 步 -> 4
    assert(dBoth[2][6] == 2);                               // (2,6) 贴着终点那侧 -> 2
    assert(countReached(dBoth) == 27);
    assert(maxDist(dBoth) == 8 && dBoth[0][6] == 8);        // 最远的格子是右上角 (0,6)
    cout << "  双源 == min(单源A, 单源B) 全图逐格成立；最远格 (0,6) 要 " << maxDist(dBoth)
         << " 步  OK\n";

    /* 重复源点、墙当源点、界外当源点、空源点列表：都不能崩，也不能污染结果。 */
    vector<pair<int, int>> dup;
    dup.push_back(c.start());
    dup.push_back(c.start());
    assert(bfsMultiSource(c, dup) == dS);                   // 写两遍 = 写一遍
    assert(countReached(bfsMultiSource(c, vector<pair<int, int>>{make_pair(0, 1)})) == 0);
    Maze bad = mazeTiny();
    assert(countReached(bfsMultiSource(bad, vector<pair<int, int>>{make_pair(-5, 7)})) == 0);
    assert(countReached(bfsMultiSource(bad, vector<pair<int, int>>{})) == 0);
    cout << "  重复 / 墙 / 界外 / 空列表四种脏源点输入都不崩  OK\n";

    /* 多源能跨连通块，但穿不过墙：noWay 里 S 和 T 各占一块，
     * 两个源点一铺，7 个能踩的格子全被覆盖（单源只能覆盖 5 个）。
     * islands 里 S 和 T 同在一块，另外两块（12 + 8 格）依然是 -1。
     * 这两条 assert 就是「源点数量」和「连通块」之间关系的完整描述。 */
    Maze nw = mazeNoWay();
    vector<vector<int>> nBoth = bfsMultiSource(nw, vector<pair<int, int>>{nw.start(),
                                                                           nw.target()});
    assert(countReached(nBoth) == 7);                       // 5 + 2：两块一起亮
    assert(nBoth[0][0] == 0 && nBoth[3][3] == 0);
    assert(nBoth[2][0] == 2 && nBoth[3][2] == 1);
    assert(maxDist(nBoth) == 2);
    cout << "  noWay: 单源覆盖 5 格，双源一次点亮两块共 7 格  OK\n";

    Maze is = mazeIslands();
    vector<vector<int>> iBoth = bfsMultiSource(is, vector<pair<int, int>>{is.start(),
                                                                           is.target()});
    assert(countReached(iBoth) == 9);                       // 只有 S/T 那一块被点亮
    assert(iBoth[0][3] == -1 && iBoth[4][8] == -1);         // 隔壁两间房，进不去
    assert(iBoth[4][1] == 1 && maxDist(iBoth) == 2);
    cout << "  islands: 双源也只能点亮自己那一块的 9 格，另两块保持 -1  OK\n";

    /* tiny 上把整张 dist 打出来核一遍（3x4，肉眼可查），顺手把「最远格」定死：
     * 距两个出口 {S,T} 都最远的是 (2,0)，3 步 —— 这就是「离最近出口最远的格子」。 */
    vector<vector<int>> tiny2 = bfsMultiSource(mazeTiny(),
                                               vector<pair<int, int>>{make_pair(0, 0),
                                                                      make_pair(2, 3)});
    assert((tiny2[0] == vector<int>{0, 1, 2, -1}));
    assert((tiny2[1] == vector<int>{-1, -1, 2, -1}));
    assert((tiny2[2] == vector<int>{3, 2, 1, 0}));
    assert(maxDist(tiny2) == 3);
    cout << "  tiny 双源距离图逐行核对，最远格 (2,0) = 3 步  OK\n";
}

/* ===========================================================================
 * 6. 展示：把四张图、两条路、一张汇总表印出来给人看
 * =========================================================================== */
static void show_all()
{
    cout << "\n========== 6. 四张迷宫的实际结果 ==========\n";

    Maze all[4] = {mazeTiny(), mazeClassic(), mazeNoWay(), mazeIslands()};
    /* 表里那一列必须是纯 ASCII：setw 数的是**字符个数**，一个汉字在终端里占两格，
     * 名字里掺中文，列立刻就歪（notes/03 第 3 节，score_report 的表头同理）。
     * 中文说明放在每张图自己的标题行里 —— 那一行不需要对齐，随便写。 */
    const char* name[4] = {"tiny", "classic", "noWay", "islands"};
    const char* desc[4] = {"小而简单", "讲义同款 5x7", "无解", "三个连通块"};

    for (int i = 0; i < 4; i++) {
        const Maze& m = all[i];
        DfsResult d = dfsReachable(m);
        PathResult b = bfsShortest(m);
        vector<int> sizes = componentSizes(m);

        cout << "\n--- " << name[i] << " / " << desc[i]
             << " (" << m.rows() << " x " << m.cols() << ") ---\n";
        printGrid(m.render());
        cout << "  DFS  : 可达 = " << (d.reachable ? "yes" : "no ")
             << "，从 S 踩到 " << d.visitedCells << " 格";
        if (d.reachable) cout << "，第一条路 " << d.steps << " 步 " << pathToString(d.path);
        cout << "\n";
        cout << "  BFS  : 最短 = ";
        if (b.reachable) cout << b.steps << " 步，路径 " << pathToString(b.path) << "\n";
        else             cout << "不可达（终点在另一个连通块 / 不存在）\n";
        cout << "  连通块: " << sizes.size() << " 块，大小 = ";
        for (size_t j = 0; j < sizes.size(); j++) cout << sizes[j] << (j + 1 < sizes.size() ? " " : "");
        cout << "，能踩的格子共 " << m.countPassable() << " 个\n";

        if (b.reachable) {
            cout << "  把 BFS 的路画回地图：\n";
            printGrid(m.render(b.path));
        }
    }

    /* 汇总表。最后一列用 right（setw 把空格补在数字左边），
     * 行尾一个多余空格都不留 —— score_report 里那条 assert 教的规矩，
     * 这里照着做：打印出去的东西收不回来，所以排版习惯要当场练。 */
    cout << "\n  " << left << setw(9) << "maze" << setw(7) << "size" << setw(7) << "reach"
         << setw(10) << "dfsSteps" << setw(10) << "dfsCells" << setw(10) << "bfsShort"
         << setw(9) << "pathLen" << right << setw(8) << "comps" << "\n";
    cout << "  " << string(9 + 7 + 7 + 10 + 10 + 10 + 9 + 8, '-') << "\n";
    for (int i = 0; i < 4; i++) {
        const Maze& m = all[i];
        DfsResult d = dfsReachable(m);
        PathResult b = bfsShortest(m);
        string stepsD = d.reachable ? to_string(d.steps) : "-";
        string stepsB = b.reachable ? to_string(b.steps) : "-";
        string pathLen = b.reachable ? to_string(b.path.size()) : "-";
        cout << "  " << left << setw(9) << name[i]
             << setw(7) << (to_string(m.rows()) + "x" + to_string(m.cols()))
             << setw(7) << (d.reachable ? "yes" : "no")
             << setw(10) << stepsD
             << setw(10) << d.visitedCells
             << setw(10) << stepsB
             << setw(9) << pathLen
             << right << setw(8) << countComponents(m) << "\n";
    }
    cout << "\n  读表提醒：DFS 步数那一列永远 >= BFS 最短那一列。"
         << "tiny / islands 上两列相等是运气好（图太直，一条道走到黑正好最短），\n"
         << "  classic 上 DFS 绕到 12 步、BFS 是 10 步。"
         << "把 solver.cpp 里方向数组的顺序转一格，DFS 那一列会变，BFS 永远是 10 —— 自己试。\n";
}

int main()
{
    cout << "maze_runner —— 迷宫求解器（DFS / BFS / 连通块 / 多源 BFS，多文件工程）\n";
    cout << "全部数据内置，不读键盘、不读文件；每张图跑四个算法，再用 assert 核数。\n";

    test_model();
    test_dfs();
    test_bfs();
    test_components();
    test_multi_source();
    show_all();

    /* 走到这一行，说明上面每一条 assert 都过了。断言失败根本到不了这里，
     * 屏幕上会留下 assert 打印的 文件:行号:《表达式》 —— 那就是验收信号。 */
    cout << "\n>>> maze_runner 全部通过\n";
    return 0;
}
