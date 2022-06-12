// codeshaunted - soul
// source/soul/events.cc
// events class source file
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

#include "events.hh"

namespace soul {

KeyAction keyActionFromGLFW(int action) {
  switch (action) {
    case GLFW_PRESS:
      return KeyAction::PRESS;
    case GLFW_RELEASE:
      return KeyAction::RELEASE;
    case GLFW_REPEAT:
      return KeyAction::REPEAT;
  }
  return KeyAction::UNKNOWN;
}

KeyboardModifiers KeyboardModifiers::fromGLFW(int glfwMods) {
  KeyboardModifiers ret;
  ret.mod_shift     = glfwMods | GLFW_MOD_SHIFT;
  ret.mod_control   = glfwMods | GLFW_MOD_CONTROL;
  ret.mod_alt       = glfwMods | GLFW_MOD_ALT;
  ret.mod_super     = glfwMods | GLFW_MOD_SUPER;
  ret.mod_caps_lock = glfwMods | GLFW_MOD_CAPS_LOCK;
  ret.mod_num_lock  = glfwMods | GLFW_MOD_NUM_LOCK;
  return ret;
}

MouseButton mouseButtonFromGLFW(int button) {
  if (button >= 0 && button <= (int)MouseButton::BUTTON_8) {
    return (MouseButton) button;
  }
  return MouseButton::UNKNOWN_BUTTON;
}

}