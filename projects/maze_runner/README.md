# 项目 2 · 迷宫求解器（DFS / BFS / 连通块 / 多源 BFS）

**这是什么**：把 notes/11（DFS 与 BFS）、notes/12（`queue` / `pair` / `vector`）、notes/06（二维网格、方向数组、越界）、notes/07（struct + 成员函数）学到的东西，收进**一个多文件工程**里，顺便把 ex12 结尾预告的那句「下一站：projects/maze_runner（DFS + BFS 真刀真枪）」兑现掉。

迷宫是搜索入门的全部难点压缩成的一个模型：**状态 = 一个格子，转移 = 上下左右走一格，障碍 = 墙**。四个算法把这张图上能问的问题一次问完：

- 从 `S` 出发**走得到 `T` 吗**？顺手还能知道「一共能踩到几个格子」—— DFS
- 最短**几步**？那条路**具体怎么走**？—— BFS + 前驱表
- 这张图被墙切成了**几块**？—— flood fill 数连通块
- 有好几个起点**同时**往外扩散，每格离**最近**的起点多远？—— 多源 BFS

重点不是「会写模板」（模板在 notes/11 里已经写全了）。重点是三件写出来才发现的事：

1. **DFS 也能走到终点，但它给的那条路不是最短的** —— 本项目让两个算法跑同一张图，把 12 和 10 并排打给你看；
2. **路径不是一个数字，是一串格子**，它有五种坏法（穿墙、跳格、斜走、出界、绕圈），所以本项目的 `main.cpp` 里写了一个**不看期望值**也能验收路径的函数；
3. **搜索题的 assert 可以互相查账**：DFS 数的格子数 == BFS 走到的格子数，连通块大小之和 == 能踩的格子总数 —— 两套独立代码给出同一个数，比十道「我以为对了」可靠。

数据是**内置的四张迷宫**，程序不读键盘也不读文件，`make run` 非交互、几十毫秒跑完。

---

## 目录结构

```
projects/maze_runner/
├── maze.h            地图模型接口：字符含义、Maze 结构体、四张内置样例的声明
├── maze.cpp          地图模型的实现：构造(补齐矩形/找 S T)、at/passable/render + 样例数据
├── solver.h          算法接口：四个算法 + DfsResult / PathResult 两个结果结构体
├── solver.cpp        算法实现：DFS、BFS + 前驱表还原、连通块、多源 BFS。零 cout
├── main.cpp          使用方 + 测试驱动 + 打印：156 处 assert、路径合法性检查、结果汇总
├── Makefile          分离编译规则：.cpp -> .o -> .exe（和 p1 完全同构）
└── build/            产物目录（.o / .d / .exe），已被 .gitignore 忽略，不要提交
```

---

## 怎么用

```bash
# 在仓库根目录
make p2                     # 编译 + 运行（推荐，这条就是验收命令）
make clean                  # 连两个项目的产物一起清（会调本目录的 make clean）

# 或者在项目目录里
cd projects/maze_runner
make            # = make all，只编译不运行
make run        # 编译并运行
make clean      # 删掉 build/ 里的东西
make rebuild    # clean + all
make help       # 列出所有目标
```

Windows 上如果 `make` 不认，用 `mingw32-make`（同一个程序，见下面的报错速查）。

第一次跑 `make p2`，屏幕上先出现四行构建输出：

```
  [CXX]  maze.cpp -> build/maze.o
  [CXX]  solver.cpp -> build/solver.o
  [CXX]  main.cpp -> build/main.o
  [LINK] build/maze.o build/solver.o build/main.o -> build/maze_runner.exe
```

然后是六节自测输出，最后停在 `>>> maze_runner 全部通过`。

---

## 每个文件负责什么

