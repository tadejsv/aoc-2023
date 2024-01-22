TODO:
* Learn about generator tests, and implement some
* CI/CD: linting, testing (+ build). WOuld be great to later also add building (a completely static executable)
* Proper error handling and executions

## Build and install external dependencies

This project uses 2 external dependencies: `Eigen` (for matrices) and `Catch2`.

To install them, execute

```
make install-external
```

This only has to be done once.


### How the installation works

This make action will install (and build) the dependencies using CMake from their source code in `external/`.

The libraries and headers will be installed to `external/installed`.

For installation, a toolchain file (`toolchain/Build3rdParty.cmake`) is used, which sets up the same compiler and associate utils and C++ standard library as is used for our executables (for compatibility), but with no linting/warnings.

### Why not install using pixi?

We could install using pixi, which would install already built libraries and headers (along with CMake config files) to `.pixi/env`. For a header-only library like `Eigen`, this would work.

However, for compiled libraries, like `Catch2`, this would fail. This is because the libraries installed by `pixi` are built using the conda toolchain (which, among other things, uses `stdlibc++` instead of `libc++` that we are using). This is not compatible with our toolchain (using `llvm` things as much as possible), so if we try to compile our executables, we would get a lot of linker errors.

### Why not use `FetchContent` or `add_subdirectory`?

This option would build the external libraries together with our executables - unlike with `pixi`-installed libraries, this would work, but the downside is that we would get a lot of warnings from our linter for these external libraries, and we would need to re-build the libraries every time we want to build form scratch (deleting the `build/` directory), which would significantly increase the build times.