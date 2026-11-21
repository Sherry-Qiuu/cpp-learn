/* ============================================================================
 * ex12_bfs_containers.cpp —— BFS 与容器选型：queue · stack · map · set · 堆 · deque
 * ----------------------------------------------------------------------------
 * 目标：
 *   1. 用 queue 把讲义 11 的 BFS 模板跑通，并复现那个 5×7 迷宫的 10 步
 *   2. 会用「先记下 q.size()」这一招做分层遍历（最少轮次类题目的通用写法）
 *   3. stack / map / set / priority_queue / deque 各写一遍最小可用代码
 *   4. 把四个容器陷阱踩在练习里：出队才标记、operator[] 查询、比较函数方向、默认大顶堆
 *
 * 编译运行：
 *   make run/ex12
 *   或：g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 exercises/ex12_bfs_containers.cpp \
 *          -o build/ex12.exe && ./build/ex12.exe
 *
 * 说明：把每个函数体里的「TODO 在这里写实现」注释行和它下面那行占位 return
 *       一起替换成你的实现。实现完 unused-parameter 警告就会消失。
 *       全部 assert 通过才算做完。答案在文件末尾，先自己想 20 分钟。
 * ========================================================================== */

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <deque>
#include <functional>
#include <cassert>
using namespace std;

/* ---------------------------------------------------------------------------
 * TODO 1：迷宫最短步数——讲义 11 的 BFS 模板，用 queue 写
 *
 *   约定：g[r][c] == '#' 是墙，'.' 能走；一步只能上下左右走一格。
 *         返回从 (sr, sc) 到 (tr, tc) 的最少步数（起点算第 0 步）。
 *         起点或终点是墙、或者压根走不到，都返回 -1。
 *
 *   期望（maze = {".#.....", "...#..#", ".#...#.", "..#....", "..#.#.."}，5 行 7 列）：
 *     mazeSteps(maze, 0, 0, 4, 6) -> 10   ← 讲义里实测的那个 10
 *     mazeSteps(maze, 4, 0, 0, 6) -> 10
 *     mazeSteps(maze, 0, 6, 4, 0) -> 10
 *     mazeSteps(maze, 1, 4, 3, 3) -> 3
 *     mazeSteps(maze, 0, 0, 0, 0) -> 0（起点就是终点，一步不用走）
 *     mazeSteps(maze, 0, 0, 0, 1) -> -1（终点是墙）
 *     mazeSteps(maze, 0, 1, 0, 0) -> -1（起点是墙，同样 -1，别写 0）
 *     mazeSteps({".#.", "###", "..."}, 0, 0, 2, 0) -> -1（被一堵横墙隔开）
 *     mazeSteps({"....."}, 0, 0, 0, 4) -> 4       只有一行也能跑
 *     mazeSteps({"."}, 0, 0, 0, 0) -> 0
 *
 *   提示：dist 数组开成 -1，「-1」既表示没到过也表示不可达，最后直接 return dist[tr][tc]。
 *         顺序永远是：先判越界 continue，再看是不是墙、是不是已经去过。
 *         ★ 关键那一句：dist[nr][nc] = dist[r][c] + 1 必须写在 push 之前，
 *           也就是「入队时就标记」。为什么不能等出队再标？main 里我准备了两个版本
 *           给你量出差距。DFS 出栈时标记无所谓，BFS 不行——想清楚差别在哪。
 *         想少一点拷贝可以用 vector + 一个 head 下标代替 queue（讲义 12 第 3 节），
 *         两种写法都得过同一批 assert。
 * ------------------------------------------------------------------------- */
int mazeSteps(const vector<string>& g, int sr, int sc, int tr, int tc)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 2：BFS 分层——返回每一层有多少个结点
 *
 *   约定：g[u] 是 u 的邻居表（无向图，邻接表）；vis 长度 = g.size()，调用前全 0。
 *         返回一个 vector：result[0] = 第 0 层的结点数（永远是 1，就是你出发的点），
 *         result[1] = 距离为 1 的结点数……层数就是 result 的长度。
 *         图是连通的（不用考虑走不到的点）。
 *
 *   期望（g 有 8 个点：0-{1,2} 1-{0,3,4} 2-{0,5} 3-{1,6} 4-{1} 5-{2,7} 6-{3,7} 7-{5,6}）：
 *     bfsLevels(g, 0, vis) -> {1, 2, 3, 2}      四层的宽度分别是 1、2、3、2
 *     bfsLevels(g, 4, vis) -> {1, 1, 2, 2, 2}   五层，层数 - 1 = 4 = 4 号点的最远距离
 *     bfsLevels(g, 7, vis) -> {1, 2, 2, 2, 1}
 *   期望（三角形图 {{1,2},{0,2},{0,1}}）：
 *     bfsLevels(g, 0, vis) -> {1, 2}            ← 三个点两层的图，别漏了「第二层只剩 2 个」
 *
 *   提示：就是讲义里那六行：while 里先 sz = (int)q.size()，然后把这一层的 sz 个点
 *         全部处理完，才把层数加一。sz 必须在内层循环之前取——
 *         一边 pop 一边 push 时 q.size() 是变的，写成 while (sz = q.size()) 就永远走不完了。
 *         vis[start] = 1 别忘了，否则起点会被邻居再塞回队列一次。
 *         这招能算的东西比你想的多：最少轮次、腐烂的橘子、地图相邻同色扩散，全是它。
 * ------------------------------------------------------------------------- */
