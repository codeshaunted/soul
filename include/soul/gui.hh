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
// See the License for the specific language governing permissionsand
// limitations under the License.

#ifndef SOUL_GUI_HH
#define SOUL_GUI_HH

#include <cstdint>
#include <memory>
#include <vector>

#include "renderer.hh"

namespace soul {

struct GNode {
  virtual bool shouldBeDrawn();
};

struct GLeaf : GNode {
	uint32_t color;

  virtual bool shouldBeDrawn() override {
    return true;
  }

  GLeaf(uint32_t box_color) {
    this->color = box_color;
  }
};

struct GNonLeaf : GNode {
  /**
   * If true, the two child nodes 
   */
  bool vertical;
  float split_point;
  std::unique_ptr<GNode> first;
  std::unique_ptr<GNode> second;

  virtual bool shouldBeDrawn() override {
    return false;
  }
};

class GUI {
public:
  static GUI create();

  std::pair<std::vector<Vertex>, std::vector<unsigned int>>
  toGeometry(unsigned int window_width, unsigned int window_height);
// private:
  std::unique_ptr<GNode> tree;
private:
  GUI() {
    this->tree = nullptr;
  }
};

}

#endif // SOUL_GUI_HH