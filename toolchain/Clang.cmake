set(CMAKE_C_COMPILER $ENV{ENV_ROOT}/bin/clang)
set(CMAKE_CXX_COMPILER $ENV{ENV_ROOT}/bin/clang++)
set(CMAKE_AR $ENV{ENV_ROOT}/bin/llvm-ar)
set(CMAKE_RANLIB $ENV{ENV_ROOT}/bin/llvm-ranlib)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-stdlib=libc++)

set(LINKER_FLAGS -fuse-ld=lld -Wl,-rpath,$ENV{ENV_ROOT}/lib -Wl,-rpath-link,$ENV{ENV_ROOT}/lib -L$ENV{ENV_ROOT}/lib)
add_link_options(${LINKER_FLAGS} -stdlib=libc++)
