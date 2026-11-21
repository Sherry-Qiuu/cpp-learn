/* ============================================================================
 * ex11_dfs_backtrack.cpp —— DFS · 状态 · 搜索空间 · 回溯与剪枝
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 把讲义 11 模板一写熟：到达完整状态 / 剪枝 / 做选择 / 深入 / 撤销，五步各就各位
 *   2. 分清两种 DFS：要回溯的「枚举型」和标记完就没事的「遍历型」（flood fill）
 *   3. 「做选择 → 递归 → 撤销选择」三行必须成对出现，忘撤销是本练习的第一号陷阱
 *   4. 亲手量一次剪枝的收益：同一道题，进入函数的次数差两个数量级
 *
 * 编译运行：
 *   make run/ex11
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex11_dfs_backtrack.cpp \
 *          -o build/ex11.exe && ./build/ex11.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cassert>
using namespace std;

/* ---------------------------------------------------------------------------
 * TODO 1：手写全排列——讲义 11 模板一，一个字都别改结构
 *
 *   约定：pool 是待排的元素；used[i] 记录「第 i 个元素是否已经在 path 里」；
 *         path 是正在构造的排列；out 收集所有完整排列。
 *         函数返回这一层往下总共记录了多少个排列。
 *
 *   期望（pool = {1,2,3}，used 全 0，path 空）：
 *     返回 6，out 依次是 123 132 213 231 312 321   ← 讲义里实测的那一行
 *     pool = {7}  -> 返回 1，out = {{7}}
 *     pool = {}   -> 返回 1，out = {{}}（空排列只有「空序列」这一种，不是 0）
 *
 *   提示：① path.size() == pool.size() 就是「到达完整状态」：把 path 存进 out 然后 return；
 *         ② used[i] 为真就 continue（可行性剪枝）；
 *         ③④⑤ 做选择 → 递归 → 撤销选择。撤销一定要紧跟在递归返回之后，
 *            并且 push_back 与 pop_back、used[i]=1 与 used[i]=0 一一配对。
 *         out.push_back(path) 存的是**快照拷贝**。想一想如果 out 存的是引用会怎样。
 *         递归深度只有 pool.size() 层，爆栈轮不到它操心；要操心的是 n! 的宽度：
 *         n = 12 就是 479001600 个排列，讲义第 6 节的表里 n <= 11 是全排列的上限。
 *         所以本练习最多排到 4 个元素，别在 assert 里手滑写 12。
 * ------------------------------------------------------------------------- */
int permuteAll(const vector<int>& pool, vector<char>& used, vector<int>& path, vector<vector<int>>& out)
{
    /* TODO 在这里写实现 */
    return 0;
}

/* ---------------------------------------------------------------------------
 * TODO 2：枚举组合 C(n, k)——从 1..n 里挑 k 个，每个组合内部升序
 *
 *   约定：start 是「本轮最小可以从几开始挑」（挑过的数不再回来，于是不重不漏）；
 *         cur 是当前组合，out 收集。返回组合总数。
 *
 *   期望：
 *     combine(5, 3, 1, cur, out) -> 10，第一个是 123，最后一个是 345
 *     combine(4, 2, 1, ...) -> 6      combine(8, 3, 1, ...) -> 56
 *     combine(5, 4, ...) -> 5         combine(5, 5, ...) -> 1      combine(1, 1, ...) -> 1
 *     combine(5, 0, ...) -> 1（空组合本身就是一个组合）
 *     combine(3, 5, ...) -> 0（k > n，一个都凑不出来）
 *
 *   提示：结构和任务 1 一样，只是「下一层的最小起点」变成 v + 1，且不再需要 used 数组。
 *         边界 cur.size() == k 要放在函数最开头。
 *         然后加一条剪枝：v 往上走的时候，「v..n 还剩几个数」如果已经不够补齐
 *         k - cur.size() 个，就再也没有 v 能成功了——这时候是 break，不是 continue。
 *         写完顺手想一下：这条剪枝一个组合都没剪掉（C(8,3) 永远是 56），
 *         它省掉的是「走进凑不满的死胡同」的那些层——和任务 5 是同一种剪枝。
 * ------------------------------------------------------------------------- */
