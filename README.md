# Glucose SAT Solver

> This repository is a _fork_ of the original [Glucose SAT solver](https://github.com/audemard/glucose).

## Changes

This repository _preserves_ the original Glucose SAT solver code, but it has included some modifications to make it easier to use as a library in other projects.

- Improved [CMake configuration](CMakeLists.txt) to build a shared library.
- Added [C API](capi/) for easier integration with other languages.

Credits to [Christoph Jabs @chrjabs](https://github.com/chrjabs/glucose4) for some parts.

## Dev branches

The `fork` branch (_which you are currently looking at_) is just an empty placeholder.

All the development is located in `lipen/<version>` branches, where `<version>` is the version tag from the original repository, upon which the changes are based.

For example, to work with the forked version 4.2.1 (currently the latest upstream version), you can switch to the `lipen/4.2.1` branch.

```sh
git clone https://github.com/Lipen/glucose
cd glucose
git switch lipen/4.2.1
```

## Building

To build the Glucose SAT solver, you can use [CMake](https://cmake.org/).
The following commands will create a shared library `libglucose.so` in the `build` directory:

```sh
cmake -B build -DBUILD_SHARED_LIBS=ON -DBUILD_SYRUP=OFF -DBUILD_EXECUTABLES=OFF
cmake --build build
```
