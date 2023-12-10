# Directories
SRCDIR := .
BUILDDIR := build
LDFLAGS = "-Wl,-rpath,/home/tadej/projects/aoc-2023/.pixi/env/lib -Wl,-rpath-link,/home/tadej/projects/aoc-2023/.pixi/env/lib -L/home/tadej/projects/aoc-2023/.pixi/env/lib"

FIND_CPP_FILES := find . \( -path './.*' -o -path './build*' \) -prune -o -type f \( -name '*.cpp' -o -name '*.h' \) -print

.PHONY: configure
configure:
	LDFLAGS=${LDFLAGS} cmake -G Ninja -S $(SRCDIR) -B $(BUILDDIR) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DCMAKE_C_COMPILER=/home/tadej/projects/aoc-2023/.pixi/env/bin/clang \
		-DCMAKE_CXX_COMPILER=/home/tadej/projects/aoc-2023/.pixi/env/bin/clang++ \
		-DCMAKE_CXX_FLAGS="-fuse-ld=/home/tadej/projects/aoc-2023/.pixi/env/bin/ld.lld" \
		-DCMAKE_AR=/home/tadej/projects/aoc-2023/.pixi/env/bin/llvm-ar \
		-DCMAKE_RANLIB=/home/tadej/projects/aoc-2023/.pixi/env/bin/llvm-ranlib

.PHONY: build
build: configure
	cmake --build $(BUILDDIR) -v

.PHONY: clean
clean:
	@rm -rf $(BUILDDIR)

.PHONY: format
format:
	clang-format -style=file -i $$( $(FIND_CPP_FILES) ) 

.PHONY: check-format
check-format:
	clang-format -style=file --dry-run --Werror $$( $(FIND_CPP_FILES) )

.PHONY: lint
lint:
	clang-tidy $$( $(FIND_CPP_FILES) ) -p build --enable-check-profile