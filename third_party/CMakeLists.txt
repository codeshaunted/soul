# codeshaunted - soul
# third_party/CMakeLists.txt
# third party directory CMake file
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

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
add_subdirectory("glfw")

# bgfx.cmake
set(BGFX_BUILD_TOOLS ON CACHE BOOL "" FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("bgfx.cmake")

# freetype
add_subdirectory("freetype")

# cascadia-code
include(FetchContent)
FetchContent_Declare(
    cascadia-code
    URL https://github.com/microsoft/cascadia-code/releases/download/v2111.01/CascadiaCode-2111.01.zip
)
FetchContent_MakeAvailable(cascadia-code)

# libtinyfiledialogs
add_library(tinyfiledialogs STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/libtinyfiledialogs/tinyfiledialogs.c"
    )