int combine(int n, int k, int start, vector<int>& cur, vector<vector<int>>& out)
{
    /* TODO 在这里写实现 */
    return 0;
}

/* ---------------------------------------------------------------------------
 * TODO 3：N 皇后计数——按行往下放，冲突就换一列
 *
 *   约定：colOf[r] = 第 r 行的皇后所在的列；参数 row 表示「现在要放第几行」。
 *         调用时传 row = 0，colOf 的长度 = n（初值随意，函数自己填）。返回摆法总数。
 *
 *   期望（countQueens(n, 0, colOf)，colOf 长度 n）：
 *     n = 0 -> 1（空棋盘算一种）    n = 1 -> 1     n = 2 -> 0     n = 3 -> 0
 *     n = 4 -> 2    n = 5 -> 10    n = 6 -> 4     n = 7 -> 40    n = 8 -> 92
 *     最后那个 92 就是讲义里说的八皇后答案，跑出来对上了再往下写。
 *
 *   提示：行是一层层往下推的，所以永远不用担心「同一行冲突」，只剩两件事要查：
 *         同列 colOf[i] == c；同斜线 abs(colOf[i] - c) == row - i。
 *         为什么斜线只有一个式子？主、副两条斜线上「列差」和「行差」总是同样长，
 *         差个正负号——abs 一次把两条都管住。
 *         注意这个函数**不需要撤销** colOf[row]：下一个候选列会直接把它覆盖掉。
 *         想清楚它和任务 1 的差别在哪（任务 1 的 used[] 可不会被自动覆盖），
 *         这就是「什么要撤销、什么不用撤销」的判断标准。
 * ------------------------------------------------------------------------- */
int countQueens(int n, int row, vector<int>& colOf)
{
    /* TODO 在这里写实现 */
    return 0;
}

/* ---------------------------------------------------------------------------
 * TODO 4：网格 DFS 数连通块（flood fill）——遍历型，不回溯
 *
 *   约定：g[r][c] == '#' 是墙，'.' 是空地；vis 与 g 同尺寸、调用前全 0。
 *         返回「从 (r, c) 出发上下左右能走到的空地格数（含自己）」。
 *         越界、是墙、已访问——一律返回 0。
 *
 *   期望（g = {".#...", ".####", ".#...", "..##.", "....."}，5×5，8 个格子是墙）：
 *     floodCount(g, 0, 0, vis) -> 14     floodCount(g, 4, 4, vis) -> 14
 *     floodCount(g, 2, 4, vis) -> 14
 *     floodCount(g, 0, 2, vis) -> 3      ← 右上角被墙封死的那一小块
 *     floodCount(g, 0, 1, vis) -> 0（墙）   floodCount(g, 1, 3, vis) -> 0（墙）
 *     floodCount(g, 5, 0, vis) -> 0（行越界）  floodCount(g, 0, -1, vis) -> 0（列越界）
 *
 *   提示：顺序永远是「先判越界，再访问数组」，反了就是一次越界读（讲义第 7 节第 3 条）。
 *         标记时机：进函数、确认这一步合法之后**立刻** vis[r][c] = 1，然后再递归四邻。
 *         千万别撤销——撤销了就从「走一遍」变成「来回走到爆栈」。
 *         方向数组 dr/dc 各 4 个，比写四个 if 干净得多，也别手写四遍递归。
 * ------------------------------------------------------------------------- */
int floodCount(const vector<string>& g, int r, int c, vector<vector<char>>& vis)
{
    /* TODO 在这里写实现 */
    return 0;
}

