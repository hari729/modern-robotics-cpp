cmake_minimum_required(VERSION 3.20)
project(mr VERSION 0.1.0 LANGUAGES CXX)

# ---- Options ----
option(MR_BUILD_TESTS "Build tests" ${PROJECT_IS_TOP_LEVEL})
option(MR_BUILD_EXAMPLES "Build examples" ${PROJECT_IS_TOP_LEVEL})
option(MR_INSTALL "Generate install targets" ${PROJECT_IS_TOP_LEVEL})

# ---- Helpers ----
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)

# ---- Dependencies ----
find_package(Eigen3 3.4 QUIET NO_MODULE)
if(NOT Eigen3_FOUND)
  message(STATUS "mr: Eigen3 not found; building without it")
endif()

# ---- Library ----
add_library(mr src/mr.cpp)
add_library(mr::mr ALIAS mr)

target_compile_features(mr PUBLIC cxx_std_20)
set_target_properties(mr PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(mr
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

mr_set_warnings(mr)

if(Eigen3_FOUND)
  set(MR_HAS_EIGEN3 ON)
  target_link_libraries(mr PUBLIC Eigen3::Eigen)
  target_compile_definitions(mr PUBLIC MR_HAS_EIGEN3)
else()
  set(MR_HAS_EIGEN3 OFF)
endif()

# ---- Examples ----
if(MR_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# ---- Tests ----
if(MR_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# ---- Install & Export ----
if(MR_INSTALL)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  install(TARGETS mr
    EXPORT mrTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(EXPORT mrTargets
    FILE mrTargets.cmake
    NAMESPACE mr::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mr
  )

  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/mrConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/mrConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mr
  )

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/mrConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/mrConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/mrConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mr
  )
endif()

# ---- CPack ----
if(PROJECT_IS_TOP_LEVEL)
  set(CPACK_PACKAGE_NAME mr)
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "mr: a C++ library")
  set(CPACK_GENERATOR "TGZ")
  include(CPack)
endif()
