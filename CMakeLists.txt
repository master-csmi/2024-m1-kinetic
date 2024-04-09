cmake_minimum_required(VERSION 3.22)
project(kinetic VERSION 0.1 LANGUAGES CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

# add_executable(kinetic main.cpp)
# target_include_directories(kinetic PUBLIC 
#     ${CMAKE_SOURCE_DIR}/cgal/Kinetic_surface_reconstruction/include
#     ${CMAKE_SOURCE_DIR}/cgal/Installation/include
#     )

add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)

set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} eigen/3.4.0)
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} boost/1.66)
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} Qt/6.4)
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} gmp/6.3.0)
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} mpfr/4.2.1)

include(cmake/Conan.cmake)
run_conan()

add_subdirectory(src)