/* ---------------------------------------------------------------------------
 * TODO 5：子集和计数 + 访问次数——剪枝专题，同一道题跑两遍
 *
 *   约定：a 已升序，元素全为正整数。从下标 i 往后挑若干个（按下标递增挑，每个最多用一次），
 *         使和恰好等于 left。返回方案数；每进入一次函数就 visits++。
 *         prune 为 true 时启用可行性剪枝，为 false 时老老实实全枚举。
 *         **两个版本返回的方案数必须完全相同**，这正是剪枝的合法性要求。
 *
 *   期望（a = {2,3,5,7,11,13,17,19,23,29,31,37}，12 个数，总和 197）：
 *     left = 5   -> 方案 2（{2,3} 与 {5}）
 *     left = 40  -> 方案 12    left = 60 -> 方案 28
 *     left = 197 -> 方案 1（全部取走）    left = 200 -> 方案 0
 *     left = 1   -> 方案 0     left = 0 -> 方案 1（一个都不选）
 *     进入次数（main 里会打印出来，剪枝版少得多的多）：
 *     不剪枝那一列的上界是 2^12 = 4096——每个子集最多调用一次；
 *     left = 1 和 left = 197 打满 4096，方案越多反而越小，因为一凑满就 return 不再展开。
 *
 *   提示：循环体里那一刀就是全部：a[j] > left 时 **break**。
 *         这里写 continue 等于没剪——数组是升序的，后面的只会更大，
 *         一个都试不成，continue 只是让它们再排一遍队。
 *         递归的下一层：countSubsets(a, j + 1, left - a[j], visits, prune)。
 *         left == 0 记一个方案（return 1）；下标走到尽头记 0。
 *         再想一个进阶问题：left = 197 时为什么剪枝一点用都没有？
 *         （提示：break 的条件什么时候永远不成立——这也是「剪枝强度」的度量。）
 *         想更深一层：只靠 a[j] > left 还不够狠。把「从 j 开始所有数的和」
 *         预处理成后缀和 suf[j]，进函数先 if (left > suf[i]) return 0;
 *         ——剩下的全拿上也不够凑，整棵子树直接消失。这就是最优性剪枝的雏形。
 * ------------------------------------------------------------------------- */
int countSubsets(const vector<int>& a, int i, int left, int& visits, bool prune)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 6：带重复元素的全排列——讲义第 5 节的「排序剪枝 / 去重」
 *
 *   约定：签名和任务 1 一模一样，但 pool 里可能有相等的元素，值相同的排列只许算一个。
 *         **调用方保证 pool 已升序**（main 里排好了再传进来）。返回排列数。
 *
 *   期望：
 *     pool = {1,1,2}   -> 3：112 121 211   （任务 1 同一份数据给 6 个，一半是重复的）
 *     pool = {1,1,2,2} -> 6
 *     pool = {1,1,1}   -> 1
 *     pool = {1,2,3}   -> 6（没有重复元素时，结果和任务 1 完全一致）
 *     pool = {}        -> 1
 *
 *   提示：在任务 1 的循环里加一句：
 *         if (i > 0 && pool[i] == pool[i - 1] && !used[i - 1]) continue;
 *         读法：跟我值相同的那个数，如果它**还没被用**，就说明当前这个位置
 *         已经被「前面那个相同的数」代表过一次了，跳过我。
 *         为什么是 !used[i - 1] 而不是 used[i - 1]？把 {1,1,2} 的搜索树画出来，
 *         两种写法各跑一遍，答案一个是 3 一个是 6——这题面试和笔试都爱考。
 *         前提条件是「已排序」：main 里我把 {1,2,1} 原样丢进去跑，重复立刻回来；
 *         sort 一遍再喂（<algorithm> 已经 include 好了），就还是那 3 个。
 * ------------------------------------------------------------------------- */
int permuteDistinct(const vector<int>& pool, vector<char>& used, vector<int>& path, vector<vector<int>>& out)
{
    /* TODO 在这里写实现 */
    return 0;
}

/* 把 {1,2,3} 拼成 "123"，纯粹为了让 assert 和打印好看，不是练习内容。 */
string joinPath(const vector<int>& p)
{
    string s;
    for (size_t i = 0; i < p.size(); i++) s += to_string(p[i]);
    return s;
}

/* 把一组排列拼成 "123 132 213 ..." 这样的清单，同样只是工具。 */
vector<string> joinAll(const vector<vector<int>>& v)
{
    vector<string> s;
    for (size_t i = 0; i < v.size(); i++) s.push_back(joinPath(v[i]));
    return s;
}

