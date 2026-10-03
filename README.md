# pkg

A minimal C++20 library template with CMake: proper targets, install/export
for `find_package` consumers, tests, presets, CI, and packaging.

## Layout

```
.
├── CMakeLists.txt              # project, library, options, install/export, CPack
├── CMakePresets.json           # dev / release / ci presets
├── cmake/
│   ├── CompilerWarnings.cmake  # warning flags helper
│   └── pkgConfig.cmake.in      # template for the consumer-facing pkgConfig.cmake
├── include/pkg/pkg.hpp         # public headers (installed)
├── src/pkg.cpp                 # library implementation
├── examples/
│   ├── CMakeLists.txt
│   └── main.cpp                # example executable linking pkg::pkg
├── tests/
│   ├── CMakeLists.txt          # GoogleTest via FetchContent, CTest registration
│   ├── test_pkg.cpp            # unit tests
│   └── consumer/               # standalone project that does find_package(pkg)
│       ├── CMakeLists.txt
│       └── main.cpp
├── .github/workflows/ci.yml    # build, test, install, consumer check (GCC + Clang)
└── init.sh                     # renames pkg/PKG placeholders to a new project name
```

## How it works

### Targets

`add_library(pkg ...)` creates the library, and
`add_library(pkg::pkg ALIAS pkg)` creates a namespaced alias. Code inside the
repo links the alias, so it looks exactly the same as code consuming the
installed package — if it builds here, it builds downstream.

### Include directories

```cmake
target_include_directories(pkg
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
```

`PUBLIC` means "I need it, and so do consumers". The two generator expressions
give different paths at build time (the repo's `include/`) and after install
(`CMAKE_INSTALL_INCLUDEDIR`, usually `include` under the prefix).

### Options and `PROJECT_IS_TOP_LEVEL`

`PKG_BUILD_TESTS`, `PKG_BUILD_EXAMPLES`, and `PKG_INSTALL` default to
`PROJECT_IS_TOP_LEVEL`. When the repo is built directly they are ON; when it
is pulled into another project with `add_subdirectory`/`FetchContent` they
are OFF, so the parent isn't forced to build tests or inherit install rules.

### Presets

`CMakePresets.json` pins the generator (Ninja), binary dir (`build/<preset>`),
and install prefix (`install/<preset>`), and turns compile commands export on
for clangd. `dev` is Debug, `release` is Release, `ci` is what the workflow
uses.

### Tests

`tests/CMakeLists.txt` fetches GoogleTest with `FetchContent` (no system
package needed) and registers each test with `gtest_discover_tests` so CTest
sees them individually. `INSTALL_GTEST OFF` stops GoogleTest's headers from
being installed alongside the library.

### Install and export

The `install(...)` block does four things:

1. `install(TARGETS pkg EXPORT pkgTargets ...)` — puts the library file in
   `lib/` and records it in an export set.
2. `install(DIRECTORY include/ ...)` — copies public headers.
3. `install(EXPORT pkgTargets ...)` — generates `pkgTargets.cmake`, which
   recreates the `pkg::pkg` imported target with the right install paths.
4. `configure_package_config_file` + `write_basic_package_version_file` —
   generate `pkgConfig.cmake` (from `cmake/pkgConfig.cmake.in`) and
   `pkgConfigVersion.cmake`, the two files `find_package(pkg)` looks for.

`pkgConfig.cmake.in` runs `find_dependency` for packages the installed
library needs, then includes the targets file:

```cmake
@PACKAGE_INIT@
include(CMakeFindDependencyMacro)
if(@PKG_HAS_EIGEN3@)
  find_dependency(Eigen3 3.4 NO_MODULE)
endif()
include("${CMAKE_CURRENT_LIST_DIR}/pkgTargets.cmake")
check_required_components(pkg)
```

### Optional Eigen

`Eigen3` is found with `find_package(... QUIET)`. If present, the library
links `Eigen3::Eigen` `PUBLIC` and the generated config calls
`find_dependency(Eigen3 ...)`, so consumers get it transitively. If absent,
both the build and the config skip it. `PKG_HAS_EIGEN3` is set at configure
time and substituted into the config template.

### CPack

When the project is top-level, `include(CPack)` generates a `.tar.gz` of the
installed tree with `cpack --config build/dev/CPackConfig.cmake`.

## Commands

```bash
# configure, build, test
cmake --preset dev
cmake --build build/dev
ctest --preset dev

# run the example
./build/dev/examples/pkg_example

# install to ./install/dev (prefix set by the preset)
cmake --build build/dev --target install

# verify a downstream project can find it
cmake -S tests/consumer -B build/consumer \
  -DCMAKE_PREFIX_PATH="$PWD/install/dev"
cmake --build build/consumer
./build/consumer/consumer

# create a .tar.gz package
cpack -B build/cpack --config build/dev/CPackConfig.cmake
```

A downstream `CMakeLists.txt` then only needs:

```cmake
find_package(pkg REQUIRED)
target_link_libraries(app PRIVATE pkg::pkg)
```

## Reusing as a template

```bash
./init.sh myproject
```

This replaces `pkg`/`PKG` in all file contents, renames `include/pkg/` and
any `pkg*` files, then deletes itself.

## Notes

- CI (`.github/workflows/ci.yml`) runs the full build → test → install →
  consumer-check flow on GCC and Clang. It has not been run from this repo
  yet.
- The `.tar.gz` from CPack contains the installed tree (headers, library,
  CMake config files).
