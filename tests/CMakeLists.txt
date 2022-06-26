# codeshaunted - soul
# tests/CMakeLists.txt
# tests directory CMake file
# Copyright 2022 the soul team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include("${CMAKE_SOURCE_DIR}/source/soul/sources.cmake")


set(SOUL_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/test_main.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_text_engine.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_window.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_gui.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_ft.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_font_manager.cc")

# HACK: this file should already be marked as generated but cmake is dumb and stupid.
set_source_files_properties("${CMAKE_BINARY_DIR}/source/soul/shaders.cc" PROPERTIES GENERATED TRUE)
set_source_files_properties("${CMAKE_BINARY_DIR}/source/soul/fonts.cc" PROPERTIES GENERATED TRUE)

add_executable(soul_tests ${SOUL_TEST_SOURCES} ${SOUL_SOURCE_FILES})
# need to link the same libs as the main executable
target_link_libraries(soul_tests glfw bgfx freetype)

target_include_directories(soul_tests PUBLIC 
  ${SOUL_INCLUDE_DIRECTORIES}
  "${CMAKE_SOURCE_DIR}/third_party/doctest/doctest")