/* 忘撤销的**错误**版本：只比任务 1 少了 path.pop_back() 和 used[i] = 0 两行。
   本练习把它留在这里，是为了让你在 assert 里亲眼看到它的症状。 */
int permuteNoUndo(const vector<int>& pool, vector<char>& used, vector<int>& path, vector<vector<int>>& out)
{
    if ((int)path.size() == (int)pool.size()) { out.push_back(path); return 1; }
    int found = 0;
    for (int i = 0; i < (int)pool.size(); i++) {
        if (used[i]) continue;
        used[i] = 1;
        path.push_back(pool[i]);
        found += permuteNoUndo(pool, used, path, out);   // 就是这一行后面少了两句撤销
    }
    return found;
}

int main()
{
    cout << "========== 任务 1：全排列（used + 回溯） ==========\n";
    {
        vector<int> pool = {1, 2, 3};
        vector<char> used(pool.size(), 0);
        vector<int> path;
        vector<vector<int>> out;
        assert(permuteAll(pool, used, path, out) == 6);
        assert(out.size() == 6);
        assert(joinAll(out) == vector<string>({"123", "132", "213", "231", "312", "321"}));
        assert(used == vector<char>(3, 0));        // 递归回到顶层，used 必须全归零
        assert(path.empty());                      // path 也必须空回来
        cout << "  pool = {1,2,3} -> ";
        for (size_t i = 0; i < out.size(); i++) cout << joinPath(out[i]) << " ";
        cout << "\n  （讲义 11 实测那一行：123 132 213 231 312 321，6 = 3!）\n";

        vector<int> one = {7};
        vector<char> u1(one.size(), 0);
        vector<int> p1;
        vector<vector<int>> o1;
        assert(permuteAll(one, u1, p1, o1) == 1);
        assert(joinAll(o1) == vector<string>({"7"}));

        vector<int> emptyPool;
        vector<char> u2;
        vector<int> p2;
        vector<vector<int>> o2;
        assert(permuteAll(emptyPool, u2, p2, o2) == 1);
        assert(o2.size() == 1 && o2[0].empty());

        vector<int> four = {1, 2, 3, 4};
        vector<char> u3(four.size(), 0);
        vector<int> p3;
        vector<vector<int>> o3;
        assert(permuteAll(four, u3, p3, o3) == 24);
        assert(u3 == vector<char>(4, 0));
    }
    cout << "  permuteAll 全部通过\n";

    cout << "\n========== 任务 2：组合 C(n, k) ==========\n";
    {
        vector<int> cur;
        vector<vector<int>> out;
        assert(combine(5, 3, 1, cur, out) == 10);
        assert(out.size() == 10);
        assert(joinPath(out.front()) == "123");
        assert(joinPath(out.back()) == "345");
        assert(cur.empty());                        // 又是「撤销干净」的检查
        assert(combine(4, 2, 1, cur, out) == 6);
        assert(combine(8, 3, 1, cur, out) == 56);
        assert(combine(5, 4, 1, cur, out) == 5);
        assert(combine(5, 5, 1, cur, out) == 1);
        assert(combine(1, 1, 1, cur, out) == 1);
        assert(combine(5, 0, 1, cur, out) == 1);    // 空组合也是一个组合
        assert(combine(3, 5, 1, cur, out) == 0);    // k > n
        assert(combine(5, -1, 1, cur, out) == 0);   // 非法 k 也得给 0，别死循环
        /* 和阶乘算出来的 C(n,k) 对拍：C(n,k) = n!/(k!(n-k)!)，这里用递推表算。 */
        long long c[11][11] = {};
        for (int i = 0; i <= 10; i++) { c[i][0] = 1; c[i][i] = 1; }
        for (int i = 2; i <= 10; i++)
            for (int j = 1; j < i; j++) c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
        for (int n = 0; n <= 10; n++) {
            for (int k = 0; k <= n; k++) {
                vector<int> tmp;
                vector<vector<int>> o;
                assert(combine(n, k, 1, tmp, o) == (int)c[n][k]);
                assert((int)o.size() == (int)c[n][k]);
            }
        }
        cout << "  combine 全部通过（含与杨辉三角的 66 组对拍）\n";
    }

    cout << "\n========== 任务 3：N 皇后计数 ==========\n";
    {
        int want[9] = {1, 1, 0, 0, 2, 10, 4, 40, 92};
        for (int n = 0; n <= 8; n++) {
            vector<int> colOf(n, 0);
            int got = countQueens(n, 0, colOf);
            assert(got == want[n]);
        }
        cout << "  n = 0..8 的解数：";
        for (int n = 0; n <= 8; n++) {
            vector<int> colOf(n, 0);
            cout << countQueens(n, 0, colOf) << " ";
        }
        cout << "\n  countQueens 全部通过（8 皇后 = 92，和讲义一致）\n";
    }

    cout << "\n========== 任务 4：网格 DFS 可达格数 ==========\n";
    {
        vector<string> g = {".#...", ".####", ".#...", "..##.", "....."};
        vector<vector<char>> vis(g.size(), vector<char>(g[0].size(), 0));
        assert(floodCount(g, 0, 0, vis) == 14);
        vector<vector<char>> v2(g.size(), vector<char>(g[0].size(), 0));
        assert(floodCount(g, 4, 4, v2) == 14);
        vector<vector<char>> v3(g.size(), vector<char>(g[0].size(), 0));
        assert(floodCount(g, 2, 4, v3) == 14);
        vector<vector<char>> v4(g.size(), vector<char>(g[0].size(), 0));
        assert(floodCount(g, 0, 2, v4) == 3);
        vector<vector<char>> v5(g.size(), vector<char>(g[0].size(), 0));
        assert(floodCount(g, 0, 1, v5) == 0);
        assert(floodCount(g, 1, 3, v5) == 0);
        assert(floodCount(g, 5, 0, v5) == 0);
        assert(floodCount(g, 0, -1, v5) == 0);
        cout << "  地图（# 是墙，14 + 3 + 8 = 25，一块不多一块不少）：\n";
        for (size_t r = 0; r < g.size(); r++) cout << "    " << g[r] << "\n";
        cout << "  floodCount 全部通过\n";
    }

    cout << "\n========== 任务 5：剪枝前后对比 ==========\n";
    {
        vector<int> a = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
        int targets[7] = {0, 1, 5, 40, 60, 197, 200};
        int wantSol[7] = {1, 0, 2, 12, 28, 1, 0};
        cout << "  left  方案数  进入次数(剪枝)  进入次数(全枚举)\n";
        for (int t = 0; t < 7; t++) {
            int vp = 0, vn = 0;
            int sp = countSubsets(a, 0, targets[t], vp, true);
            int sn = countSubsets(a, 0, targets[t], vn, false);
            assert(sp == wantSol[t]);
            assert(sn == wantSol[t]);               // 剪枝剪掉的全是废分支，答案不许变
            assert(vp <= vn);                       // 剪枝只会更少，绝不会更多
            cout << "  " << targets[t] << "     " << sp << "       " << vp << "              " << vn << "\n";
        }
        int vp5 = 0, vn5 = 0;
        countSubsets(a, 0, 5, vp5, true);
        countSubsets(a, 0, 5, vn5, false);
        assert(vp5 < vn5);                          // left = 5 时差得很远（实测 5 : 2562）
        int vp1 = 0, vn1 = 0;
        countSubsets(a, 0, 1, vp1, true);
        countSubsets(a, 0, 1, vn1, false);
        assert(vp1 < vn1);                          // left = 1 时一刀砍光（实测 1 : 4096）
        cout << "  不剪枝那一列的上界是 2^12 = 4096（每个子集至多调用一次），\n"
             << "  这就是讲义第 6 节「n <= 20 才敢枚举子集」的来历。\n";
        cout << "  countSubsets 全部通过\n";
    }

    cout << "\n========== 任务 6：重复元素去重 ==========\n";
    {
        vector<int> pool = {1, 1, 2};
        vector<char> used(pool.size(), 0);
        vector<int> path;
        vector<vector<int>> out;
        assert(permuteDistinct(pool, used, path, out) == 3);
        assert(joinAll(out) == vector<string>({"112", "121", "211"}));
        assert(used == vector<char>(3, 0));

        vector<int> pool4 = {1, 1, 2, 2};
        vector<char> u4(pool4.size(), 0);
        vector<int> p4;
        vector<vector<int>> o4;
        assert(permuteDistinct(pool4, u4, p4, o4) == 6);

        vector<int> same = {1, 1, 1};
        vector<char> u3(same.size(), 0);
        vector<int> p3;
        vector<vector<int>> o3;
        assert(permuteDistinct(same, u3, p3, o3) == 1);
        assert(joinAll(o3) == vector<string>({"111"}));

        vector<int> clean = {1, 2, 3};
        vector<char> uc(clean.size(), 0);
        vector<int> pc;
        vector<vector<int>> oc;
        assert(permuteDistinct(clean, uc, pc, oc) == 6);
        vector<vector<int>> oAll;
        vector<char> ua(clean.size(), 0);
        vector<int> pa;
        permuteAll(clean, ua, pa, oAll);
        assert(joinAll(oc) == joinAll(oAll));       // 无重复元素时两个函数必须给一样的表
    }
    cout << "  permuteDistinct 全部通过\n";

    cout << "\n========== 陷阱 1：忘了撤销 ==========\n";
    {
        vector<int> pool = {1, 2, 3};
        vector<char> used(pool.size(), 0);
        vector<int> path;
        vector<vector<int>> out;
        int got = permuteNoUndo(pool, used, path, out);
        assert(got == 1);                           // 实测：只剩一个排列
        assert(joinAll(out) == vector<string>({"123"}));
        cout << "  少了 path.pop_back() 和 used[i] = 0，输出只有：";
        for (size_t i = 0; i < out.size(); i++) cout << joinPath(out[i]) << " ";
        cout << "\n  为什么？第一支把 1、2、3 全标成 used 之后一路回到顶层，used 再没被清零，\n"
             << "  顶层循环剩下的 i 全部 continue，搜索树第二枝就没长出来。\n"
             << "  症状记住：只出一组解，程序还不报错——竞赛里这就是 WA，不是 RE。\n";

        vector<int> unsorted = {1, 2, 1};           // 没排序就上去重剪枝
        vector<char> u(unsorted.size(), 0);
        vector<int> p;
        vector<vector<int>> o;
        assert(permuteDistinct(unsorted, u, p, o) == 6);   // 实测：重复又回来了
        cout << "  去重剪枝的前提是「已排序」：pool = {1,2,1} 没排序，"
             << "permuteDistinct 给出 " << o.size() << " 个：";
        for (size_t i = 0; i < o.size(); i++) cout << joinPath(o[i]) << " ";
        cout << "\n";

        vector<int> sorted = unsorted;
        sort(sorted.begin(), sorted.end());               // {1,1,2}
        vector<char> u2(sorted.size(), 0);
        vector<int> p2;
        vector<vector<int>> o2;
        assert(permuteDistinct(sorted, u2, p2, o2) == 3);
        assert(joinAll(o2) == vector<string>({"112", "121", "211"}));
        cout << "  同一份数据 sort 之后再喂：";
        for (size_t i = 0; i < o2.size(); i++) cout << joinPath(o2[i]) << " ";
        cout << " —— 剪枝的正确性常挂在一个前提上：前提没了，去重这一刀就形同虚设。\n";
    }

    cout << "\n========== 观察题：不改代码，先猜再跑 ==========\n";
    /* --- 下面两个「写法」都会当场炸掉，所以只留在注释里，别取消注释跑 ---
     * ① 忘标 vis（或者把 vis[r][c] = 1 写到了四邻递归之后）：
     *      在 (2,2) 和 (2,3) 这种相邻空地上，A 叫 B、B 又叫 A，谁都没标记，
     *      两格之间来回互调，1MB 栈大约 4.3 万层就崩（讲义 11 第 7 节第 2、4 条）。
     *      递归爆栈的症状是「程序直接没了」，连 assert 的行号都看不到。
     * ② 越界判断放在访问之后：
     *      if (g[r][c] == '#' || r < 0 || r >= n) return;   // r 已经越界了才判
     *      短路求值救得了第 ② 种（|| 左边真就不再往右），但 g[-1] 这种下标
     *      写出去就是越界读，ASan 一跑一个准。顺序永远是先判边界再看数组。
     */
    cout << "  vis 数组这里用 vector<char> 而不是 vector<bool>：\n"
         << "  vector<bool> 存的是位，operator[] 返回代理对象，不能取引用，\n"
         << "  写 char& x = vis[r][c] 直接编译失败；char 一个字节，慢不了多少。\n";
    {
        vector<vector<int>> g(3);
        g[0] = {1, 2};
        g[1] = {0, 2};
        g[2] = {0, 1};
        vector<char> vis(g.size(), 0);
        int n = 0;
        for (size_t i = 0; i < g[0].size(); i++) {
            if (!vis[g[0][i]]) { vis[g[0][i]] = 1; n++; }
        }
        cout << "  从 0 出发，先把邻居标了再数：第一层标到 " << n << " 个点，"
             << "自己没算进去（少写一句 vis[起点] = 1 就是这个下场）。\n";
    }
    cout << "  多组数据时最坑的是「全局 vis / ans / path 忘清空」：第一组对、后面全错。\n"
         << "  本练习每个 assert 之前都新建一个 vis，就是这个习惯的具象化。\n";

    cout << "\n>>> ex11 全部通过。下一站：ex12_bfs_containers.cpp\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

int permuteAll(const vector<int>& pool, vector<char>& used, vector<int>& path, vector<vector<int>>& out)
{
    if ((int)path.size() == (int)pool.size()) {    // ① 到达完整状态：存快照
        out.push_back(path);
        return 1;
    }
    int found = 0;
    for (int i = 0; i < (int)pool.size(); i++) {
        if (used[i]) continue;                     // ② 可行性剪枝
        used[i] = 1;                               // ③ 做选择
        path.push_back(pool[i]);
        found += permuteAll(pool, used, path, out);// ④ 深入
        path.pop_back();                           // ⑤ 撤销选择 ← 回溯的灵魂
        used[i] = 0;
    }
    return found;
}

int combine(int n, int k, int start, vector<int>& cur, vector<vector<int>>& out)
{
    if ((int)cur.size() == k) {
        out.push_back(cur);
        return 1;
    }
    int found = 0;
    for (int v = start; v <= n; v++) {
        if (n - v + 1 < k - (int)cur.size()) break;   // 剩下的数不够补齐，停
        cur.push_back(v);
        found += combine(n, k, v + 1, cur, out);      // 下一层只能选比 v 大的
        cur.pop_back();
    }
    return found;
}

int countQueens(int n, int row, vector<int>& colOf)
{
    if (row == n) return 1;                        // 每一行都放下了，找到一种
    int cnt = 0;
    for (int c = 0; c < n; c++) {
        bool ok = true;
        for (int i = 0; i < row; i++) {            // 只跟已经放好的行比
            if (colOf[i] == c || abs(colOf[i] - c) == row - i) { ok = false; break; }
        }
        if (!ok) continue;
        colOf[row] = c;                            // 不用撤销：下一轮直接覆盖这一格
        cnt += countQueens(n, row + 1, colOf);
    }
    return cnt;
}

int floodCount(const vector<string>& g, int r, int c, vector<vector<char>>& vis)
{
    int n = (int)g.size(), m = (int)g[0].size();
    if (r < 0 || r >= n || c < 0 || c >= m) return 0;   // 先判越界
    if (vis[r][c] || g[r][c] == '#') return 0;          // 再判已访问 / 墙
    vis[r][c] = 1;                                      // 进函数立刻标记
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    int cnt = 1;
    for (int k = 0; k < 4; k++) cnt += floodCount(g, r + dr[k], c + dc[k], vis);
    return cnt;
}

int countSubsets(const vector<int>& a, int i, int left, int& visits, bool prune)
{
    visits++;
    if (left == 0) return 1;                       // 凑满了，算一种
    if (i == (int)a.size()) return 0;              // 没数可用了
    int cnt = 0;
    for (int j = i; j < (int)a.size(); j++) {
        if (prune && a[j] > left) break;           // 升序：后面只会更大，整段砍掉
        cnt += countSubsets(a, j + 1, left - a[j], visits, prune);
    }
    return cnt;
}

int permuteDistinct(const vector<int>& pool, vector<char>& used, vector<int>& path, vector<vector<int>>& out)
{
    if (path.size() == pool.size()) {
        out.push_back(path);
        return 1;
    }
    int found = 0;
    for (int i = 0; i < (int)pool.size(); i++) {
        if (used[i]) continue;
        if (i > 0 && pool[i] == pool[i - 1] && !used[i - 1]) continue;   // 去重剪枝
        used[i] = 1;
        path.push_back(pool[i]);
        found += permuteDistinct(pool, used, path, out);
        path.pop_back();
        used[i] = 0;
    }
    return found;
}

  为什么「做选择 / 递归 / 撤销选择」必须是紧挨着的三行：
    递归返回之后，你站的位置没变，但世界得回到「还没做这个选择」的状态，
    否则第二个分支就是在脏状态上长的。写撤销最容易犯两个错：顺序反了
    （先 used[i] = 0 再 pop_back() 本身没错，但如果中间又读了 path.back() 就悬了），
    以及只撤销了一半——used 清了、path 没 pop。本练习用 assert(used 全 0) 和
    assert(path.empty()) 把这两件事钉死：回到顶层时世界必须和进来时一模一样。

  为什么皇后可以不撤销，全排列不行：
    判断标准只有一条——「这个状态位会不会被下一轮无条件覆盖」。
    colOf[row] 在每一轮循环里都会被重新赋值，覆盖就是撤销，写 used[i] = 0
    反而多余。全排列的 used[] 有 n 个格子，循环换了 i 之后旧的那个没人管，
    不显式清就永远脏着。flood fill 的 vis[] 同理：它压根不打算重来，
    标记本身就是目的，撤销等于把「不许重复走」这条规则删掉，马上爆栈。

  为什么 break 和 continue 在剪枝里是天壤之别：
    a 升序，所以 a[j] > left 一成立，a[j+1]、a[j+2]… 全都成立。
    continue 是「这一个我不试了，后面的照旧排队」，一行代码都没省下；
    break 才是「这一整段永远不会合法，收工」。同一份数据 left = 1 时，
    break 只进 1 次函数，continue 和不剪一样是 2^n 次。
    剪枝的本质是「利用单调性或已排序，把一整个子树从搜索树上剪掉」，
    只会跳过单点的判断，通常算不上剪枝。

  为什么 N 皇后按行放，而不是在 n×n 里随便挑 n 个格子：
    状态怎么定义，直接决定搜索空间多大。按行放隐含了「每行恰好一个」，
    空间是 n^n；随便挑格子是 C(n^2, n) 级别，8 皇后差出好几个数量级。
    再补一句「每列只能一个」就是 used 数组，「两个方向对角线只能一个」
    就是那两条 (r+c) / (r-c) 的标记——把约束搬进状态里，搜索空间自己就缩小了。
    这比写出多牛的分枝技巧都有用，竞赛里想复杂度先想状态。

  为什么不剪枝版的调用数「不超过 2^n」，而且常常更少：
    每次进入 countSubsets，(已选集合, i) 唯一确定一个子集前缀，所以调用数 ≤ 子集总数
    2^n。一旦某条分支把 left 凑满就 return 1，它下面那些子集不再展开，
    于是 left = 5 时是 2562 而不是 4096；left = 197 只有最后一个子集能凑满，
    才打满 4096。数字能对上，说明你的递归既不重复也不漏——
    这是搜索题最常用的自检法：先算理论规模，再看实测 visits。
    理论值和实测值差得比预期多，多半是同一状态被走了两次，那就是剪枝没到位。
 * ========================================================================== */
