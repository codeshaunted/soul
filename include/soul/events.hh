// codeshaunted - soul
// include/soul/window.hh
// window class header file
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
// See the License for the specific language governing permissionsand
// limitations under the License.

#ifndef SOUL_EVENTS_HH
#define SOUL_EVENTS_HH

#include "GLFW/glfw3.h"

#include <cstdint>
#include <variant>

namespace soul {

enum class KeyAction {
  PRESS,
  RELEASE,
  REPEAT,
  UNKNOWN,
};

KeyAction keyActionFromGLFW(int action);

struct KeyboardModifiers {
  bool mod_shift;
  bool mod_control;
  bool mod_alt;
  bool mod_super;
  bool mod_caps_lock;
  bool mod_num_lock;

  static KeyboardModifiers fromGLFW(int glfwMods);
};

struct KeyEvent {
public:

  int key;
  int scancode;
  KeyAction action;
  KeyboardModifiers modifiers;

  const char* getName() {
    return glfwGetKeyName(this->key, this->scancode);
  }
};

struct TextEvent {
public:
  // TODO: support unicode at all lmao
  uint8_t character;
};

struct MouseCursorPosEvent {
public:
  double x, y;
};

struct MouseCursorEnterExitEvent {
public:
  bool entered;
};

enum class MouseButton {
  LEFT = 0,
  RIGHT = 1,
  MIDDLE = 2,
  BUTTON_4 = 3,
  BUTTON_5 = 4,
  BUTTON_6 = 5,
  BUTTON_7 = 6,
  BUTTON_8 = 7,
  UNKNOWN_BUTTON,
};

MouseButton mouseButtonFromGLFW(int button);

struct MouseButtonEvent {
public:
  MouseButton button;
  // only press or release
  KeyAction action;
  KeyboardModifiers modifiers;
};

struct ScrollEvent {
public:
  double x, y;
};

struct WindowResizeEvent {
public:
  unsigned int width, height;
};

// there is just no good way to format this is there...
typedef std::variant
    <
      KeyEvent,
      TextEvent,
      MouseCursorPosEvent,
      MouseCursorEnterExitEvent,
      MouseButtonEvent,
      ScrollEvent,
      WindowResizeEvent
    >
  Event;

}

#endif // SOUL_EVENTS_HH