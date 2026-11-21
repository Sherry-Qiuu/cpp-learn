/* ============================================================================
 * solver.cpp —— 四个搜索算法的实现
 * ----------------------------------------------------------------------------
 * 这个文件负责什么：
 *   把 solver.h 里那四条合同落地：DFS 判可达、BFS 求最短路 + 还原路径、
 *   连通块计数、多源 BFS。
 *
 * 为什么这样拆：
 *   这个文件不 include <iostream>：算法只算不印（notes/07 那句
 *   「数据和打印分家」）。它也不改地图 —— 参数全是 const Maze&，
 *   想把 vis 数组挂到 Maze 身上是办不到的，所有中间状态都是本文件的局部变量。
 *   这正是 notes/11 第 7 节第 5 条「多组数据忘清空全局状态」的根治办法：
 *   **没有全局状态可忘**。每次调用重新开一个 vis，函数返回就析构，
 *   跑第二张迷宫时不可能带着上一张的脏数据。
 * ========================================================================== */

#include "solver.h"

#include <algorithm>    /* reverse */
#include <queue>        /* queue —— BFS 的命根子；notes/12 第 3 节 */
#include <vector>

/* ----------------------------------------------------------------------------
 * 方向数组。讲义里写的是 dr/dc，这里按题目习惯写 dx/dy，含义**完全一样**：
 * 一对下标，加一次就换一格。
 *
 * ⚠️ 本项目的坐标是 (r, c) = (行, 列)，所以 dx 改的是**行**、dy 改的是**列**，
 *    dx[k] 配 r、dy[k] 配 c，一次都不能串。notes/06 第 4 节那句
 *    「很多人把 a[x][y] 写反，越界就崩在这里」说的就是这件事：
 *    在网格里 x/y 这种命名本身就是在埋雷，r/c + 成对的 dr/dc 才是安全的写法。
 *
 * 顺序取「上、下、左、右」：{-1,0} {+1,0} {0,-1} {0,+1}。
 * 顺序不影响 BFS 的步数（最短就是最短），但**会**影响 DFS 找到哪条路、
 * 也影响 BFS 在同长度的最短路里挑哪一条。想换风格（比如洛谷题解常用的
 * 「右下左上」）就把四个数整个转一圈，main.cpp 里的步数 assert 一条都不会变，
 * 只有 DFS 那条「不最短的路」会变长或变短 —— 自己动手试试就明白了。
 * ------------------------------------------------------------------------- */
static const int dx[4] = {-1, 1, 0, 0};
static const int dy[4] = {0, 0, -1, 1};
static const int kDirCount = 4;

/* ============================================================================
 * 一、DFS
 * ========================================================================== */

/* ----------------------------------------------------------------------------
 * 递归主体：模板一（要回溯）和模板二（不回溯）在同一个函数里合体。
 *
 *   notes/11 第 2 节：
 *     遍历型（数连通块）只要「进函数就标记」，**绝不撤销**；
 *     枚举型（要交出具体那条路）必须「做选择 → 递归 → 撤销选择」。
 *
 *   这里两件都要：既要数清踩到几个格子（遍历型），
 *   又要把「第一次走到 T 的那一条路」原样交出去（枚举型）。
 *   于是 vis 只标不清（第 ① 步），path 却必须进栈出栈成对（第 ② 步配第 ⑤ 步）。
 *   如果学乖了把 path.pop_back() 删掉，path 就退化成「访问顺序表」，
 *   里面躺着一整张图的格子，根本不是路 —— 这是本项目最容易写出、
 *   也最难看出来的一个 bug，main.cpp 的 pathIsLegal 专门盯它。
 * ------------------------------------------------------------------------- */
