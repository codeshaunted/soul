
# this sucks and i hate it but this file is included by tests/CMakeLists.txt
# so cmake_current_source_dir doesn't work >:(
set(SOUL_SOURCE_DIR
  "${CMAKE_SOURCE_DIR}/source/soul")

set(SOUL_MAIN
"${SOUL_SOURCE_DIR}/main.cc")

set(SOUL_SOURCE_FILES
"${SOUL_SOURCE_DIR}/window.cc"
"${SOUL_SOURCE_DIR}/renderer.cc"
"${SOUL_SOURCE_DIR}/text_engine.cc")

set(SOUL_INCLUDE_DIRECTORIES
"${CMAKE_SOURCE_DIR}/include/soul"
"${PROJECT_BINARY_DIR}/source/soul"
"${CMAKE_SOURCE_DIR}/third_party/expected/include"
"${CMAKE_SOURCE_DIR}/third_party/glfw/include"
"${CMAKE_SOURCE_DIR}/third_party/bgfx.cmake/bgfx/include"
"${CMAKE_SOURCE_DIR}/third_party/bgfx.cmake/bx/include")