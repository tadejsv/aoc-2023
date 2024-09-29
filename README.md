

## Build and install external dependencies

This project uses 2 external dependencies: `Eigen` (for matrices) and `Catch2`.

To install them, execute

```
just install-external
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

### Why doesn't the installation use CMake presets?

Unlike the compilation of the executables (exercises) in this repo, we can not use CMake presets to specify the CMake configuration for external dependencies. This is because unfortunately, `CMakePresets.json` file would need to be in the source root directory (the file path is hardcoded in CMake).

For now, this is not a big issue - we are only building for one type of configuration, so we can just write out all the arguments manually (and we do reuse the CLang toolchain file). However, if we had more configurations (different operating systems or different architectures), this would get unmanagable.

In that case, the only option is forking the 3rd party repository, and inserting / replacin the `CMakePresets.json` file, and importing from our main presets file using `include`.

## Setting up CMake configuration

Before compiling, you must prepare the `CMake` configuration. This can be done with

```
just configure
```

or also as part of the build step if using `just build-new` (see below). By default, the `debug` preset will be used for building. You can choose a different preset with

```
just configure PRESET_NAME
```

To see a list of available presets use

```
cmake --list-presets
```

We currently have a `debug` preset (using a `Debug` build type), a `release` preset (using `Relase` build type) and the `ci` preset (same as release, but with `-Werror` configuration arg added).

This command will create the build folder - either `build/debug` or `build/release`.

## Compiling

To compile from scratch (deleting any existing build folder, and re-running the configuration step before compiling), run

```
just build-new
```

this will delete the `build/PRESET_NAME` directory, configure CMake and finally run CMake build step. Just as with the `just configure` command, you can also set a preset here (with `debug` as the default).

To build only binaries for changed code, run

```
just build
```

this simply runs the CMake build step, which will not re-build parts that have no changed code. You can also set the preset here.

To build and run the executable of a specific target, use

```
just run PRESET_NAME TARGET_NAME
```

Both of these arguments must be provided.

## Testing

You can run all tests with

```
just test
```

This should be done after the build step. You can also set the preset here.

## Debugging

For the lack of good LLDB (linux) integration with, debug is done from the command line.

You just need to execute

```
just debug TARGET_NAME
```

and the target will automatically be built, and lldb debugger will be started with the target binary already loaded.

From there on, you can consult the [LLDB documentation](https://lldb.llvm.org/use/tutorial.html) for how to set breakpoints, run the program, inspect the state, etc. For example, setting the breakpoints is very simple (and by some magic there will be no problems with file paths):

```lldb
(lldb) breakpoint set --file main2.cpp --line 90
```

this will set the breakpoint at line 90 of the the file `main2.cpp` for your current target.

TODO: useful lldb commands

## Linting

For linting, you should install the `clangd` VSCode extension (and make sure you have IntelliSense disabled). This
will give you very fast linting, performing the same checks as `clang-tidy`, but it will not attempt to lint
system headers. On compilation, you will also get linting from `iwyu`.

You can run linting separately as well (after your build step), using

```
just lint
```

You can also set the preset here.

### Linting in CI

In CI, the `just lint` command is run for linting

## Formatting

To format using `clang-format`, use

```
just format
```

You can also check that formatting is correct using

```
just check-format
```

hello!
