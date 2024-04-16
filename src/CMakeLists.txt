add_executable(kinetic main.cpp)
target_link_libraries(
  kinetic
  PRIVATE project_options
          # project_warnings
          CONAN_PKG::gmp
          CONAN_PKG::mpfr
          CONAN_PKG::eigen

          )

target_include_directories(kinetic PUBLIC
  ${CMAKE_SOURCE_DIR}/cgal/Kinetic_surface_reconstruction/include
  ${CMAKE_SOURCE_DIR}/cgal/Installation/include
  ${CMAKE_SOURCE_DIR}/cgal/STL_Extension/include
  ${CMAKE_SOURCE_DIR}/cgal/Kinetic_space_partition/include
  ${CMAKE_SOURCE_DIR}/cgal/BGL/include/
  ${CMAKE_SOURCE_DIR}/cgal/Kernel_23/include
  ${CMAKE_SOURCE_DIR}/cgal/Profiling_tools/include/
  ${CMAKE_SOURCE_DIR}/cgal/Stream_support/include
  ${CMAKE_SOURCE_DIR}/cgal/Number_types/include
  ${CMAKE_SOURCE_DIR}/cgal/Algebraic_foundations/include
  ${CMAKE_SOURCE_DIR}/cgal/Interval_support/include
  ${CMAKE_SOURCE_DIR}/cgal/Modular_arithmetic/include
  ${CMAKE_SOURCE_DIR}/cgal/Property_map/include
)