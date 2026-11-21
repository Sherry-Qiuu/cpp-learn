CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -O0 -fno-omit-frame-pointer
STRICTFLAGS := $(CXXFLAGS) -Wconversion -Wsign-conversion -Wshadow -Wcast-qual -fanalyzer
BUILD    := build

EX := $(sort $(wildcard exercises/ex*.cpp))

.PHONY: help list all-ex run-all clean p1 p2
.DEFAULT_GOAL := help

help:
	@echo ""
	@echo "  make list          列出所有练习"
	@echo "  make run/ex01      编译并运行 exercises/ex01_*.cpp（短名、全名都认）"
	@echo "  make asm/ex01      生成 build/ex01.s，看编译器怎么理解你的代码"
	@echo "  make strict/ex01   严格警告 + -fanalyzer 静态分析（本机可用，推荐）"
	@echo "  make san/ex01      ASan+UBSan（本机 MinGW 缺库，会明确告诉你）"
	@echo "  make all-ex        编译全部练习（不运行，只看能不能过）"
	@echo "  make run-all       编译并依次运行全部练习（答案写完后用这个验收）"
	@echo "  make p1            编译并运行 projects/score_report"
	@echo "  make p2            编译并运行 projects/maze_runner"
	@echo "  make clean         清空 build/，源码不动"
	@echo ""
	@echo "  注意：本机 make 的真名是 mingw32-make，在 ~/.bashrc 里加 alias make=mingw32-make"
	@echo ""

list:
	@echo "可用练习："
	@for f in $(EX); do echo "  $$(basename $$f .cpp)"; done

$(BUILD):
	@mkdir -p $(BUILD)

all-ex: $(BUILD)
	@for f in $(EX); do \
	  n=$$(basename $$f .cpp); \
	  printf "%-30s " "$$n"; \
	  if $(CXX) $(CXXFLAGS) $$f -o $(BUILD)/$$n.exe 2>$(BUILD)/$$n.log; then \
	    echo "OK"; \
	  else \
	    echo "FAILED  (看 $(BUILD)/$$n.log)"; \
	    cat $(BUILD)/$$n.log; \
	  fi; \
	done

run-all: $(BUILD)
	@for f in $(EX); do \
	  n=$$(basename $$f .cpp); \
	  printf "\n========== %s ==========\n" "$$n"; \
	  if $(CXX) $(CXXFLAGS) $$f -o $(BUILD)/$$n.exe 2>/dev/null; then \
	    ./$(BUILD)/$$n.exe || echo "  [X] $$n 断言失败或运行出错"; \
	  else \
	    echo "  [!] $$n 编译失败"; \
	  fi; \
	done
	@echo ""
	@echo ">>> 全部跑完。任何一个 assert 失败都会打印文件名和行号。"

run/%: | $(BUILD)
	@src=$(firstword $(wildcard exercises/$*.cpp exercises/$*_*.cpp)); \
	 if [ -z "$$src" ]; then echo "找不到练习 "$*"，用 make list 看全部练习名"; exit 1; fi; \
	 $(CXX) $(CXXFLAGS) $$src -o $(BUILD)/$*.exe && echo "--- 运行 $* ---" && ./$(BUILD)/$*.exe

san/%: | $(BUILD)
	@src=$(firstword $(wildcard exercises/$*.cpp exercises/$*_*.cpp)); \
	 if [ -z "$$src" ]; then echo "找不到练习 "$*"，用 make list 看全部练习名"; exit 1; fi; \
	 if $(CXX) $(CXXFLAGS) -fsanitize=address,undefined $$src -o $(BUILD)/$*_san.exe 2>$(BUILD)/$*_san.log; then \
	   echo "--- ASan+UBSan 运行 $* ---" && ./$(BUILD)/$*_san.exe; \
	 else \
	   echo "[!] 本机 MinGW 没有 libasan / libubsan（ld: cannot find -lasan），这个目标在 Windows 上用不了。"; \
	   echo "    这是工具链的限制，不是你代码的问题。改用 make strict/$*（本机可用），"; \
	   echo "    或者到 WSL / Linux 上再跑 san。"; \
	 fi

strict/%: | $(BUILD)
	@src=$(firstword $(wildcard exercises/$*.cpp exercises/$*_*.cpp)); \
	 if [ -z "$$src" ]; then echo "找不到练习 "$*"，用 make list 看全部练习名"; exit 1; fi; \
	 rm -f $(BUILD)/$*_strict.exe; \
	 echo "--- 严格模式编译 $*（-Wconversion -Wsign-conversion -Wshadow -Wcast-qual -fanalyzer）---"; \
	 echo "--- 这些警告是「信号」不是「错误」：逐条看懂，别用 -w 一口气关掉 ---"; \
	 $(CXX) $(STRICTFLAGS) $$src -o $(BUILD)/$*_strict.exe 2>$(BUILD)/$*_strict.log; \
	 if [ -s $(BUILD)/$*_strict.log ]; then head -30 $(BUILD)/$*_strict.log; else echo "  零警告通过（很干净）"; fi; \
	 if [ -f $(BUILD)/$*_strict.exe ]; then echo "--- 运行 ---"; ./$(BUILD)/$*_strict.exe; fi

asm/%: | $(BUILD)
	@src=$(firstword $(wildcard exercises/$*.cpp exercises/$*_*.cpp)); \
	 if [ -z "$$src" ]; then echo "找不到练习 "$*"，用 make list 看全部练习名"; exit 1; fi; \
	 $(CXX) $(CXXFLAGS) -S $$src -o $(BUILD)/$*.s && echo "已生成 $(BUILD)/$*.s（用 VS Code 打开看）"

p1:
	@$(MAKE) -C projects/score_report run

p2:
	@$(MAKE) -C projects/maze_runner run

clean:
	@rm -rf $(BUILD)
	@mkdir -p $(BUILD)
	@$(MAKE) -C projects/score_report clean
	@$(MAKE) -C projects/maze_runner clean
	@echo "build/ 与两个项目的产物已清空"