vector<int> bfsLevels(const vector<vector<int>>& g, int start, vector<char>& vis)
{
    /* TODO 在这里写实现 */
    return {};
}

/* ---------------------------------------------------------------------------
 * TODO 3：括号匹配——stack 的经典用法
 *
 *   约定：只关心 ( ) [ ] { } 这六种字符，其它字符一律忽略（空串算匹配）。
 *         匹配返回 true，否则 false。
 *
 *   期望：
 *     isBalanced("([]{})")  -> true    ← 讲义里那个 matched
 *     isBalanced("{{[]}}()") -> true
 *     isBalanced("()[]{}")  -> true
 *     isBalanced("a(b)c")   -> true    （字母不是括号，直接放过）
 *     isBalanced("")        -> true    （空串：栈空，匹配）
 *     isBalanced("(((")     -> false   （左括号剩下了）
 *     isBalanced("())")     -> false   （右括号多了）
 *     isBalanced("([)]")    -> false   （顺序错乱：交叉闭合不算）
 *     isBalanced("{[}]")    -> false   （类型不匹配）
 *     isBalanced(")(")      -> false   （一上来就是右括号）
 *     isBalanced("(")       -> false
 *
 *   提示：遇左括号 push；遇右括号，先 if (st.empty()) return false; 再 top()/pop()。
 *         这两个判空顺序反过来就是「在空容器上调 top()」——未定义行为，
 *         不崩在你预期的地方，也不给编译器报错。
 *         最后 return st.empty(); 而不是 return true;——"(((" 就是这么混过去的。
 * ------------------------------------------------------------------------- */
bool isBalanced(const string& s)
{
    /* TODO 在这里写实现 */
    return false;
}

/* ---------------------------------------------------------------------------
 * TODO 4：统计词频并按字典序输出——map 的正确用法
 *
 *   约定：words 是一串单词；返回 "单词:次数" 用单个空格连起来的一行，
 *         顺序按单词字典序（map 帮你排好了），次数只统计出现过的词。
 *
 *   期望：
 *     freqReport({"pear", "apple", "pear", "fig"}) -> "apple:1 fig:1 pear:2"
 *       ← 讲义实测那一行：按字母序，不是按出现顺序，也不是按次数
 *     freqReport({"a"})         -> "a:1"
 *     freqReport({"a", "a", "a"})-> "a:3"
 *     freqReport({})            -> ""（空串，别多一个空格）
 *     freqReport({"zeta", "alpha", "Alpha", "beta"}) -> "Alpha:1 alpha:1 beta:1 zeta:1"
 *       ← 'A' 的 ASCII 比 'a' 小，所以大写排在前面。string 的 < 是字典序，不是长度序。
 *
 *   提示：统计就一行 cnt[w]++，这靠的是 map 的 operator[]：键不存在时先插入 0 再给引用。
 *         拼字符串时「空格只在元素之间」这个老问题（ex06）在这里又来了：
 *         要么先判 res 非空再补空格，要么用 ostringstream。
 *         要「按输入顺序」输出的话，光有 map 不够——得再用一个 vector 记下第一次出现的顺序。
 *         只查询不统计时用 cnt.count(w) 或 cnt.find(w)，理由看 main 里的陷阱演示。
 * ------------------------------------------------------------------------- */
string freqReport(const vector<string>& words)
{
    /* TODO 在这里写实现 */
    return "";
}

