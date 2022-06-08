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
  "${CMAKE_CURRENT_SOURCE_DIR}/test_window.cc")

add_executable(soul_tests ${SOUL_TEST_SOURCES} ${SOUL_SOURCE_FILES})
# need to link the same libs as the main executable
target_link_libraries(soul_tests glfw bgfx)

message(STATUS "including: ${SOUL_INCLUDE_DIRECTORIES}")

target_include_directories(soul_tests PUBLIC 
  ${SOUL_INCLUDE_DIRECTORIES}
  "${CMAKE_SOURCE_DIR}/third_party/doctest/doctest")