add_executable(kinetic main.cpp)

find_package(CGAL COMPONENTS Qt5)

target_link_libraries(
  kinetic
  PRIVATE project_options
          # project_warnings
          CONAN_PKG::gmp
          CONAN_PKG::mpfr
          CONAN_PKG::eigen

          )


# Ensure that the CGAL directory is set and exists
set(CGAL_DIR "${CMAKE_SOURCE_DIR}/cgal")

if(NOT EXISTS "${CGAL_DIR}")
  message(FATAL_ERROR "CGAL directory not found: ${CGAL_DIR}")
endif()

# look in ${CMAKE_SOURCE_DIR}/cgal and check all subdirectories if a subfolder is called include
# if yes than add it to the list
file(GLOB children RELATIVE "${CGAL_DIR}" "${CGAL_DIR}/*")

foreach(child ${children})
  set(dir "${CGAL_DIR}/${child}")

  if(IS_DIRECTORY "${dir}")
    if(EXISTS "${dir}/include")
      target_include_directories(kinetic PRIVATE "${dir}/include")
    endif()
  endif()
endforeach()