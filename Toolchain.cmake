set(PIXI_ENV_ROOT $ENV{PWD}/.pixi/env)

set(LINKER_FLAGS -fuse-ld=lld -Wl,-rpath,${PIXI_ENV_ROOT}/lib -Wl,-rpath-link,${PIXI_ENV_ROOT}/lib -L${PIXI_ENV_ROOT}/lib)

set(CMAKE_C_COMPILER ${PIXI_ENV_ROOT}/bin/clang)
set(CMAKE_CXX_COMPILER ${PIXI_ENV_ROOT}/bin/clang++)
set(CMAKE_AR ${PIXI_ENV_ROOT}/bin/llvm-ar)
set(CMAKE_RANLIB ${PIXI_ENV_ROOT}/bin/llvm-ranlib)

set(CMAKE_CXX_CLANG_TIDY clang-tidy)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE include-what-you-use)
set(LINK_WHAT_YOU_USE ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -stdlib=libc++)
add_link_options(-stdlib=libc++ ${LINKER_FLAGS})