/* ---------------------------------------------------------------------------
 * TODO 5：交集与并集——set 自动去重 + 自动有序
 *
 *   约定：两个函数都返回「升序、无重复」的结果，用 vector<int> 装好方便比较。
 *         输入里可以有重复元素，也可以乱序。空表合法。
 *
 *   期望：
 *     setUnion({3, 1, 2, 3}, {2, 5, 1})        -> {1, 2, 3, 5}
 *     setIntersection({3, 1, 2, 3}, {2, 5, 1}) -> {1, 2}
 *     setIntersection({1, 2}, {3, 4})          -> {}（没有公共元素，空表不是 0）
 *     setUnion({}, {})                         -> {}
 *     setUnion({-1, 0, 5}, {-1, -1})           -> {-1, 0, 5}
 *     setIntersection({2, 2, 2}, {2})          -> {2}
 *
 *   提示：并集 = 把两边都 insert 进同一个 set；交集 = 遍历一个 set，问另一个 count() > 0。
 *         别写成「两个 vector 双重循环」——那是 O(n·m)，set 版本是 O((n+m) log(n+m))。
 *         set 迭代出来就是有序的，构造 vector 时直接给区间：vector<int>(se.begin(), se.end())，
 *         不用再 sort 一遍。
 *         标准库确实有 set_intersection 那一套，但它要求输入已排序、
 *         还要配 inserter 用，手写这两个循环反而更清楚。
 * ------------------------------------------------------------------------- */
vector<int> setUnion(const vector<int>& a, const vector<int>& b)
{
    /* TODO 在这里写实现 */
    return {};
}

vector<int> setIntersection(const vector<int>& a, const vector<int>& b)
{
    /* TODO 在这里写实现 */
    return {};
}

/* ---------------------------------------------------------------------------
 * TODO 6：第 k 大——priority_queue，注意堆的方向
 *
 *   约定：k 从 1 开始数，k = 1 就是最大值；重复元素算不同的名次（{5,5,5,1} 的第 2 大是 5）。
 *         k <= 0 或 k 超过元素个数时返回 -1。
 *
 *   期望：
 *     kthLargest({7, 2, 9, 1, 5}, 3) -> 5      ← 讲义实测那个 5
 *     kthLargest({7, 2, 9, 1, 5}, 1) -> 9
 *     kthLargest({7, 2, 9, 1, 5}, 5) -> 1      （第 n 大 = 最小值）
 *     kthLargest({5, 5, 5, 1}, 2)    -> 5      （算重数，不是「第 2 个不同的值」）
 *     kthLargest({-3, -1, -2}, 2)    -> -2     （负数照样排）
 *     kthLargest({42}, 1)            -> 42
 *     kthLargest({7, 2, 9}, 4)       -> -1     k 太大
 *     kthLargest({7, 2, 9}, 0)       -> -1     k = 0 非法
 *     kthLargest({}, 1)              -> -1     空表什么都取不出
 *
 *   提示：Top-K 的标准套路是「维护一个大小为 k 的小顶堆」：每个数都 push，
 *         一旦 size() > k 就 pop 掉堆顶（也就是当前最小的那个），最后 top() 就是第 k 大。
 *         堆的方向三行背下来：priority_queue<int> 是**大顶堆**；
 *         要小顶堆必须写全三个模板参数 priority_queue<int, vector<int>, greater<int>>；
 *         自定义比较函数写 return a < b; 得到的还是大顶堆（main 里有实测）。
 *         为什么是反的？那个比较函数是「谁该被挤下去」的关系，不是「谁先出场」。
 *         全量排序取 a[n-k] 也能过这组数据，但 n = 1e7、k = 10 时你就知道区别了。
 * ------------------------------------------------------------------------- */
int kthLargest(const vector<int>& data, int k)
{
    /* TODO 在这里写实现 */
    return -1;
}