| 文件 | 角色 | 里面有什么 |
|---|---|---|
| `maze.h` | **合同**：一张地图长什么样 | include 守卫、五个字符常量、`struct Maze`（含 const 成员函数）、四个 `mazeXxx()` 声明 |
| `maze.cpp` | 地图的实现 | 构造函数（补矩形 + 扫 S/T）、`at` / `passable` / `countPassable` / `render` + 四张写死的样例。**一行 cout 都没有** |
| `solver.h` | 合同：四个算法各回答什么问题 | `DfsResult` / `PathResult` 两个结果结构体 + 六个函数声明 |
| `solver.cpp` | 算法实现 | 方向数组、`dfsWalk`、`bfsPath`（dist + prev）、`floodFill`、`bfsMultiSource`。不 include iostream，也没有任何全局状态 |
| `main.cpp` | 使用方 + 测试 + 打印 | 六个测试/展示函数、`pathIsLegal` 等五个辅助函数、156 处 `assert`、结果汇总表 |
| `Makefile` | 构建规则 | 每个 `.cpp` 单独出 `.o` 再链接；`-MMD -MP` 自动生成头文件依赖 |

依赖方向单向：`main.cpp → solver.h → maze.h`。`maze.cpp` 不知道 `solver` 存在，
`solver.cpp` 不知道 `main` 存在 —— 所以这四个算法可以直接搬去写洛谷的迷宫题，只要换一个 main。

---

## 关键代码逐段讲解

### 1. 地图：把「越界」这件事从算法里消灭掉

```cpp
char Maze::at(int r, int c) const
{
    if (!inBounds(r, c)) return kWall;      // 界外一律当墙
    return g_[r][c];
}
bool Maze::passable(int r, int c) const { return at(r, c) != kWall; }
```

notes/11 第 7 节第 3 条：「顺序永远是先判越界，再访问数组」。但如果每个算法都自己写一遍
`if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;`，这个项目就有**四个地方**会写漏。
现在「界外 == 墙」，越界和撞墙被一个函数一起挡掉，判断只写一次、只可能错一次。
语义上也讲得通：地图外面当然是走不出去的。

构造函数里还藏着第二个防御：把矮一截的行**用墙补齐成矩形**。

```cpp
for (std::string& row : g_)
    if ((int)row.size() < cols_) row.append(cols_ - (int)row.size(), kWall);
```

手写地图少敲一个字符太常见了。不补的话 `g_[1].size()` 可能小于 `cols_`，
而 `inBounds` 只比到 `cols_` —— 于是 `g_[1][cols_-1]` 就是一次**越界读**：
编译不报错，运行不一定崩，崩在别的函数里。补墙而不是补空地，
是因为宁可让地图比想象中更严，也不要凭空多送出一条路。
`main.cpp` 里 `Maze ragged({"S.", "."})` 那三条 assert 就是在验这件事。

### 2. 方向数组：`dx/dy` 的取舍，以及它影响的到底是什么

```cpp
static const int dx[4] = {-1, 1, 0, 0};
static const int dy[4] = {0, 0, -1, 1};
...
const int nr = r + dx[k], nc = c + dy[k];
if (!m.passable(nr, nc)) continue;
```

四个方向写成一个循环，比抄四遍 `if` 短，更重要的是**改邻域只改一处**
（想加斜向就往数组里再塞两对，加完四个算法一起生效）。

两个必须想清楚的点：

- **`dx` 改的是行还是列？** 本项目的坐标是 `(r, c)`，所以 `dx` 配 `r`、`dy` 配 `c`。
  讲义里写 `dr/dc` 就是为了不让人把 `x/y` 想成「横向/纵向」——在屏幕坐标里 `y` 向下增，
  在矩阵里第 0 维是行，两套直觉撞在一起就写出 `g[x][y]`，然后越界崩在别处。
  **网格题一律用 `r/c` 命名，是免费的保险。**
- **顺序影响什么、不影响什么？** 上→下→左→右 换成 右→下→左→上，**BFS 的步数不变**
  （最短就是最短），但 DFS 找到的第一条路会变长或变短，BFS 在同长度的最短路里挑哪一条也会变。
  所以：**别把「DFS 这次走了 12 步」当成性质**，它是方向顺序的产物；
  而「DFS 步数 >= BFS 步数」是性质，`main.cpp` 里 assert 的是后者（外加把这个数钉死，换顺序就让它炸给你看）。

