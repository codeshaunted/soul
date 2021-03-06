// codeshaunted - soul
// source/soul/text_engine.cc
// utils source file
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

#include "utils.hh"
#include <filesystem>

#include <fstream>

namespace soul::utils {

tl::expected<std::string, Error> readFilePath(std::filesystem::path path) {
  auto size = std::filesystem::file_size(path);
  std::string ret(size, ' ');
  
  std::ifstream stream(path);

  stream.read(&ret[0], size);

  return ret;
}

}