/* ---------------------------------------------------------------------------
 * TODO 7：滑动窗口最大值——deque 单调队列
 *
 *   约定：窗口大小 k，从左往右每次滑一格；返回每个窗口位置的最大值。
 *         k <= 0 或 k > a.size() 时返回空表。输出长度 = a.size() - k + 1。
 *
 *   期望：
 *     windowMax({1, 3, 2, 5, 4}, 3) -> {3, 5, 5}   ← 讲义实测那三个数
 *     windowMax({1, 3, 2, 5, 4}, 1) -> {1, 3, 2, 5, 4}（窗口为 1 就是原数组）
 *     windowMax({1, 3, 2, 5, 4}, 5) -> {5}         （一个窗口罩住全部）
 *     windowMax({3, 3, 3, 3}, 2)    -> {3, 3, 3}   （全相等，注意 <= 才压得掉）
 *     windowMax({5, 4, 3, 2, 1}, 3) -> {5, 4, 3}   （严格下降，队里挤满了一串）
 *     windowMax({-5, -1, -3}, 2)    -> {-1, -1}    （负数没有特殊待遇）
 *     windowMax({1, 2}, 5)          -> {}          k 比长度大
 *     windowMax({}, 3)              -> {}          空表
 *
 *   提示：dq 里存**下标**，不存值——因为要判断队首有没有滑出窗口。
 *         三步：① 队尾那些 <= a[i] 的没有存在价值，pop_back 掉（比 a[i] 小又比 a[i] 早，
 *         以后永远轮不到它当最大值）；② push_back(i)；
 *         ③ 队首下标 <= i - k 说明它已经出窗，pop_front。
 *         然后 i >= k - 1 才开始收答案，收的是 a[dq.front()]。
 *         为什么整体是 O(n)：每个下标最多进队一次、出队一次，总共 2n 次操作。
 *         想一想把 ① 的 <= 写成 < 会怎样：{3,3,3,3} 那组数据队列会越积越长，
 *         答案虽然还对，但复杂度分析的「每个元素出队一次」就开始打折扣了。
 * ------------------------------------------------------------------------- */
vector<int> windowMax(const vector<int>& a, int k)
{
    /* TODO 在这里写实现 */
    return {};
}

/* 「出队时才标记」的 BFS：代码只差一行，push 次数已经能量出来了。不是练习内容。 */
int pushesMarkOnPop(const vector<vector<int>>& g, int start)
{
    vector<char> vis(g.size(), 0);
    queue<int> q;
    int pushes = 1;
    q.push(start);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        vis[u] = 1;                                  // ← 晚了一步
        for (size_t i = 0; i < g[u].size(); i++) {
            int v = g[u][i];
            if (vis[v]) continue;
            q.push(v); pushes++;
        }
    }
    return pushes;
}

/* 「入队时就标记」的正确版本。 */
int pushesMarkOnPush(const vector<vector<int>>& g, int start)
{
    vector<char> vis(g.size(), 0);
    queue<int> q;
    int pushes = 1;
    vis[start] = 1;
    q.push(start);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (size_t i = 0; i < g[u].size(); i++) {
            int v = g[u][i];
            if (vis[v]) continue;
            vis[v] = 1;                              // ← 赶在 push 之前
            q.push(v); pushes++;
        }
    }
    return pushes;
}

/* 大家都以为「a < b 是小的优先」，于是写出这个比较函数。它得到的其实是大顶堆。 */
struct CmpAsWritten {
    bool operator()(int a, int b) const { return a < b; }
};

/* 只用来打印，帮你看清三种堆的出场顺序。 */
template <typename PQ>
void drain(PQ pq, const string& tag)
{
    cout << "  " << tag;
    while (!pq.empty()) { cout << pq.top() << " "; pq.pop(); }
    cout << "\n";
}