### 3. DFS：两个模板在同一个函数里合体

notes/11 给了两个模板，很多人背下来了却没想过它们为什么不一样：

```cpp
vis[r][c] = 1;                 // ① 进函数、确认合法之后**立刻**标记，绝不撤销
out.visitedCells++;
path.push_back({r, c});        // ② 做选择
if (到终点 && !out.reachable) out.path = path;      // ③ 第一次到，把这条路拍照存档
for (k : 四邻) if (能踩 && !去过的) dfsWalk(...);   // ④ 深入
path.pop_back();               // ⑤ 撤销选择
```

- **`vis` 必须只标不清**。这是模板二（遍历型）：一个格子踩到过就不用再踩，
  数格子、判可达靠它，复杂度才会是 O(n·m)。撤销 `vis` 就是 notes/11 第 7 节第 2 条：
  无限递归、爆栈。
- **`path` 必须撤销**。这是模板一（枚举型）：`path` 表示「**当前正在走的这一条**」，
  退回上一步就得把这一格从条里摘掉。如果图省事不 pop，`path` 就退化成「访问顺序表」，
  里面躺着整张图的格子，最后一步却不是从起点走过去的 ——
  步数看着还挺合理，`pathIsLegal` 一查就发现中间在穿墙或跳格。
  **这是本项目最容易写出、也最难用肉眼看出来的 bug**，所以它有自己的 assert。

顺便：`out.reachable` 那个条件让 DFS 只记录**第一条**到达终点的路，之后不再覆盖。

### 4. 为什么 BFS 能找到最短，DFS 不能

```cpp
dist[nr][nc] = dist[r][c] + 1;      // 层号 = 到起点的步数
```

队列是 FIFO，所以「先被发现的点一定不比后发现点更远」，
于是一个点**第一次**被写上 `dist` 的那次，必然就是最早的那次 ⇒ 层号 == 最短步数。
DFS 没有这个性质：它顺着一条道冲到黑，等它摸到终点时，
那条路可能早就绕出去了 —— 绕出去的那一段不会因为「后来找到了出口」而消失。

本项目不用你信这句话，用两张实测数字盯着它：

| 迷宫 | DFS 第一条路 | BFS 最短 | 差 |
|---|---|---|---|
| `classic` 5×7 | **12** 步 | **10** 步 | 多绕 2 |
| `tiny` 3×4 | 5 步 | 5 步 | 0（图太直，运气好） |
| `islands` 5×9 | 4 步 | 4 步 | 0 |

`main.cpp` 里对应三条：`assert(dc.steps == 12)`、`assert(bc.steps == 10)`、
以及那条**对所有图都成立**的 `assert(a.steps >= b.steps)`。
把方向数组转一格，第一条会跟着变，最后一条永远绿 —— 这就是「性质」和「巧合」的区别。

经典 3×3 反例（自己画一下就有）：`S` 在左上、`T` 在右下，中间全空。
DFS 若按「上→下→左→右」走，会先向下绕一大圈再回来，5 步；BFS 是 4 步。

### 5. 前驱表：怎么把「10 步」变成「这 10 步是怎么走的」

BFS 的原版只给你 `dist[T] = 10`。想输出路径，就多养一张表：

```cpp
prev[nr][nc] = cur;                       // 是谁把 (nr,nc) 塞进队列的
...
for (int r = tr, c = tc; ; ) {            // 从终点倒着回退到起点
    res.path.push_back({r, c});
    if (r == sr && c == sc) break;
    auto [pr, pc] = prev[r][c];
    r = pr;  c = pc;
}
reverse(res.path.begin(), res.path.end());
```

三个「为什么」：

- **为什么要 `prev`？** 因为 `dist` 只记了「几层」，没记「从哪来」。
  一张图里同一层可能有十几个格子，光靠 `dist` 描不出唯一的一条道。
- **为什么从终点倒着走？** 前驱是唯一的（每个点只被发现的**那一次**），
  「后继」却有分支。倒着走只有一个方向，天然不会走错。
