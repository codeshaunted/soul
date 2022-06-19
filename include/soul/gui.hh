// codeshaunted - soul
// include/soul/gui.hh
// gui class header file
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

#ifndef SOUL_GUI_HH
#define SOUL_GUI_HH

#include <cstdint>
#include <memory>
#include <vector>

#include "renderer.hh"

namespace soul {

struct Rect {
	float x, y; // top left
	float width, height;
	uint32_t color;
};

struct GNode {
  virtual void toDrawCmds(std::vector<std::unique_ptr<DrawCmd::Any>>& out, Rect bounding_box) {}
  virtual ~GNode() {}
};

struct GLeaf : GNode {
	uint32_t color;

  virtual void toDrawCmds(std::vector<std::unique_ptr<DrawCmd::Any>>& out, Rect bounding_box) override;

  GLeaf(uint32_t box_color) {
    this->color = box_color;
  }

  virtual ~GLeaf() override {}
};

struct GNonLeaf : GNode {
  /**
   * If true, the two child nodes 
   */
  bool horizontal;
  float split_point;
  GNode* first;
  GNode* second;

  virtual void toDrawCmds(std::vector<std::unique_ptr<DrawCmd::Any>>& out, Rect bounding_box) override;

  static std::optional<GNonLeaf*> create(bool split_horizontal, float split, GNode* first, GNode* second);

  virtual ~GNonLeaf() override {
    delete first;
    delete second;
  }
private:
  GNonLeaf(bool split_horizontal, float split, GNode* first, GNode* second);
};

class GUI {
public:
  static GUI create();

  std::vector<std::unique_ptr<DrawCmd::Any>>
  toCmds(unsigned int window_width, unsigned int window_height);
// private:
  GNode* tree;

  ~GUI() {
    delete tree;
  }
private:
  GUI() {
    this->tree = nullptr;
  }
};

}

#endif // SOUL_GUI_HH