int main()
{
    cout << "========== 任务 1：queue + BFS = 最短步数 ==========\n";
    {
        vector<string> maze = {".#.....", "...#..#", ".#...#.", "..#....", "..#.#.."};
        assert(mazeSteps(maze, 0, 0, 4, 6) == 10);
        assert(mazeSteps(maze, 4, 0, 0, 6) == 10);
        assert(mazeSteps(maze, 0, 6, 4, 0) == 10);
        assert(mazeSteps(maze, 1, 4, 3, 3) == 3);
        assert(mazeSteps(maze, 0, 0, 0, 0) == 0);
        assert(mazeSteps(maze, 0, 0, 0, 1) == -1);
        assert(mazeSteps(maze, 0, 1, 0, 0) == -1);
        assert(mazeSteps({".#.", "###", "..."}, 0, 0, 2, 0) == -1);
        assert(mazeSteps({"....."}, 0, 0, 0, 4) == 4);
        assert(mazeSteps({"."}, 0, 0, 0, 0) == 0);
        cout << "  迷宫（5 行 7 列）：\n";
        for (size_t r = 0; r < maze.size(); r++) cout << "    " << maze[r] << "\n";
        cout << "  (0,0) -> (4,6) = " << mazeSteps(maze, 0, 0, 4, 6) << " 步，"
             << "曼哈顿距离 |4-0| + |6-0| = 10，一点没绕远。\n"
             << "  同一个迷宫用 DFS 也能到，但它给你的第一条路不保证是 10 步——这就是 BFS 的存在理由。\n";
    }
    cout << "  mazeSteps 全部通过\n";

    cout << "\n========== 任务 2：分层遍历 ==========\n";
    {
        vector<vector<int>> g(8);
        g[0] = {1, 2};
        g[1] = {0, 3, 4};
        g[2] = {0, 5};
        g[3] = {1, 6};
        g[4] = {1};
        g[5] = {2, 7};
        g[6] = {3, 7};
        g[7] = {5, 6};
        vector<char> vis(g.size(), 0);
        assert((bfsLevels(g, 0, vis) == vector<int>({1, 2, 3, 2})));
        vector<char> v2(g.size(), 0);
        assert((bfsLevels(g, 4, v2) == vector<int>({1, 1, 2, 2, 2})));
        vector<char> v3(g.size(), 0);
        assert((bfsLevels(g, 7, v3) == vector<int>({1, 2, 2, 2, 1})));
        vector<char> v4(g.size(), 0);
        int total = 0;
        vector<int> lv = bfsLevels(g, 0, v4);
        for (size_t i = 0; i < lv.size(); i++) total += lv[i];
        assert(total == 8);                          // 每层的并起来就是全部点，不多不少
        vector<vector<int>> tri = {{1, 2}, {0, 2}, {0, 1}};
        vector<char> v5(tri.size(), 0);
        assert((bfsLevels(tri, 0, v5) == vector<int>({1, 2})));
        cout << "  从 0 出发各层宽度：";
        for (size_t i = 0; i < lv.size(); i++) cout << lv[i] << " ";
        cout << "-> 最远距离 " << (int)lv.size() - 1 << " 步\n";
        cout << "  bfsLevels 全部通过\n";
    }

    cout << "\n========== 任务 3：stack 判括号 ==========\n";
    {
        assert(isBalanced("([]{})") == true);
        assert(isBalanced("{{[]}}()") == true);
        assert(isBalanced("()[]{}") == true);
        assert(isBalanced("a(b)c") == true);
        assert(isBalanced("") == true);
        assert(isBalanced("(((") == false);
        assert(isBalanced("())") == false);
        assert(isBalanced("([)]") == false);
        assert(isBalanced("{[}]") == false);
        assert(isBalanced(")(") == false);
        assert(isBalanced("(") == false);
        cout << "  isBalanced(\"([]{})\") -> " << (isBalanced("([]{})") ? "matched" : "broken") << "\n";
        cout << "  isBalanced 全部通过\n";
    }

    cout << "\n========== 任务 4：map 统计词频 ==========\n";
    {
        assert(freqReport({"pear", "apple", "pear", "fig"}) == "apple:1 fig:1 pear:2");
        assert(freqReport({"a"}) == "a:1");
        assert(freqReport({"a", "a", "a"}) == "a:3");
        assert(freqReport({}) == "");
        assert(freqReport({"zeta", "alpha", "Alpha", "beta"}) == "Alpha:1 alpha:1 beta:1 zeta:1");
        cout << "  freqReport({pear,apple,pear,fig}) = " << freqReport({"pear", "apple", "pear", "fig"}) << "\n";
        cout << "  map 全部通过\n";
    }

    cout << "\n========== 任务 5：set 交并 ==========\n";
    {
        assert((setUnion({3, 1, 2, 3}, {2, 5, 1}) == vector<int>({1, 2, 3, 5})));
        assert((setIntersection({3, 1, 2, 3}, {2, 5, 1}) == vector<int>({1, 2})));
        assert((setIntersection({1, 2}, {3, 4}) == vector<int>{}));
        assert((setUnion({}, {}) == vector<int>{}));
        assert((setUnion({-1, 0, 5}, {-1, -1}) == vector<int>({ -1, 0, 5})));
        assert((setIntersection({2, 2, 2}, {2}) == vector<int>({2})));
        cout << "  set 全部通过\n";
    }

    cout << "\n========== 任务 6：priority_queue 第 k 大 ==========\n";
    {
        assert(kthLargest({7, 2, 9, 1, 5}, 3) == 5);
        assert(kthLargest({7, 2, 9, 1, 5}, 1) == 9);
        assert(kthLargest({7, 2, 9, 1, 5}, 5) == 1);
        assert(kthLargest({5, 5, 5, 1}, 2) == 5);
        assert(kthLargest({-3, -1, -2}, 2) == -2);
        assert(kthLargest({42}, 1) == 42);
        assert(kthLargest({7, 2, 9}, 4) == -1);
        assert(kthLargest({7, 2, 9}, 0) == -1);
        assert(kthLargest({}, 1) == -1);
        cout << "  kthLargest 全部通过\n";
    }

    cout << "\n========== 任务 7：deque 单调队列 ==========\n";
    {
        assert((windowMax({1, 3, 2, 5, 4}, 3) == vector<int>({3, 5, 5})));
        assert((windowMax({1, 3, 2, 5, 4}, 1) == vector<int>({1, 3, 2, 5, 4})));
        assert((windowMax({1, 3, 2, 5, 4}, 5) == vector<int>({5})));
        assert((windowMax({3, 3, 3, 3}, 2) == vector<int>({3, 3, 3})));
        assert((windowMax({5, 4, 3, 2, 1}, 3) == vector<int>({5, 4, 3})));
        assert((windowMax({-5, -1, -3}, 2) == vector<int>({-1, -1})));
        assert((windowMax({1, 2}, 5) == vector<int>{}));
        assert((windowMax({}, 3) == vector<int>{}));
        assert((windowMax({7}, 1) == vector<int>({7})));
        cout << "  windowMax({1,3,2,5,4}, 3) -> ";
        vector<int> wm = windowMax({1, 3, 2, 5, 4}, 3);
        for (size_t i = 0; i < wm.size(); i++) cout << wm[i] << " ";
        cout << "\n  deque 全部通过\n";
    }

    cout << "\n========== 陷阱 1：出队才标记 ==========\n";
    {
        vector<vector<int>> g(8);
        g[0] = {1, 2};
        g[1] = {0, 3, 4};
        g[2] = {0, 5};
        g[3] = {1, 6};
        g[4] = {1};
        g[5] = {2, 7};
        g[6] = {3, 7};
        g[7] = {5, 6};
        assert(pushesMarkOnPush(g, 0) == 8);         // 每个点正好进队一次
        assert(pushesMarkOnPop(g, 0) == 9);          // 多塞了 1 次，图越密多得越凶
        assert(pushesMarkOnPop(g, 4) == 9);
        vector<vector<int>> tri = {{1, 2}, {0, 2}, {0, 1}};
        assert(pushesMarkOnPush(tri, 0) == 3);
        assert(pushesMarkOnPop(tri, 0) == 4);
        cout << "  8 个点、8 条边的图：入队时标记 push " << pushesMarkOnPush(g, 0)
             << " 次（正好每点一次），出队时标记 push " << pushesMarkOnPop(g, 0) << " 次。\n";
        cout << "  3 个点的三角形图：8 → 3 次，9 → 4 次，重复率立刻上来。\n"
             << "  在 1000×1000 的空迷宫里，一个点被重复入队的次数等于「到它的最短路径条数」，\n"
             << "  中间那一带一次就是上万，队列先爆内存。复杂度从 O(n·m) 退化掉，\n"
             << "  讲义第 3 节才说这是 BFS 唯一必须记牢的细节。\n";
    }

    cout << "\n========== 陷阱 2：map 的 operator[] 会插入 ==========\n";
    {
        map<string, int> cnt;
        cnt["pear"]++;
        cnt["apple"]++;
        cnt["pear"]++;
        cnt["fig"]++;
        assert(cnt.size() == 3);
        int hit = cnt["kiwi"] > 0 ? 1 : 0;           // 只是想查一下，什么都没写
        cout << "  查了一个不存在的 kiwi：hit = " << hit
             << "，可 cnt.size() 变成了 " << cnt.size() << "（原来只有 3 个键）。\n";
        assert(cnt.size() == 4);
        assert(!cnt.count("mango"));                 // count / find 才不会脏容器
        assert(cnt.size() == 4);
        cout << "  换成 cnt.count(\"mango\") 查一次，size 还是 " << cnt.size() << "。\n"
             << "  查询用 count/find，统计才用 operator[]。这条在「统计不同单词数」的题里直接决定 AC 还是 WA。\n";
    }

    cout << "\n========== 陷阱 3：堆的方向 ==========\n";
    {
        int data[5] = {7, 2, 9, 1, 5};
        priority_queue<int> big;
        priority_queue<int, vector<int>, greater<int>> small;
        priority_queue<int, vector<int>, CmpAsWritten> fooled;
        for (int i = 0; i < 5; i++) {
            big.push(data[i]);
            small.push(data[i]);
            fooled.push(data[i]);
        }
        assert(big.top() == 9);
        assert(small.top() == 1);
        assert(fooled.top() == 9);                   // 写了 a < b，结果还是大顶堆
        drain(big, "priority_queue<int>（默认，大顶堆）出队顺序：");
        drain(small, "第三个参数 greater<int>（小顶堆）出队顺序：");
        drain(fooled, "自定义 return a < b; 的出队顺序：");
        cout << "  记忆法：比较函数描述的是「父子之间谁压着谁」，不是「谁先出场」。\n"
             << "  默认 less -> 大的在堆顶；要小顶堆就 greater，别无脑自己写 a < b。\n";
    }

    cout << "\n========== 观察题：不改代码，先猜再跑 ==========\n";
    {
        vector<string> order = {"pear", "apple", "fig"};
        set<string> se(order.begin(), order.end());
        cout << "  vector 按插入序：";
        for (size_t i = 0; i < order.size(); i++) cout << order[i] << " ";
        cout << "\n  set 按字典序：  ";
        for (set<string>::iterator it = se.begin(); it != se.end(); ++it) cout << *it << " ";
        cout << "\n  要「插入序」就留 vector，要「自动有序 + 去重」才换 set，别顺手就 set。\n";
    }
    {
        set<string> cs = {"abc", "Ab", "b", "aB"};
        cout << "  set<string> 排序（ASCII）：";
        for (set<string>::iterator it = cs.begin(); it != cs.end(); ++it) cout << *it << " ";
        cout << "\n  大写比小写小（'A' = 65 < 'a' = 97），比较的是逐字符 ASCII，不是长度。\n"
             << "  想忽略大小写就先 tolower 再进 set，或者自己写比较器（第 08 章）。\n";
    }
    /* --- 空容器上取 top()/front()/back() 是未定义行为，所以这段留在注释里 ---
     * stack<int> emptySt;
     * int x = emptySt.top();          // 不报错，给你一个垃圾值，然后一路带到 assert 里
     * queue<int> emptyQ;
     * emptyQ.pop();                   // 同上，ASan 都未必抓得住
     * 规矩：任何 top / front / back / pop 之前，先确认容器非空。
     *       BFS 里 while (!q.empty()) 包住整个循环体，就是这条规则的体现。
     */
    cout << "  queue 没有 q[0]：只能碰 front()/back()。这不是缺陷，是 BFS 正确性的来源——\n"
         << "  「先进先出」保证了先被发现的点一定比后被发现的近，最短路的性质就建在这上面。\n"
         << "  想随机访问就用讲义 12 那个 vector + head 下标的写法，语义完全一样。\n";

    cout << "\n>>> ex12 全部通过。练习部分到此结束，下一站：projects/maze_runner（DFS + BFS 真刀真枪）\n";
    return 0;
}