- **为什么 `dist` 和 `prev` 在同一个 `if` 里赋值？** 因为它俩必须严格同步。
  少写一行 `prev` 就会得到「步数对、路径断」的诡异结果 —— 而且 `pathIsLegal` 一抓一个准。

起点 == 终点时，那个循环转一圈就 `break`，`path.size() == 1`、`steps == 0`，
**不需要任何特判**。好的边界处理是「逻辑本身覆盖边界」，不是多加一个 `if`。

### 6. 入队时就标记，不是出队时标记

```cpp
dist[src] = 0;  q.push(src);              // ★ 写 dist 的那一行就是「标记」
while (!q.empty()) {
    auto [r, c] = q.front(); q.pop();
    ...
    if (dist[nr][nc] != -1) continue;     // 已经被别的层认领过
    dist[nr][nc] = dist[r][c] + 1;        // ★ 入队时标记
    q.push({nr, nc});
}
```

ex12 拿一张 8 点 8 边的图实测过：
**入队时标记 push 8 次（每个点恰好一次），出队时才标记 push 9 次**；
换成 3 个点的三角形就是 3 次 vs 4 次。多出来的那一次，是同一个点被它的两个邻居各塞进队列一回。

为什么小图看着没事、大图会死：一个格子被重复入队的次数 = **到它的最短路径条数**。
1000×1000 的空迷宫中间那一带，这个数字是上万，队列先撑爆内存，
复杂度直接从 O(n·m) 退化掉。另一层理由和正确性有关：写 `dist` 的那一句
等于宣布「我找到了离它最近的来路」，晚一步就会被别的邻居抢先写成一个更长的值。

`bfsMultiSource` 里同一段逻辑出现第二次 —— 这是有意的：
**多源 BFS 的「源点全部开局入队、dist 写 0」正是这个规则的产物**，
两处写法必须一模一样，将来才改得动。

### 7. 连通块为什么能用 flood fill 数

```cpp
for (int r = 0; r < m.rows(); r++)
    for (int c = 0; c < m.cols(); c++)
        if (m.passable(r, c) && !vis[r][c])
            sizes.push_back(floodFill(m, r, c, vis));      // 一整块一次收光
```

连通关系的定义就是「互相走得到」。所以从任何一格出发 flood fill，
**恰好**把它所在的整块全标上 `vis`：这一块之后不会再被数第二次，
而下一块一定还有至少一格没被标（否则它就和上一块连通了）。
于是「扫到没标过的格子」这件事和「发现一块新领土」严格一一对应，块数 = 循环成功的次数。

两个配套的细节：

- **墙不能是块**。判据必须是 `passable(r,c) && !vis[r][c]` 两个条件都有。
  只写 `!vis` 的话，一整片墙会被当成一块 —— `main.cpp` 里
  `assert(countComponents(Maze({"##","##"})) == 0)` 就专门盯这一条。
- **`vector<vector<bool>>` 的代理对象**。`notes/06` 第 4 节和 `notes/11` 自检第 3 问都提过：
  `vector<bool>` 是位压缩特化版，`vis[r][c]` 返回的**不是 `bool&`**，
  写 `bool& x = vis[r][c];` 会当场编译失败。这里只「写 true」和「读一下」，不取引用，所以安全；
  需要引用的场合一律换 `vector<char>`（`dfsExplore` 里的 `vis` 就是 `vector<char>`，
  同一个项目里两种写法都在，对照着记）。
- 各块大小之和必须 == `countPassable()`。这条 `assert` 是「漏了一格」和「墙算进来了」
  两种错的共同探测器。

### 8. 多源 BFS：好几个点同时往外扩

```cpp
for (auto s : sources) {                    // 唯一的区别就是这个开局循环
    if (!m.passable(s.first, s.second)) continue;
    if (dist[s.first][s.second] != -1) continue;    // 重复源点只算一次
    dist[s.first][s.second] = 0;
    q.push(s);
}
// 之后和单源 BFS 一字不差
```

