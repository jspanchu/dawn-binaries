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

add_library(dawn_interface INTERFACE)
target_link_libraries(dawn_interface INTERFACE dawn_proc dawn_common dawn_headers webgpu_dawn)

add_library(dawn_loader SHARED dawn_loader.cxx "${dawn_BINARY_DIR}/gen/src/dawn/dawn_proc.cpp")
generate_export_header(dawn_loader
  EXPORT_MACRO_NAME DAWN_LOADER_EXPORT
  EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/dawn_loader_export.h")
target_compile_features(dawn_loader PRIVATE cxx_std_17)
target_link_libraries(dawn_loader PRIVATE dawn_interface)
target_compile_definitions(dawn_loader PRIVATE "WGPU_SHARED_LIBRARY" "WGPU_IMPLEMENTATION")
target_include_directories(dawn_loader
  PRIVATE
    # needed because dawn_proc.cpp includes dawn/common/Compiler.h which is directly used as a source file in the dawn_common target
    "${dawn_SOURCE_DIR}/src"
    # For dawn_loader_export.h
    "${CMAKE_CURRENT_BINARY_DIR}")
