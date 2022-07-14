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

- On Linux:

  ```shell
  cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
  cmake --build build
  cmake --install build --strip --prefix install
  ```

  The produced artifacts (in `install/`) will be:

  - `libglucose.a` and `libglucosep.a`: the static libraries for the simp and parallel version of the Glucose SAT solver, respectively.
  - `glucose-simp` and `glucose-syrup`: the binaries for the simp and parallel version of the Glucose SAT solver, respectively.

* On Windows:

  ```cmd
  cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -G "MinGW Makefiles"
  cmake --build build --target glucose
  cmake --install build --strip --prefix install
  ```

  The produced artifacts (in `install/`) will be:

  - `libglucose.a`: the static library for the simp version of the Glucose SAT solver.
  - `glucose.dll`: the dynamic library for the simp version of the Glucose SAT solver.