static void dfsWalk(const Maze& m,
                    int r, int c,
                    std::vector<std::vector<char>>& vis,
                    std::vector<std::pair<int, int>>& path,
                    std::pair<int, int> dst,
                    DfsResult& out)
{
    vis[(size_t)r][(size_t)c] = 1;        /* ① 确认这一步合法之后**立刻**标记 */
    out.visitedCells++;                   /*    晚了就会重复进同一个格子，直接爆栈 */

    path.push_back(std::make_pair(r, c)); /* ② 做选择：把这一格记进当前这条路 */

    /* ③ 到终点了，而且是**第一次**到 —— 把这条路拍照存档。
     *    之后不再覆盖，所以我们交出去的是「DFS 顺手找到的第一条」，
     *    它可能比最短路径长。这正是本项目要你亲眼看见的东西。 */
    if (r == dst.first && c == dst.second && !out.reachable) {
        out.reachable = true;
        out.steps = (int)path.size() - 1; /* 路过的格子数 - 1 = 走的步数 */
        out.path = path;
    }

    for (int k = 0; k < kDirCount; k++) {
        const int nr = r + dx[k];
        const int nc = c + dy[k];
        /* 越界和撞墙被 passable 一次挡掉（界外一律算墙），
         * 所以这里**不可能**出现「先访问数组再判越界」那种 UB 顺序。 */
        if (!m.passable(nr, nc)) continue;
        if (vis[(size_t)nr][(size_t)nc]) continue;   /* 去过的不再去 = 重复状态剪枝 */
        dfsWalk(m, nr, nc, vis, path, dst, out);     /* ④ 深入 */
    }

    path.pop_back();                      /* ⑤ 撤销选择：这条路走不通，把这一格退回去 */
}

DfsResult dfsExplore(const Maze& m, std::pair<int, int> src, std::pair<int, int> dst)
{
    DfsResult out;                        /* 默认值就是「不可达 / 0 格 / -1 步 / 空路径」 */

    /* 起点是墙或界外：一个格子也走不到，直接返回。
     * 注意**不能**返回 steps = 0 —— 那是「起点就是终点」的答案，
     * 两种情况混成一个数字，main.cpp 的边界 assert 第一条就会抓到。 */
    if (!m.passable(src.first, src.second)) return out;

    /* vis 用 vector<char> 而不是 vector<bool>：notes/06 第 4 节说得很清楚，
     * vector<bool> 的 v[i] 返回的是代理对象不是 bool&，需要引用的场合会绊人。
     * 尺寸 (rows x cols) 全 0，函数级局部变量 —— 每次调用都是干净的一张新表。 */
    std::vector<std::vector<char>> vis(m.rows(), std::vector<char>(m.cols(), 0));
    std::vector<std::pair<int, int>> path;

    dfsWalk(m, src.first, src.second, vis, path, dst, out);
    return out;
}

DfsResult dfsReachable(const Maze& m)
{
    return dfsExplore(m, m.start(), m.target());
}

/* ============================================================================
 * 二、BFS：最短步数 + 路径还原
 * ========================================================================== */