等价于凭空造一个「超级起点」，它到每个源点之间连一条 0 步的边。
得到的 `dist[r][c]` = 「这一格到**最近那个**源点要几步」。
典型题：多个出口谁先跑到、腐烂的橘子（烂橘子就是源点）、
「地图上离任何一个基地最远的那个格子有多远」。

本项目用三条 assert 把它和前面三个算法的关系钉死了：

- **退化**：只给一个源点时，`dist[T] == bfsShortest().steps`（两个函数算同一个数）；
- **定义**：两个源点时，每一格都满足 `dist == min(到A, 到B)`，全图逐格检查；
- **和连通块的接口**：源点数盖不住墙 ——
  `noWay` 单源只点亮 5 格，`{S,T}` 双源一次点亮 7 格（两块一起亮）；
  `islands` 里 `S` 和 `T` 同在一块，双源也只能点亮 9 格，另外两块（12 + 8 格）保持 `-1`。

顺带一个 `notes/11` 的疑问：为什么这里不写「分层 `while (sz--)`」？
因为**层号已经存在 `dist` 里了**，轮次直接就是 `dist[r][c]`，不用现场数队列。
只有当你不想开 `dist` 数组、或者要在「层」这个粒度上做统计时才需要 `sz`。

### 9. `pathIsLegal`：不看期望值的验收

数字型的 assert（`steps == 10`）只能测「这一张图」，换图就得重算。
路径型的 assert 不一样，它是**性质**，对任何图、任何路径都成立：

```cpp
static bool pathIsLegal(const Maze& m, const vector<pair<int, int>>& path)
{
    if (path.empty()) return false;
    for (size_t i = 0; i < path.size(); i++) {
        if (!m.inBounds(r, c)) return false;       // ② 不许出界
        if (!m.passable(r, c)) return false;       // ③ 不许穿墙
        if (i == 0) continue;
        const int dr = r - path[i-1].first, dc = c - path[i-1].second;
        if (dr * dr + dc * dc != 1) return false;  // ④ 必须走一格、只能四方向
    }
    return true;
}
```

`dr*dr + dc*dc == 1` 这个判据一次排掉了三种错：斜走（得 2）、跳格（得 4）、原地不动（得 0）。
配套还有 `pathHasNoRepeat()`（最短路径不可能绕圈：回到同一个格子，把中间那段剪掉会更短）
和 `dist[path[k]] == k`（每一格的 BFS 层号恰好等于它在路径里的下标 ——
这一条才是「最短」的证明）。

**这一段是本项目最值得抄进你自己的模板的东西**：
它不依赖任何写死的数字，却能把「假 AC 的迷宫路径」当场按住。

### 10. Makefile：和 p1 完全同构，改头文件会自动重编

规则一条没变（见 `projects/score_report/README.md` 第 6 节）。
本项目实测的行为：`touch maze.h && make` 重编**全部三个** `.o`
（三个 `.cpp` 都包含它），`touch solver.h` 只重编 `solver.o` 和 `main.o`。
`-MMD -MP` 生成的 `.d` 文件就是负责这件事的，没有它，改头文件 make 会认为「什么都没说变」。

---

## 验收标准

```bash
make p2        # 在仓库根目录
```

要求同时满足三件事：**零警告**（`-Wall -Wextra -Wpedantic` 全开）、**退出码 0**、
最后一行打印 `>>> maze_runner 全部通过`。少任何一条都算没做完。

下面是本项目**实跑**出来的全部关键数字（`main.cpp` 里 156 处 `assert` 钉的就是它们；
换机器、换编译器都应该一模一样，因为没有任何随机性和浮点数）：

| 内置迷宫 | 尺寸 | 能踩格数 | DFS 第一条路 | BFS 最短 | 从 S 踩到 | 连通块 | 各块大小 |
|---|---|---|---|---|---|---|---|
| `mazeTiny()` | 3×4 | 8 | 5 步 | **5 步** | 8 | 1 | [8] |
| `mazeClassic()` | 5×7 | 27 | **12 步** | **10 步** | 27 | 1 | [27] |
| `mazeNoWay()` | 4×4 | 7 | 不可达(-1) | 不可达(-1) | 5 | **2** | [5, 2] |
| `mazeIslands()` | 5×9 | 29 | 4 步 | 4 步 | 9 | **3** | [9, 12, 8] |

