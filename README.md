# Glucose SAT Solver

This repository mirrors the [Glucose project](https://www.labri.fr/perso/lsimon/glucose/) on GitHub.

The original instructions for building the software are in `README`.

## Dependencies

### CMake

Differently from the original source code, we use [CMake](https://cmake.org/) as a build tool.

On Ubuntu:

```shell
sudo apt-get install cmake
```

### zlib

Glucose depends on [zlib](https://zlib.net/).

On Ubuntu:

```shell
sudo apt-get install zlib-dev
```

## Build

To build the software, run the following commands:

```shell
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
sudo cmake --install build --strip --prefix install
```

The produced artifacts will be:

- `libglucose.a` and `libglucosep.a`: the static libraries for the simple and parallel version of the SAT solver, respectively.
- `glucose-simp` and `glucose-syrup`: the binaries for the simple and parallel version of the SAT solver, respectively.
