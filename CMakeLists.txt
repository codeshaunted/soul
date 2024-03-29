# codeshaunted - soul
# CMakeLists.txt
# root CMake file
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

cmake_minimum_required(VERSION 3.12)

project(soul VERSION 1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

# platform identification
set(SOUL_IS_WINDOWS FALSE)
set(SOUL_IS_UNIX FALSE)
set(SOUL_IS_LINUX FALSE)
set(SOUL_IS_MACOS FALSE)

if(WIN32)
    set(SOUL_IS_WINDOWS TRUE)
    add_compile_definitions(SOUL_IS_WINDOWS)
endif(WIN32)

if(UNIX)
    set(SOUL_IS_UNIX TRUE)
    add_compile_definitions(SOUL_IS_UNIX)
endif(UNIX)

if(UNIX AND NOT APPLE)
    set(SOUL_IS_LINUX TRUE)
    add_compile_definitions(SOUL_IS_LINUX)
endif(UNIX AND NOT APPLE)

if(UNIX AND APPLE)
    set(SOUL_IS_MACOS TRUE)
    add_compile_definitions(SOUL_IS_MACOS)
endif(UNIX AND APPLE)

# third party
add_subdirectory("third_party")

# shaders
add_subdirectory("shaders")

# internal
add_subdirectory("source")

option(SOUL_BUILD_TESTS "build soul tests" ON)

if(SOUL_BUILD_TESTS) 
    add_subdirectory("tests")
endif(SOUL_BUILD_TESTS)