

include("${CMAKE_SOURCE_DIR}/source/soul/sources.cmake")

set(SOUL_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/test_main.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_text_engine.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_window.cc")

add_executable(soul_tests ${SOUL_TEST_SOURCES} ${SOUL_SOURCE_FILES})
# need to link the same libs as the main executable
target_link_libraries(soul_tests glfw bgfx)

message(STATUS "including: ${SOUL_INCLUDE_DIRECTORIES}")

target_include_directories(soul_tests PUBLIC 
  ${SOUL_INCLUDE_DIRECTORIES}
  "${CMAKE_SOURCE_DIR}/third_party/doctest/doctest")