多源 BFS 的实测：`classic` 上 `{S,T}` 双源最远格是 `(0,6)`，8 步；
`tiny` 上 `{S,T}` 双源最远格是 `(2,0)`，3 步；`noWay` 双源点亮 7 格，`islands` 双源只点亮 9 格。

`mazeClassic()` 的 10 步不是本项目自己算出来就算了：它就是 `notes/11` 第 3 节
和 `exercises/ex12_bfs_containers.cpp` 里那张 5×7 图（只把起终点的 `.` 换成 `S`/`T`，
墙一根没动）。讲义那个 10 能在本项目里复现，说明两边有一致的基准。

`assert` 失败长这样，文件号和行号直接给你：

```
Assertion failed: dc.steps == 12, file main.cpp, line 259
```

**这就是本仓库的验收方式**：不靠你肉眼觉得「看起来能走」，靠机器停在错的那一行。
（规矩同 p1：`assert` 里的表达式不能有副作用 —— 加了 `-DNDEBUG` 整条语句会消失。
所以本项目一律先算进变量，再 assert 那个变量。）

---

## 你要动手做的 5 件事

按顺序做，每做完一件跑一次 `make p2`。**别改完三个再编译**，那时你分不清是哪一处改坏的。
期望值全部是本项目的图**实测**出来的，如果你的数字和这里不一样，就是你错了或你换了图。

### 任务 1：允许斜向，改成 8 邻域

把 `solver.cpp` 顶上的 `dx/dy` 从 4 对扩到 8 对（再加 `{-1,-1},{-1,1},{1,-1},{1,1}`），
其余代码一行不改。

- **期望**：`tiny` 从 5 步变 **3** 步，`classic` 从 10 步变 **6** 步；
  `mazeNoWay()` 和 `mazeIslands()` 的连通块数**不变**（墙还是墙，斜着也穿不过去），
  所以 `noWay` 仍然不可达；`islands` 的 BFS 仍然 4 步（它本来就是一条直道）。
  `main.cpp` 会当场炸给你看：`dc.visitedCells`、`bc.steps` 那几条对不上，
  而 `pathIsLegal` 里那句 `dr*dr + dc*dc != 1` 也会把新路径全判成非法 ——
  **改成 `<= 2` 就是 8 邻域版本的全部校验工作量**。
- **提示**：先预测「斜向会不会让连通块变多/变少」再动手。
  想清楚为什么斜向穿墙（两个对角都贴着墙的格子）**在这份代码里根本不会发生**。

### 任务 2：把最短路径印成一串方向字母

给 `PathResult` 加一个 `std::string moves`（或者写个 `pathToMoves()` 函数放在 `main.cpp`），
把 `(0,0)->(1,0)->(1,1)` 这种坐标串翻译成 `DR` 这种方向串：行 +1 是 `D`、-1 是 `U`、
列 +1 是 `R`、-1 是 `L`。

- **期望**：`classic` 的最短路径变成 **`DRRDRDRRDR`**（10 个字母，长度必须 == 步数）；
  `islands` 是 `DDDD`；`tiny` 是 `RRDRD`。加一条 `assert(moves.size() == (size_t)b.steps)`。
- **提示**：方向字母由**相邻两格的差**决定，不是由 `k` 决定 ——
  所以先在循环里算 `dr/dc`，再两两组合成 if/else（别写 `if (dr == -1 && dc == 0)` 之外漏掉斜向的情况，
  那正好能当任务 1 的探针）。想一想：为什么方向串比坐标串短一倍？

### 任务 3：最多砸一堵墙（洛谷 / LeetCode 风格经典题）

新增 `int bfsWithOneBreak(const Maze& m, std::pair<int,int> src, std::pair<int,int> dst)`：
允许把**至多一堵**墙变成通路（也可以一堵都不砸），求最短步数；砸了也不通就返回 -1。

