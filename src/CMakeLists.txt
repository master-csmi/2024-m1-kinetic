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
    )