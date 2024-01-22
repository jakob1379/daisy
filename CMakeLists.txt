cmake_minimum_required(VERSION 3.25)
project(
  daisy
  VERSION 6.44
  LANGUAGES CXX
)

include(cmake/AddCoverageBuildType.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_PROFILE "Set to ON to build for profiling" OFF)
option(MAKE_PORTABLE "Set to ON to make a generic build" OFF)

IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Release
       CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel Coverage."
       FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

add_compile_options(${COMPILER_OPTIONS_WARNING})

find_package(Boost 1.74 REQUIRED CONFIG COMPONENTS filesystem system)
message(STATUS "Boost version: ${Boost_VERSION}")

add_executable(daisy)

# Sources are added with target_sources in CMakeLists in the source tree
add_subdirectory(src)


target_include_directories(daisy PUBLIC include)
target_link_directories(daisy INTERFACE Boost::Boost)
target_link_libraries(daisy PUBLIC
  stdc++
  ${Boost_LIBRARIES}
  cxsparse
  dl
  pthread
)

target_compile_options(daisy PRIVATE
  "$<$<CONFIG:RELEASE>:${COMPILER_OPTIONS_RELEASE}>"
  "$<$<CONFIG:COVERAGE>:${COMPILER_OPTIONS_COVERAGE}>"
  "$<$<BOOL:${USE_PROFILE}>:${COMPILER_OPTIONS_PROFILE}>"
  "$<IF:$<BOOL:${MAKE_PORTABLE}>,${COMPILER_OPTIONS_PORTABLE},${COMPILER_OPTIONS_NATIVE}>"
)


target_link_options(daisy PRIVATE
  "$<$<CONFIG:COVERAGE>:${LINKER_OPTIONS_COVERAGE}>"
)




install(TARGETS daisy DESTINATION bin)

# Contains .dai files that define functionality
add_subdirectory(lib)

include(CTest)
add_subdirectory(test)
