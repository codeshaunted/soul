// codeshaunted - soul
// shaders/fragment.sh
// fragment shader source file
// Copyright 2022 the soul team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org / licenses / LICENSE - 2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_CharTexture, 0);

uniform vec4 u_color;

void main() {
	gl_FragColor = vec4(v_color0.r, v_color0.g, v_color0.b, v_color0.a) * texture2D(s_CharTexture, v_texcoord0).r;
}