PathResult bfsPath(const Maze& m, std::pair<int, int> src, std::pair<int, int> dst)
{
    PathResult res;

    /* 起点/终点不合法：不可达。终点是墙的时候「距离」这件事根本不存在，
     * 所以步数是 -1，不是 0（ex12 里那两个「起点是墙 -> -1」的期望同理）。 */
    if (!m.passable(src.first, src.second)) return res;
    if (!m.passable(dst.first, dst.second)) return res;

    /* dist 一举两得：既是「到起点的步数」，又是「访问过没有」
     * —— 未访问是 -1，而步数永远 >= 0，两者不会混。
     * 这样写比另开一个 vis 数组少一个「两个数组不同步」的 bug 来源。 */
    std::vector<std::vector<int>> dist(m.rows(), std::vector<int>(m.cols(), -1));

    /* 前驱表：prev[nr][nc] = 「是从哪一格把 (nr,nc) 发现并塞进队列的」。
     * 没有它，BFS 只能给你一个数字 10；有了它，你才能把走了 10 步的那条道
     * 一格一格描出来。它和 dist 在同一个 if 里赋值，所以永远同步。 */
    std::vector<std::vector<std::pair<int, int>>> prev(
        m.rows(), std::vector<std::pair<int, int>>(m.cols(), std::make_pair(-1, -1)));

    std::queue<std::pair<int, int>> q;
    dist[(size_t)src.first][(size_t)src.second] = 0;
    q.push(src);
    /* ★ 入队的时候就把 dist 写上，不是等出队再写。这一行是 BFS 唯一必须记牢的细节：
     *   ex12 拿一张 8 点 8 边的图实测过 —— 入队时标记 push **8** 次（每点恰好一次），
     *   出队时才标记 push **9** 次；换成 3 个点的三角形就是 3 次 vs 4 次。
     *   多出来的那一次就是「同一个点被两个邻居各塞进队列一回」。
     *   图小只是多绕一圈，图一大就完蛋：1000×1000 的空迷宫里，一个点被重复入队
     *   的次数等于「到它的最短路径条数」，中间那一带一次就是上万，队列先撑爆内存，
     *   复杂度从 O(n·m) 直接退化掉。这里的 dist != -1 就是那个「提前标记」，
     *   它对 BFS 的正确性也是必需的：dist 写下的那一刻就宣布
     *   「我找到了离它最近的来路」，晚一步就会被别的邻居抢先写成一个更长的值。 */
    while (!q.empty()) {
        const std::pair<int, int> cur = q.front();
        q.pop();                          /* C++17 也可以写 auto [r, c] = q.front(); */
        const int r = cur.first;
        const int c = cur.second;

        /* 要不要「弹出终点就 break」？只求步数时可以，而且大图省一大截。
         * 这里不 break，因为跑满全图的 dist 还能顺手回答
         * 「从起点一共能走到几格」，多一次遍历换一批可免费验证的数字，值。 */
        for (int k = 0; k < kDirCount; k++) {
            const int nr = r + dx[k];
            const int nc = c + dy[k];
            if (!m.passable(nr, nc)) continue;   /* 越界 / 墙：passable 内部先判越界 */
            if (dist[(size_t)nr][(size_t)nc] != -1) continue;  /* 已经被别的层认领过 */
            dist[(size_t)nr][(size_t)nc] = dist[(size_t)r][(size_t)c] + 1;  /* ★ 入队时标记 */
            prev[(size_t)nr][(size_t)nc] = cur;
            q.push(std::make_pair(nr, nc));
        }
    }

    if (dist[(size_t)dst.first][(size_t)dst.second] == -1) return res;  /* 走不到 */

    res.reachable = true;
    res.steps = dist[(size_t)dst.first][(size_t)dst.second];

    /* ---- 还原路径：从终点沿 prev 一路回退到起点，再倒过来 ----
     * 为什么从终点倒着走而不是从起点正着走？因为 prev 表只有「我是谁发现的」
     * 这一个信息，正着走需要「下一个是谁」，而一个点的下一个可能有好几个
     * （分支），前驱却是唯一的。倒着走是唯一方向，天然不会走错。
     * 循环出口用的是「走到起点了」，不是「prev 是 (-1,-1)」：
     * 起点的 prev 恰好也是 (-1,-1)，两种写法在这张表上等价，
     * 但「按坐标判等」读起来更像人话，也不怕将来起点被写成别的哨兵值。 */
    for (int r = dst.first, c = dst.second; ; ) {
        res.path.push_back(std::make_pair(r, c));
        if (r == src.first && c == src.second) break;
        const std::pair<int, int> p = prev[(size_t)r][(size_t)c];
        r = p.first;
        c = p.second;
    }
    std::reverse(res.path.begin(), res.path.end());
    /* 起点 == 终点时，这个循环只转一圈就 break，path.size() == 1、steps == 0 ——
     * 不用为它写任何特判。好的边界处理是「逻辑本身就对」，不是「多加一个 if」。 */
    return res;
}

PathResult bfsShortest(const Maze& m)
{
    return bfsPath(m, m.start(), m.target());
}

/* ============================================================================
 * 三、连通块
 * ========================================================================== */

/* 从 (r,c) 出发能踩到的格子数，顺手把它们全标记成已访问。
 * 形状就是 notes/11 第 2 节的模板二：先判合法性 → 标记 → 递归四邻，**不回溯**。
 * 这里**故意**用 vector<vector<bool>>，因为讲义自检第 3 问就是这个：
 * vector<bool> 是位压缩的特化版，vis[r][c] 返回的是代理对象而不是 bool&。
 * 本函数只「写 true」和「读一下」，从不取引用，所以安全；
 * 哪天要写 bool& x = vis[r][c]; 会当场编译失败，那时换 vector<char>。
 * 递归深度 = 一块的最大直径，几十层的样例图毫无压力；
 * 真上 1000×1000 就得改迭代栈（README 的报错速查里写了这件事先想到什么）。 */