- **期望输出**（新加一张内置迷宫 `Maze({"S#T", "...", "..."})`）：
  不砸墙是 **4** 步，允许砸一堵是 **2** 步；
  `mazeClassic()` 上答案是 **10**（最短路本来就不用穿墙，砸墙不加分）；
  `mazeNoWay()` 上答案是 **-1** —— 那两半之间隔着**两格**厚的墙，一堵不够，
  这道题拿它当边界测试最合适。
- **提示**：状态不再是「格子」，而是「格子 + 砸没砸过」，也就是每个格子拆成两个状态。
  这就是本仓库第一次让你体会到「**搜索的 state 要自己设计**」：
  `dist` 数组要多一维，`queue` 里要带上那个 flag，`visited` 也必须带上 flag
  （不然「先到的没砸墙、后到的砸了墙」会互相把对方挡掉，得到错误的 -1）。
  先写 `vector<vector<vector<int>>> dist(2, ...)`，写顺了再想怎么省。
  **答案我不给**，但你可以用 `notes/11` 第 1 节那三个问题（状态 / 初始 / 转移）先把自己问明白。

### 任务 4：数一数「不同的最短路径有几条」

新增 `int countShortestPaths(const Maze& m, std::pair<int,int> src, std::pair<int,int> dst)`。

- **期望**：`tiny` 是 **1** 条（它只有一条道）、`classic` 是 **4** 条、
  `{"S#T","...","..."}` 是 1 条、`noWay` 是 0 条。
- **提示**：BFS 扫到一个**还没访问过**的邻居时写距离，扫到一个**距离恰好等于自己 +1**
  的邻居时做什么？—— 注意别写「`dist != -1` 就 continue」然后忘了后面这一支。
  条数增长很快（`m×n` 的空图上是组合数），想想 `int` 什么时候不够用（notes/02 的溢出老账）。

### 任务 5：把 DFS 改成迭代栈，然后亲手把递归怼爆一次

用 `stack<pair<int,int>>`（notes/12 第 2 节）把 `dfsWalk` 重写成一个不递归的版本，
只要求 `visitedCells` 和可达性一致（路径可以不给）。然后：

- **期望**：四张内置图上结果完全一致（`assert` 一条都不用改就绿），
  说明「递归」只是「栈」的一种偷懒写法。
- 接着玩一次大的：临时造一张 `vector<string>` 拼出 600×600 的**螺旋形**长迷宫，
  跑递归版。notes/11 第 7 节第 4 条说本机 4.3 万层就崩 ——
  你会看到进程直接停了，**而且没有任何 assert 输出**。
- **提示**：崩了第一件事不是去搜「怎么加大栈」（MSVC 有个 `/STACK`、链接器有 `-Wl,--stack`，
  那些是把问题往后推），而是先想清楚：**我的递归深度是多少？为什么会这么大？**
  螺旋迷宫的 DFS 深度 ≈ 通路总长，那本来就是 O(n·m)，栈装不下；
  换成 BFS 或迭代栈才是答案。做完把这张大图删掉，别提交。

---

## 常见报错速查

