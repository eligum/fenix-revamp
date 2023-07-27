# Fenix

A 3D model visualizer.

## Build Instructions

Make sure you have a working git-installation. Open a command prompt and clone the repository via:

```sh
git clone https://github.com/eligum/fenix-revamp.git
```

### Using CMake directly

Download and install [CMake](https://cmake.org/) and execute the following commands:

```sh
cd fenix-revamp
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DWARNINGS_AS_ERRORS=FALSE
cmake --build build -j 4
```

There is also a `Makefile` with all the necessary targets to generate, build and run
the project. Inside the makefile you can specify which type of build do you want.
Most common ones are:

* Release (optimize for speed)
* Debug
* RelWithDebInfo (optimized with debug information)
* MinSizeRel (optimize for size)

### Using Visual Studio Code

The good thing about CMake is that it is cross-platform and supports a lot of different
compilers. So you can use your prefered IDE + compiler for development.

If you are using [Visual Studio Code](https://code.visualstudio.com/), this repository
includes a `.vscode` folder with a series of tasks and code snippets configured to make
development easier. You can compile the project with `ctrl+shift+B`, even start a debugging
session by pressing `F5` if you have [lldb](https://lldb.llvm.org/) installed.
