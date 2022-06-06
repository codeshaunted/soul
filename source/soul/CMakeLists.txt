# codeshaunted - soul
# source/soul/CMakeLists.txt
# source file directory CMake file
# Copyright 2021 codeshaunted
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
# See the License for the specific language governing permissionsand
# limitations under the License.

set(SOUL_SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/main.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/window.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/renderer.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/textengine.cc")

set(SOUL_INCLUDE_DIRECTORIES
	"${CMAKE_SOURCE_DIR}/include/soul"
	"${PROJECT_BINARY_DIR}/source/soul"
	"${CMAKE_SOURCE_DIR}/third_party/expected/include"
	"${CMAKE_SOURCE_DIR}/third_party/glfw/include")

set(SOUL_LINK_LIBRARIES
	glfw
	bgfx)

set(SOUL_COMPILE_DEFINITIONS)

configure_file("${CMAKE_SOURCE_DIR}/include/soul/config.hh.in" "config.hh")

add_executable(soul ${SOUL_SOURCE_FILES})

target_include_directories(soul PUBLIC ${SOUL_INCLUDE_DIRECTORIES})

target_link_libraries(soul PUBLIC ${SOUL_LINK_LIBRARIES})

target_compile_definitions(soul PUBLIC ${SOUL_COMPILE_DEFINITIONS})
