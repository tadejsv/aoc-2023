# Directories
SRCDIR := .
BUILDDIR := build

FIND_CPP_FILES := find . \( -path './.*' -o -path './build*' \) -prune -o -type f \( -name '*.cpp' -o -name '*.h' \) -print

.PHONY: install-eigen
install-eigen:
	cd external/eigen && rm -rf build && cmake -G Ninja -S . -B build \
		-DCMAKE_INSTALL_PREFIX=../installed \
		-DCMAKE_TOOLCHAIN_FILE=../../toolchain/Build3rdParty.cmake && \
	cmake --build build --target install && rm -rf build

.PHONY: install-catch
install-catch:
	cd external/catch2 && rm -rf build && cmake -G Ninja -S . -B build \
		-DCMAKE_INSTALL_PREFIX=../installed \
		-DCMAKE_TOOLCHAIN_FILE=../../toolchain/Build3rdParty.cmake && \
	cmake --build build --target install && rm -rf build

.PHONY: install-external
install-external: install-catch install-eigen

.PHONY: configure
configure:
	rm -rf build && \
	cmake -G Ninja -S $(SRCDIR) -B $(BUILDDIR) \
		-DCMAKE_TOOLCHAIN_FILE=toolchain/Dev.cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DCMAKE_PREFIX_PATH=external/installed

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

.PHONY: test
test:
	ctest --test-dir build -T test -j10 --output-on-failure