# codeshaunted - soul
# source/soul/CMakeLists.txt
# soul source file directory CMake file
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

include("${CMAKE_CURRENT_SOURCE_DIR}/sources.cmake")

set(SOUL_LINK_LIBRARIES
	glfw
	bgfx
	freetype
	tinyfiledialogs
	)

set(SOUL_COMPILE_DEFINITIONS)

configure_file("${CMAKE_SOURCE_DIR}/include/soul/config.hh.in" "config.hh")

# shader packing
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/shaders.cc")
add_custom_command(
    OUTPUT shaders.cc
    COMMAND file_pack ${CMAKE_SOURCE_DIR}/source/soul/shaders.cc.in ${CMAKE_CURRENT_BINARY_DIR}/shaders.cc ${CMAKE_BINARY_DIR}/shaders
    DEPENDS shaders.cc.in ${SOUL_SHADER_BINARIES})

# font packing
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/fonts.cc")
add_custom_command(
    OUTPUT fonts.cc
    COMMAND file_pack ${CMAKE_SOURCE_DIR}/source/soul/fonts.cc.in ${CMAKE_CURRENT_BINARY_DIR}/fonts.cc ${CMAKE_BINARY_DIR}/_deps/cascadia-code-src/ttf
    DEPENDS fonts.cc.in)

add_executable(soul ${SOUL_MAIN} ${SOUL_SOURCE_FILES})

target_include_directories(soul PUBLIC ${SOUL_INCLUDE_DIRECTORIES})

target_link_libraries(soul PRIVATE ${SOUL_LINK_LIBRARIES})
#for libtinyfiledialogs
target_link_directories(soul PRIVATE "${CMAKE_SOURCE_DIR}/third_party")

target_compile_definitions(soul PUBLIC ${SOUL_COMPILE_DEFINITIONS})

# warnings
if(MSVC)
	# TODO: maybe turn /WX back on?
	target_compile_options(soul PRIVATE /W4)# /WX)
else()
	target_compile_options(soul PRIVATE 
		-Wall -Wextra -Wpedantic -Wno-unused-parameter)
endif()