/* ============================================================================
 *                          参考答案（想清楚再看）
 * ============================================================================

int mazeSteps(const vector<string>& g, int sr, int sc, int tr, int tc)
{
    int n = (int)g.size(), m = (int)g[0].size();
    if (g[sr][sc] == '#' || g[tr][tc] == '#') return -1;
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<pair<int, int>> q;
    dist[sr][sc] = 0;
    q.push({sr, sc});
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;   // 先判越界
            if (g[nr][nc] == '#' || dist[nr][nc] != -1) continue;   // 再判墙 / 已访问
            dist[nr][nc] = dist[r][c] + 1;                          // ★ 入队时就标记
            q.push({nr, nc});
        }
    }
    return dist[tr][tc];                              // 没走到过就是 -1
}

vector<int> bfsLevels(const vector<vector<int>>& g, int start, vector<char>& vis)
{
    vector<int> level;
    queue<int> q;
    vis[start] = 1;
    q.push(start);
    while (!q.empty()) {
        int sz = (int)q.size();                       // ★ 先量好这一层有多少个
        level.push_back(sz);
        while (sz--) {
            int u = q.front(); q.pop();
            for (size_t i = 0; i < g[u].size(); i++) {
                int v = g[u][i];
                if (vis[v]) continue;
                vis[v] = 1;
                q.push(v);
            }
        }
    }
    return level;
}

bool isBalanced(const string& s)
{
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') { st.push(c); continue; }
        if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;             // 右括号多了
            char t = st.top(); st.pop();
            if (c == ')' && t != '(') return false;
            if (c == ']' && t != '[') return false;
            if (c == '}' && t != '{') return false;
        }
    }
    return st.empty();                                // 左括号多了也不算匹配
}

string freqReport(const vector<string>& words)
{
    map<string, int> cnt;
    for (const string& w : words) cnt[w]++;
    string res;
    for (const auto& kv : cnt) {
        if (!res.empty()) res += " ";
        res += kv.first + ":" + to_string(kv.second);
    }
    return res;
}

vector<int> setUnion(const vector<int>& a, const vector<int>& b)
{
    set<int> se(a.begin(), a.end());
    se.insert(b.begin(), b.end());
    return vector<int>(se.begin(), se.end());
}

vector<int> setIntersection(const vector<int>& a, const vector<int>& b)
{
    set<int> sa(a.begin(), a.end()), sb(b.begin(), b.end());
    vector<int> res;
    for (int x : sa) {
        if (sb.count(x)) res.push_back(x);
    }
    return res;
}

int kthLargest(const vector<int>& data, int k)
{
    if (k <= 0 || k > (int)data.size()) return -1;
    priority_queue<int, vector<int>, greater<int>> pq;   // 大小为 k 的小顶堆
    for (int x : data) {
        pq.push(x);
        if ((int)pq.size() > k) pq.pop();                // 踢掉当前最小的
    }
    return pq.top();
}

vector<int> windowMax(const vector<int>& a, int k)
{
    vector<int> ans;
    if (k <= 0 || k > (int)a.size()) return ans;
    deque<int> dq;                                       // 存下标
    for (int i = 0; i < (int)a.size(); i++) {
        while (!dq.empty() && a[dq.back()] <= a[i]) dq.pop_back();   // 没竞争力的丢掉
        dq.push_back(i);
        if (dq.front() <= i - k) dq.pop_front();                     // 过期的从头部丢
        if (i >= k - 1) ans.push_back(a[dq.front()]);                // 队首 = 窗口最大
    }
    return ans;
}

  为什么 BFS 第一次到达就是最短：
    队列先进先出，于是「入队顺序」就是「距离顺序」：距离 d 的点全在距离 d+1 的点之前出队。
    归纳一下：起点距离 0 先入队；假设距离 d 的点出队时 dist 已经是真最短，
    它们扩展出来的邻居距离是 d+1，而此时队列里绝不会有距离更大的点比它们先出队。
    所以第一次给某个点写上 dist 的那一次，一定是最早的那次——这正好要求「入队时就标记」：
    标记 = 宣布「我找到了离它最近的来路」，晚一步（出队才标）就有别的邻居抢着宣布第二次。
    DFS 没有这个性质：它先把一条道走到底，回来的那条路可能短得多。

  为什么分层要先把 q.size() 存进 sz：
    内层循环一边 pop 一边 push，q.size() 每一刻都在变。存下 sz 就等于
    「在本层开始的地方画一条线」：sz 次处理之后队列里剩的全是下一层的点，
    层数才 +1。这一招把「一个 while」拆成两层，能算的就不只是最短距离了——
    每层宽度、最少轮次、同时起点的多源扩散（把全部源点先塞进队列、dist 都写 0，
    然后原样跑分层）都是它。多源 BFS 之所以还能给出「到最近源点」的距离，
    理由和单源完全一样：谁先把它塞进队列，谁就是离它最近的那个。

  为什么 map 的 operator[] 是「双刃剑」，而 set 没有这个问题：
    operator[] 的返回类型是 int&，为了给出一个能放在赋值号左边的引用，
    键不存在时它必须先插入一个值初始化的元素。cnt[w]++ 写起来顺手，正是靠这个语义；
    代价是「查询」和「写入」共用了一个入口。set 没有 operator[]，
    查存在只有 count / find，想插入必须显式 insert，所以 set 天生不会被动污染。
    同理 set.count() 是 O(log n)，别在双重循环里拿 vector 的 find 当哈希用。

  为什么小顶堆要写 greater<int>，而自定义 a < b 反而是大顶堆：
    priority_queue 维护的是「Compare(父, 子) 必须为 false」的结构，
    堆顶是那个「不会被任何儿子比下去」的元素。Compare = less 时，
    父 < 子 为假 ⇒ 父 >= 子 ⇒ 大顶；换成 greater 才反过来变成小顶。
    所以 top() 是 Compare 眼里的「最大值」。自己写比较函数时，
    把 return a < b; 读成「b 比 a 大时 a 该往下沉」，方向就不会想反。

  为什么单调队列整体是 O(n)，而它看起来是个嵌套循环：
    分析这种循环要看「每个元素被处理几次」，而不是「有几层 for」。
    下标 i 只 push_back 一次，之后不管从 pop_back 还是 pop_front 离开队列，
    总共就出去一次，所以两头的操作次数各 ≤ n，摊还 O(1)，整体 O(n)。
    这也解释了为什么 pop_back 的条件要用 <=：相等时让新的进来，
    队列里的下标区间才始终是「递增且值递减」，队首永远代表当前窗口的最优解。
 * ========================================================================== */
