# Daisy

Refactor of Daisy from https://github.com/perabrahamsen/daisy-model

## Building
[CMakePresets.json](CMakePresets.json) define setups for building with gcc and clang on linux.

    git clone git@github.com:daisy-model/daisy.git
    cd daisy
    mkdir build/release -p
    cd buld/release

To see a list of available presets

    cmake ../../ --list-presets

To build with gcc using release options

    cmake ../../ --preset gcc-release
    make
    
To build a portable statically linked binary

    cmake ../../ --preset gcc-release-portable
    make

## Testing
Test scenarios are in [test/dai_test_files](test/dai_test_files).
When adding a test you need to add it as a `dai_test_case` in [test/CMakeLists.txt](test/CMakeLists.txt).

Use [ctest](https://cmake.org/cmake/help/latest/manual/ctest.1.html) to run tests.

    ctest

## Code coverage
To get code coverage you need to set the build type to `Coverage`. This will disable some optimizations.
There is a coverage preset defined for gcc.

    git clone git@github.com:daisy-model/daisy.git
    cd daisy
    mkdir build/coverage -p
    cd build/coverage
    cmake ../.. --preset gcc-coverage
    make -j 8
    ctest

Use https://gcovr.com/en/stable/ to summarize coverage

    cd <coverage-build-dir>
    pip install gcovr
    gcovr -r ../../ . --html ../../test/coverage.html
