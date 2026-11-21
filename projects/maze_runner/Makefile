# =============================================================================
# 迷宫求解器 · Makefile
# -----------------------------------------------------------------------------
# 这个文件负责什么：把 3 个 .cpp 分开编译成 .o，再链接成一个 .exe。
# 和 projects/score_report/Makefile 完全同构 —— 换一个项目只需要改三处：
# TARGET 的名字、SRCS 的清单、HDRS 的清单。规则一条都不用动。
#
# 屏幕上的四行输出就是「分离编译」的现场：
#   [CXX]  maze.cpp   -> build/maze.o
#   [CXX]  solver.cpp -> build/solver.o     <- 三个互不相干的编译单元
#   [CXX]  main.cpp   -> build/main.o
#   [LINK] build/maze.o build/solver.o build/main.o -> build/maze_runner.exe
#
# 为什么这么麻烦：改了 solver.cpp 里的一处剪枝，只有 solver.o 需要重编，
# maze.o / main.o 一个字节都不动。STM32 工程动辄几百个 .c，
# 全量重编要几分钟，增量只要几秒 —— Make 干的就是「谁变了，只重编依赖它的东西」。
#
# 常用命令：
#   make            等价于 make all，只编译不运行
#   make run        编译并运行（仓库根目录的 make p2 就是调这个）
#   make clean      删掉 build/ 里的产物（仓库根的 make clean 会调它）
#   make rebuild    clean + all（改了这个 Makefile 之后用）
#   make help       列出所有目标
#
# ⚠️ 本文件必须用 LF 换行，recipe 行首必须是 Tab 不能是空格 ——
#    混进 CRLF 或空格，make 会报 "missing separator"（详见 README 的报错速查）。
# =============================================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -O0
BUILD    := build
TARGET   := $(BUILD)/maze_runner.exe

# 源文件写死，不用 $(wildcard *.cpp)：哪些文件参与链接必须是一张看得见的清单。
# 「少把一个 .cpp 加进来」是 undefined reference 的头号成因，写死能逼你看一眼。
SRCS   := maze.cpp solver.cpp main.cpp
OBJS   := $(addprefix $(BUILD)/,$(SRCS:.cpp=.o))
DEPS   := $(OBJS:.o=.d)
HDRS   := maze.h solver.h

.PHONY: all run clean rebuild help
.DEFAULT_GOAL := all

help:
	@echo ""
	@echo "  make / make all     编译链接 -> $(TARGET)"
	@echo "  make run            编译并运行（main.cpp 里有一整套 assert）"
	@echo "  make rebuild        clean + all"
	@echo "  make clean          删除 $(BUILD)/ 里的 .o .d .exe"
	@echo ""
	@echo "  源文件：$(SRCS)"
	@echo "  头文件：$(HDRS)"
	@echo ""

all: $(TARGET)

# 目录不存在就先建。竖线 | 后面的是「订单式前提」（order-only prerequisite）：
# 只管「它得存在」，不管它的时间戳，否则每次 make 都会因为目录 mtime 变了而重编。
$(BUILD):
	@mkdir -p $(BUILD)

# 每个 .cpp 单独出一个 .o —— 分离编译的全部秘密就在这一条模式规则里。
# -MMD 让 g++ 顺手生成 .d 文件（记录「这个 .o 依赖哪些头文件」），
# -MP 给每个头文件补一条空规则，删掉头文件时 make 不会报 No rule to make target。
$(BUILD)/%.o: %.cpp | $(BUILD)
	@echo "  [CXX]  $< -> $@"
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(TARGET): $(OBJS)
	@echo "  [LINK] $^ -> $@"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@echo "  完成：$@"

# 把自动生成的依赖关系读进来。改 maze.h 之后 make run 会重编全部三个 .o
# （三个 .cpp 都包含它），改 solver.h 只重编 solver.o 和 main.o。
-include $(DEPS)

run: $(TARGET)
	@./$(TARGET)

rebuild: clean all

clean:
	@rm -f $(OBJS) $(DEPS) $(TARGET)
	@rmdir $(BUILD) 2>/dev/null || true
	@echo "  已清理 $(BUILD)/ 下的产物（源码一个没动）"
