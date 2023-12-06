# Directories
SRCDIR := .
BUILDDIR := build

FIND_CPP_FILES := find . \( -path './.*' -o -path './build*' \) -prune -o -type f \( -name '*.cpp' -o -name '*.h' \) -print

.PHONY: configure
configure:
	cmake -G Ninja -S $(SRCDIR) -B $(BUILDDIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

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