| 现象 | 真实原因 | 怎么办 |
|---|---|---|
| `Permission denied` / `cannot execute binary file`，而且**是刚编译完第一件事** | Windows Defender 实时扫描临时锁住新生成的 `.exe` | 等一秒再 `make run`，不是代码问题。反复出现就把项目目录加进排除项 |
| `'make' 不是内部或外部命令` | 这台机器上只有 `mingw32-make` | 用 `mingw32-make p2`。它和 `make` 是**同一个 GNU Make**，只是 MinGW 为了不和 MSVC 的 `nmake` 撞名。嫌麻烦就 `alias make=mingw32-make` 写进 `~/.bashrc` |
| `Makefile:53: *** missing separator. Stop.` | Makefile 被存成 **CRLF**，或 recipe 行首用了**空格** | 编辑器右下角换行切 `LF`；recipe 行首必须是 Tab。`.gitattributes` 里已有 `* text=auto eol=lf` |
| 想清掉警告，加了 `-w` | 你在藏 bug，不是在解决 bug | **不许关警告**。本项目要求 `-Wall -Wextra -Wpedantic` 输出为空；看不懂的警告就逐字读它给的第一句 |
| `undefined reference to 'bfsShortest(Maze const&)'` | `SRCS` 里漏了 `solver.cpp`，或 `.cpp` 里的**签名**和 `.h` 里不逐字符相同（少个 `const` 就是另一个函数） | 先看 Makefile 的 `SRCS`，再对照声明和定义的参数表 |
| `multiple definition of 'mazeTiny()'` | 把函数的**定义**写进了头文件 | 头文件只留声明，定义搬回 `.cpp` |
| `redefinition of 'struct Maze'` | include 守卫漏了，或两个头文件用了同名的宏 | 宏名跟着文件名走：`MAZE_H` / `SOLVER_H` |
| `'Maze' was not declared in this scope` | `solver.h` 忘了 `#include "maze.h"` | 头文件要**自包含**：用到谁就 include 谁，别指望调用方帮你包 |
| `invalid types 'const int[int]' for array subscript` | 在 `.cpp` 里用了 `Maze`，但 `at()` 是成员函数被写成 `m.at[][]` | 网格在 `Maze` 内部是 `vector<string>`，对外只暴露 `at(r, c)` |
| 编译过了，`visitedCells` 却比格子总数大 / DFS 直接崩 | `vis` 标晚了或没标（**出队/递归之后才标**） | 顺序永远是：判合法 → **立刻标记** → 再展开。这条能躲掉一半的搜索坑 |
| `vector<bool>` 报 `cannot bind non-const lvalue reference` | `vis[r][c]` 返回的是代理对象，不是 `bool&` | 只读写值就没事；要引用就换 `vector<char>`（`notes/06` 第 4 节） |
| 路径打印出来看着对，但 `pathIsLegal` 返回 false | 中间有一格穿墙或斜走 —— 通常是 `dx/dy` 串了行和列 | 检查 `dx[k]` 是不是配给了 `r`；本项目坐标是 `(r, c)` |
| `Assertion failed: steps == 12`，但你没改过代码 | 你改了方向数组的顺序 / 换了地图 | 那是**性质之外**的数字，本来就会跟着顺序变。要么把顺序改回来，要么把 assert 换成 `>= bfs.steps` |
| 大图上程序直接退出，没有任何 assert 输出 | 递归爆栈（`notes/11` 第 7 节第 4 条） | 先算递归深度上限 = 通路长度，再决定改 BFS 还是迭代栈；**别去调栈大小** |
| `make run` 什么都不做，但程序明显有问题 | 目标没重建（时间戳没变） | `make rebuild` |

---

## 做完之后，自己想三个问题

1. `dfsWalk` 的递归深度上限 = 从起点出发的那条最长简单路径，最坏是「能踩的格子数」。
   `bfsPath` 的队列长度上限 = 最宽那一层的格子数。**同一张 1000×1000 的空迷宫，两者谁先撑不住？**
   （STM32 上这个问题不是理论题：栈是几百字节到几 KB，`queue` 是堆。
   哪一类搜索你敢在单片机上跑，取决于你**能不能事先算出最坏内存**。）
2. `render()` 返回 `vector<string>` 是为了能 assert。
   如果要把迷宫画到 STM32 的 128×64 单色屏上，`maze.*` 和 `solver.*` 哪些要动？
   （提示：一格画几个像素是**显示**的事，`Maze` 里不该知道屏幕尺寸；
   真正要重写的是 `printGrid`，这就是 p1 里「数据和展示分家」那条规则在这里的兑现。）
3. 现在 `passable` 只看字符。如果加一种「要拿到钥匙才能开的门」，
   状态要怎么改？（和任务 3 的「砸没砸过墙」是同一类问题 ——
   一旦答案需要多带一个 flag 进 `dist`，说明你的**状态定义**变了，
   这才是搜索题真正的难点，而不是模板本身。）
