cmake_minimum_required(VERSION 3.0...3.25)
project(DawnLoader)

include(FetchContent)
include(GenerateExportHeader)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

FetchContent_Declare(dawn
    GIT_REPOSITORY https://dawn.googlesource.com/dawn
    GIT_TAG chromium/6524
    GIT_SUBMODULES ""
    GIT_SHALLOW TRUE
    EXCLUDE_FROM_ALL)

set(DAWN_FETCH_DEPENDENCIES ON)
set(DAWN_BUILD_SAMPLES OFF)
set(DAWN_ENABLE_DESKTOP_GL OFF)
set(DAWN_ENABLE_OPENGLES OFF)
set(DAWN_USE_GLFW OFF)

set(TINT_BUILD_CMD_TOOLS OFF)
set(TINT_BUILD_TESTS OFF)

FetchContent_MakeAvailable(dawn)

add_library(dawn SHARED "${dawn_BINARY_DIR}/gen/src/dawn/native/webgpu_dawn_native_proc.cpp")
target_link_libraries(dawn
  PRIVATE dawn_native
  PUBLIC dawn_headers)
target_compile_definitions(dawn
  INTERFACE "WGPU_SHARED_LIBRARY")
target_compile_definitions(dawn
  PRIVATE "WGPU_IMPLEMENTATION")
