cmake_minimum_required(VERSION 3.0...3.25)
project(DawnLoader)

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

set(DAWN_FETCH_DEPENDENCIES OFF)
set(DAWN_BUILD_SAMPLES OFF)
set(DAWN_ENABLE_DESKTOP_OPENGL OFF)
set(DAWN_ENABLE_OPENGLES OFF)
set(DAWN_USE_GLFW OFF)

set(TINT_BUILD_CMD_TOOLS OFF)
set(TINT_BUILD_TESTS OFF)

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
