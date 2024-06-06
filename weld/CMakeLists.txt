cmake_minimum_required(VERSION 3.12...3.25)
project(weld VERSION 0.0.0 LANGUAGES CXX)

option(WELD_INSTALL "Install weld" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include (GenerateWeldSource)

add_library(weld "${CMAKE_CURRENT_BINARY_DIR}/weld.cxx")
add_library(weld::weld ALIAS weld)
target_compile_features(weld PRIVATE cxx_std_11)
target_include_directories(weld PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
)
if (NOT WIN32)
  target_link_libraries(weld PUBLIC ${CMAKE_DL_LIBS})
endif()

if (WELD_INSTALL)
  include(GNUInstallDirs)
  set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/weld)

  # Install files
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/weld.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  # Install library target and add it and any dependencies to export set.
  install(TARGETS weld
    EXPORT weld-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  # Actually write exported config w/ imported targets
  install(EXPORT weld-targets
    FILE weldTargets.cmake
    NAMESPACE weld::
    DESTINATION ${INSTALL_CONFIGDIR}
  )

  # Create a ConfigVersion.cmake file:
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/weldConfigVersion.cmake
    COMPATIBILITY AnyNewerVersion
  )

  # Configure config file
  configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/CMake/weldConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/weldConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
  )

  # Install the fully generated config and configVersion files
  install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/weldConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/weldConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
  )
endif ()
