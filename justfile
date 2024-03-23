set export

BUILDDIR := "build"
CPP_FILES := `find . -type d \( -path './.*' -o -path './build*' -o -path './external*' \) -prune -o -type f \( -name '*.cpp' -o -name '*.h' \) -printf '%p '`
ENV_ROOT := `readlink -f .pixi/envs/default`

default_target := "all"
default_preset := "debug"

# Clean the build folder
clean:
	rm -rf {{BUILDDIR}}

# Install Eigen library
install-eigen:
	cd external/eigen && rm -rf build && cmake -G Ninja -S . -B build \
		-DCMAKE_INSTALL_PREFIX=../installed \
		-DCMAKE_TOOLCHAIN_FILE=../../toolchain/Clang.cmake \
		-DCMAKE_BUILD_TYPE=Release && \
	cmake --build build --target install && rm -rf build

# Install Catch library
install-catch:
	cd external/catch2 && rm -rf build && cmake -G Ninja -S . -B build \
		-DCMAKE_INSTALL_PREFIX=../installed \
		-DCMAKE_TOOLCHAIN_FILE=../../toolchain/Clang.cmake \
		-DCMAKE_BUILD_TYPE=Release && \
	cmake --build build --target install && rm -rf build


# Install all external libraries
install-external: install-catch install-eigen

# Clean the build folder and configure CMake
configure preset=default_preset:
	rm -rf {{BUILDDIR}}/{{preset}}
	cmake --preset {{preset}}

# Build a target
build preset=default_preset target=default_target:
	cmake --build --preset {{preset}} -v --target {{target}}

# Build a target, first cleaning and reconfiguring the build dir
build-new preset=default_preset target=default_target: (configure preset) (build preset target)

# Build a target and run it (from the directory of the binary)
run preset=default_preset target=default_target: (build preset target)
	find {{BUILDDIR}}/{{preset}} -type f -name "{{target}}" -exec sh -c 'cd $(dirname {}) && ./$(basename {})' \;

# Build the target with debug preset, and load it to the lldb debugger
debug target: (build "debug" target)
	cd build/debug/{{lowercamelcase(target)}} && lldb {{target}}

# Format all C++ files with clang-format
format:
	clang-format -style=file -i {{CPP_FILES}}

# Check formatting with clang-format
check-format:
	clang-format -style=file --dry-run --Werror {{CPP_FILES}}

# Lint all files with clang-tidy
lint preset=default_preset:
	clang-tidy -p {{BUILDDIR}}/{{preset}} {{CPP_FILES}}

# Run all tests
test preset=default_preset:
	ctest --preset {{preset}}
