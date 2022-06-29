// codeshaunted - soul
// include/soul/utils.hh
// general utilities header file
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

#ifndef SOUL_UTILS_HH
#define SOUL_UTILS_HH

#include <string>
#include "tl/expected.hpp"
#include <filesystem>
#include <string_view>

#include "error.hh"

namespace soul::utils {

tl::expected<std::string, Error> readFilePath(std::filesystem::path path);

}


#endif // SOUL_UTILS_HH