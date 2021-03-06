# codeshaunted - soul
# source/soul/sources.cmake
# cursed CMake sources file
# Copyright 2022 the soul team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http:#www.apache.org / licenses / LICENSE - 2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# this sucks and i hate it but this file is included by tests/CMakeLists.txt
# so cmake_current_source_dir doesn't work >:(
set(SOUL_SOURCE_DIR
    "${CMAKE_SOURCE_DIR}/source/soul")

set(SOUL_MAIN
    "${SOUL_SOURCE_DIR}/main.cc")

set(SOUL_SOURCE_FILES
    "${SOUL_SOURCE_DIR}/window.cc"
    "${CMAKE_BINARY_DIR}/source/soul/shaders.cc"
    "${CMAKE_BINARY_DIR}/source/soul/fonts.cc"
    "${SOUL_SOURCE_DIR}/renderer.cc"
    "${SOUL_SOURCE_DIR}/text_engine.cc"
    "${SOUL_SOURCE_DIR}/events.cc"
    "${SOUL_SOURCE_DIR}/gui.cc"
    "${SOUL_SOURCE_DIR}/debug.cc"
    "${SOUL_SOURCE_DIR}/draw_cmd.cc"
    "${SOUL_SOURCE_DIR}/font_manager.cc"
    "${SOUL_SOURCE_DIR}/utils.cc"
    "${SOUL_SOURCE_DIR}/editor.cc")

set(SOUL_INCLUDE_DIRECTORIES
    "${CMAKE_SOURCE_DIR}/include/soul"
    "${PROJECT_BINARY_DIR}/source/soul"
    "${CMAKE_SOURCE_DIR}/third_party/expected/include"
    "${CMAKE_SOURCE_DIR}/third_party/glfw/include"
    "${CMAKE_SOURCE_DIR}/third_party/bgfx.cmake/bgfx/include"
    "${CMAKE_SOURCE_DIR}/third_party/bgfx.cmake/bx/include"
    "${CMAKE_SOURCE_DIR}/third_party/glm"
    "${CMAKE_SOURCE_DIR}/third_party/freetype/include"
    "${CMAKE_SOURCE_DIR}/third_party/libtinyfiledialogs")
