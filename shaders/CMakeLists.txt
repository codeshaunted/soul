# codeshaunted - soul
# shaders/CMakeLists.txt
# shaders directory CMake file
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

set(SOUL_COMPILE_SHADER_BASE shaderc --varyingdef ${CMAKE_SOURCE_DIR}/shaders/varying.def.sc -i ${CMAKE_SOURCE_DIR}/third_party/bgfx.cmake/bgfx/src)
set(SOUL_COMPILE_VERTEX_SHADER_BASE ${SOUL_COMPILE_SHADER_BASE} --type vertex -f ${CMAKE_SOURCE_DIR}/shaders/vertex.sh)
set(SOUL_COMPILE_FRAGMENT_SHADER_BASE ${SOUL_COMPILE_SHADER_BASE} --type fragment -f ${CMAKE_SOURCE_DIR}/shaders/fragment.sh)

set(SOUL_COMPILE_TEXT_SHADER_BASE shaderc --varyingdef ${CMAKE_SOURCE_DIR}/shaders/varying_text.def.sc -i ${CMAKE_SOURCE_DIR}/third_party/bgfx.cmake/bgfx/src)
set(SOUL_COMPILE_VERTEX_TEXT_SHADER_BASE ${SOUL_COMPILE_TEXT_SHADER_BASE} --type vertex -f ${CMAKE_SOURCE_DIR}/shaders/vertex_text.sh)
set(SOUL_COMPILE_FRAGMENT_TEXT_SHADER_BASE ${SOUL_COMPILE_TEXT_SHADER_BASE} --type fragment -f ${CMAKE_SOURCE_DIR}/shaders/fragment_text.sh)

set(SOUL_SHADER_BINARIES)

if(SOUL_IS_WINDOWS)
    set(SOUL_DX_SHADER_BINARIES
        "${CMAKE_SOURCE_DIR}/build/vertex_dx9.bin"
        "${CMAKE_SOURCE_DIR}/build/vertex_dx11.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_dx9.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_dx11.bin"
        "${CMAKE_SOURCE_DIR}/build/vertex_text_dx9.bin"
        "${CMAKE_SOURCE_DIR}/build/vertex_text_dx11.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_text_dx9.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_text_dx11.bin")

    list(APPEND SOUL_SHADER_BINARIES ${SOUL_DX_SHADER_BINARIES})

    add_custom_command(
        OUTPUT ${SOUL_DX_SHADER_BINARIES}
        COMMAND ${SOUL_COMPILE_VERTEX_SHADER_BASE} -o vertex_dx9.bin --platform windows -p vs_3_0 -O 3
        COMMAND ${SOUL_COMPILE_VERTEX_SHADER_BASE} -o vertex_dx11.bin --platform windows -p vs_5_0 -O 3
        COMMAND ${SOUL_COMPILE_FRAGMENT_SHADER_BASE} -o fragment_dx9.bin --platform windows -p ps_3_0 -O 3
        COMMAND ${SOUL_COMPILE_FRAGMENT_SHADER_BASE} -o fragment_dx11.bin --platform windows -p ps_5_0 -O 3
        COMMAND ${SOUL_COMPILE_VERTEX_TEXT_SHADER_BASE} -o vertex_text_dx9.bin --platform windows -p vs_3_0 -O 3
        COMMAND ${SOUL_COMPILE_VERTEX_TEXT_SHADER_BASE} -o vertex_text_dx11.bin --platform windows -p vs_5_0 -O 3
        COMMAND ${SOUL_COMPILE_FRAGMENT_TEXT_SHADER_BASE} -o fragment_text_dx9.bin --platform windows -p ps_3_0 -O 3
        COMMAND ${SOUL_COMPILE_FRAGMENT_TEXT_SHADER_BASE} -o fragment_text_dx11.bin --platform windows -p ps_5_0 -O 3
        DEPENDS shaderc)
endif(SOUL_IS_WINDOWS)

if(SOUL_IS_WINDOWS OR SOUL_IS_LINUX)
    set(SOUL_KHRONOS_SHADER_BINARIES 
        "${CMAKE_SOURCE_DIR}/build/vertex_opengl.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_opengl.bin"
        "${CMAKE_SOURCE_DIR}/build/vertex_spirv.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_spirv.bin"
        "${CMAKE_SOURCE_DIR}/build/vertex_text_opengl.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_text_opengl.bin"
        "${CMAKE_SOURCE_DIR}/build/vertex_text_spirv.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_text_spirv.bin")

    list(APPEND SOUL_SHADER_BINARIES ${SOUL_KHRONOS_SHADER_BINARIES})

    add_custom_command(
        OUTPUT ${SOUL_KHRONOS_SHADER_BINARIES}
        COMMAND ${SOUL_COMPILE_VERTEX_SHADER_BASE} -o vertex_opengl.bin --platform linux -p 120
        COMMAND ${SOUL_COMPILE_FRAGMENT_SHADER_BASE} -o fragment_opengl.bin --platform linux -p 120
        COMMAND ${SOUL_COMPILE_VERTEX_SHADER_BASE} -o vertex_spirv.bin --platform linux -p spirv
        COMMAND ${SOUL_COMPILE_FRAGMENT_SHADER_BASE} -o fragment_spirv.bin --platform linux -p spirv
        COMMAND ${SOUL_COMPILE_VERTEX_TEXT_SHADER_BASE} -o vertex_text_opengl.bin --platform linux -p 120
        COMMAND ${SOUL_COMPILE_FRAGMENT_TEXT_SHADER_BASE} -o fragment_text_opengl.bin --platform linux -p 120
        COMMAND ${SOUL_COMPILE_VERTEX_TEXT_SHADER_BASE} -o vertex_text_spirv.bin --platform linux -p spirv
        COMMAND ${SOUL_COMPILE_FRAGMENT_TEXT_SHADER_BASE} -o fragment_text_spirv.bin --platform linux -p spirv
        DEPENDS shaderc)
endif(SOUL_IS_WINDOWS OR SOUL_IS_LINUX)

if(SOUL_IS_MACOS)
    set(SOUL_METAL_SHADER_BINARIES 
        "${CMAKE_SOURCE_DIR}/build/vertex_metal.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_metal.bin"
        "${CMAKE_SOURCE_DIR}/build/vertex_text_metal.bin"
        "${CMAKE_SOURCE_DIR}/build/fragment_metal.bin")

    list(APPEND SOUL_SHADER_BINARIES ${SOUL_METAL_SHADER_BINARIES})

    add_custom_command(
        OUTPUT ${SOUL_METAL_SHADER_BINARIES}
        COMMAND ${SOUL_COMPILE_VERTEX_SHADER_BASE} -o vertex_metal.bin --platform osx -p metal
        COMMAND ${SOUL_COMPILE_FRAGMENT_SHADER_BASE} -o fragment_metal.bin --platform osx -p metal
        COMMAND ${SOUL_COMPILE_VERTEX_TEXT_SHADER_BASE} -o vertex_text_metal.bin --platform osx -p metal
        COMMAND ${SOUL_COMPILE_FRAGMENT_TEXT_SHADER_BASE} -o fragment_text_metal.bin --platform osx -p metal
        DEPENDS shaderc)
endif(SOUL_IS_MACOS)

add_custom_target(compile_shaders ALL DEPENDS ${SOUL_SHADER_BINARIES})