static int floodFill(const Maze& m, int r, int c, std::vector<std::vector<bool>>& vis)
{
    if (!m.passable(r, c)) return 0;      /* 越界 / 墙：这条路到此为止 */
    if (vis[(size_t)r][(size_t)c]) return 0;
    vis[(size_t)r][(size_t)c] = true;

    int n = 1;                            /* 自己这一格 */
    for (int k = 0; k < kDirCount; k++)
        n += floodFill(m, r + dx[k], c + dy[k], vis);
    return n;
}

std::vector<int> componentSizes(const Maze& m)
{
    std::vector<std::vector<bool>> vis(m.rows(), std::vector<bool>(m.cols(), false));
    std::vector<int> sizes;

    /* 扫描顺序：从上到下、从左到右。碰到一个「能踩且没被任何一块收走」的格子，
     * 就说明发现了一块新领土 —— 块数 +1，然后 flood fill 把整块一次收光。
     * 为什么 flood fill 敢一次收光：连通关系的定义就是「互相走得到」，
     * 走得到的会被这次遍历全标上，下次扫到直接跳过，一块只会被数一次。 */
    for (int r = 0; r < m.rows(); r++)
        for (int c = 0; c < m.cols(); c++)
            if (m.passable(r, c) && !vis[(size_t)r][(size_t)c])
                sizes.push_back(floodFill(m, r, c, vis));

    return sizes;
}

int countComponents(const Maze& m)
{
    return (int)componentSizes(m).size();
}

/* ============================================================================
 * 四、多源 BFS
 * ========================================================================== */

std::vector<std::vector<int>> bfsMultiSource(
    const Maze& m,
    const std::vector<std::pair<int, int>>& sources)
{
    std::vector<std::vector<int>> dist(m.rows(), std::vector<int>(m.cols(), -1));
    std::queue<std::pair<int, int>> q;

    /* 唯一的差别就在这个开局循环：所有源点一起进队，距离全是 0。
     * 相当于凭空造了一个「超级起点」，它和每个源点之间连一条 0 步的边。
     * 之后的扩散过程和单源 BFS 一字不差。 */
    for (const std::pair<int, int>& s : sources) {
        if (!m.passable(s.first, s.second)) continue;   /* 墙 / 界外的源点：丢掉，不崩 */
        if (dist[(size_t)s.first][(size_t)s.second] != -1) continue;  /* 重复源点只算一次 */
        dist[(size_t)s.first][(size_t)s.second] = 0;
        q.push(s);
    }

    while (!q.empty()) {
        const std::pair<int, int> cur = q.front();
        q.pop();
        const int r = cur.first;
        const int c = cur.second;
        for (int k = 0; k < kDirCount; k++) {
            const int nr = r + dx[k];
            const int nc = c + dy[k];
            if (!m.passable(nr, nc)) continue;
            if (dist[(size_t)nr][(size_t)nc] != -1) continue;
            dist[(size_t)nr][(size_t)nc] = dist[(size_t)r][(size_t)c] + 1;
            q.push(std::make_pair(nr, nc));
        }
    }
    /* 为什么多个源点同时扩散还能给出「到最近源点」的距离？
     * 理由和单源一模一样：队列按层号排，谁先把一个格子塞进队列，
     * 谁就是离它最近的那个源点。于是 dist 天然是 min(到源点1, 到源点2, ...)。
     * notes/11 第 3 节那个「分层 while (sz--)」的写法在这里可以省掉 ——
     * 因为你把层号存在 dist 里了，轮次直接就是 dist[r][c]，不用现场数队列。
     * 到不了的格子（比如被墙封在另一间房里的）保持 -1，这一点很关键：
     * 多源 BFS 穿不过墙，它能覆盖的连通块数目是「含源点的那些块」。 */
    return dist;
}
