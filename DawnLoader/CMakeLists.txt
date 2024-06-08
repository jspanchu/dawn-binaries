cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(LoadableDawn)

include(FetchContent)

FetchContent_Declare(dawn
  GIT_REPOSITORY https://dawn.googlesource.com/dawn
  GIT_TAG        chromium/6526
  GIT_SUBMODULES ""
  GIT_SHALLOW    ON
  PATCH_COMMAND
    "${CMAKE_COMMAND}"
      -E copy "scripts/standalone.gclient" ".gclient" &&
    "${CMAKE_COMMAND}"
      -V
      -P
      "${CMAKE_CURRENT_LIST_DIR}/GclientSync.cmake"
      "-DGCLIENT_SYNC_TARGET_SOURCE_DIR=${FETCHCONTENT_BASE_DIR}/dawn-src")

FetchContent_GetProperties(dawn)
if (NOT dawn_POPULATED)
  FetchContent_Populate(dawn)
  # A more minimalistic choice of backand than Dawn's default
  set(DAWN_ENABLE_NULL OFF)
  set(DAWN_ENABLE_DESKTOP_GL OFF)
  set(DAWN_ENABLE_OPENGLES OFF)
  set(TINT_BUILD_SPV_READER OFF)

  # Disable unneeded parts
  set(DAWN_BUILD_SAMPLES OFF)
  set(DAWN_FETCH_DEPENDENCIES OFF)
  set(TINT_BUILD_CMD_TOOLS OFF)
  set(TINT_BUILD_SAMPLES OFF)
  set(TINT_BUILD_DOCS OFF)
  set(TINT_BUILD_TESTS OFF)

  add_subdirectory(${dawn_SOURCE_DIR} ${dawn_BINARY_DIR})
endif ()

FetchContent_MakeAvailable(dawn)

add_library(dawn SHARED "${dawn_BINARY_DIR}/gen/src/dawn/native/webgpu_dawn_native_proc.cpp")
# 'nested-namespace-definition' requires compiler flag '/std:c++17'
target_compile_features(dawn PRIVATE cxx_std_17)
target_link_libraries(dawn
  PRIVATE dawn_native
  PUBLIC dawn_headers)
target_compile_definitions(dawn
  PUBLIC "WGPU_SHARED_LIBRARY")
target_compile_definitions(dawn
  PRIVATE "WGPU_IMPLEMENTATION")

include(CTest)
set(BUILD_TESTING ON)
if (BUILD_TESTING)
  add_executable(simple_test tests/test.cpp)
  target_link_libraries(simple_test PRIVATE dawn_headers) # for webgpu/webgpu.h
  add_test(simple-test $<TARGET_FILE:simple_test>)
endif ()
