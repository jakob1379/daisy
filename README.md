# Daisy

Refactor of Daisy from https://github.com/perabrahamsen/daisy-model

## Installation

    git clone git@github.com:daisy-model/daisy.git
    cd daisy
    mkdir build/release -p
    cd buld/release
    cmake ../../
    make -j 8

## Testing
Test scenarios are in [test/dai_test_files](test/dai_test_files).
When adding a test you need to add it as a `dai_test_case` in [test/CMakeLists.txt](test/CMakeLists.txt).

Use [ctest](https://cmake.org/cmake/help/latest/manual/ctest.1.html) to run tests.

    ctest

## Code coverage
To get code coverage you need to set the build type to `Coverage`. This will disable some optimizations.

    cd <daisy-src-dir>
    mkdir build/coverage -p
    cd build/coverage
    cmake ../.. -DCMAKE_BUILD_TYPE=Coverage
    make -j 8
    ctest

Use https://gcovr.com/en/stable/ to summarize coverage

    cd <coverage-build-dir>
    pip install gcovr
    gcovr -r ../../ . --html ../../test/coverage.html
