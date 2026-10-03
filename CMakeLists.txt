cmake_minimum_required(VERSION 3.20)
project(pkg VERSION 0.1.0 LANGUAGES CXX)

# ---- Options ----
option(PKG_BUILD_TESTS "Build tests" ${PROJECT_IS_TOP_LEVEL})
option(PKG_BUILD_EXAMPLES "Build examples" ${PROJECT_IS_TOP_LEVEL})
option(PKG_INSTALL "Generate install targets" ${PROJECT_IS_TOP_LEVEL})

# ---- Helpers ----
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)

# ---- Dependencies ----
find_package(Eigen3 3.4 QUIET NO_MODULE)
if(NOT Eigen3_FOUND)
  message(STATUS "pkg: Eigen3 not found; building without it")
endif()

# ---- Library ----
add_library(pkg src/pkg.cpp)
add_library(pkg::pkg ALIAS pkg)

target_compile_features(pkg PUBLIC cxx_std_20)
set_target_properties(pkg PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(pkg
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

pkg_set_warnings(pkg)

if(Eigen3_FOUND)
  set(PKG_HAS_EIGEN3 ON)
  target_link_libraries(pkg PUBLIC Eigen3::Eigen)
  target_compile_definitions(pkg PUBLIC PKG_HAS_EIGEN3)
else()
  set(PKG_HAS_EIGEN3 OFF)
endif()

# ---- Examples ----
if(PKG_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# ---- Tests ----
if(PKG_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# ---- Install & Export ----
if(PKG_INSTALL)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  install(TARGETS pkg
    EXPORT pkgTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(EXPORT pkgTargets
    FILE pkgTargets.cmake
    NAMESPACE pkg::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pkg
  )

  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkgConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/pkgConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pkg
  )

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/pkgConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pkgConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/pkgConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pkg
  )
endif()

# ---- CPack ----
if(PROJECT_IS_TOP_LEVEL)
  set(CPACK_PACKAGE_NAME pkg)
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "pkg: a C++ library")
  set(CPACK_GENERATOR "TGZ")
  include(CPack)
endif()
