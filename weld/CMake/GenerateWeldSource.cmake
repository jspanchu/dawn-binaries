include(WeldSymbols)

set(WELD_DECLARE_SYMBOLS)
set(WELD_LOAD_WGPU_SYMBOLS)
set(WELD_INIT_SYMBOLS)
foreach (weld_symbol IN LISTS WELD_SYMBOL_NAMES)
  string(APPEND WELD_DECLARE_SYMBOLS "WELD_DECLARE_SYMBOL(${weld_symbol});\n")
  string(APPEND WELD_LOAD_WGPU_SYMBOLS "  WELD_LOAD_WGPU_SYMBOL(${weld_symbol});\n")
  string(APPEND WELD_INIT_SYMBOLS "WELD_INIT_SYMBOL(${weld_symbol});\n")
endforeach ()

configure_file("${CMAKE_CURRENT_LIST_DIR}/weld.h.in" "${CMAKE_CURRENT_BINARY_DIR}/weld.h" @ONLY)
configure_file("${CMAKE_CURRENT_LIST_DIR}/weld.cxx.in" "${CMAKE_CURRENT_BINARY_DIR}/weld.cxx" @ONLY)
