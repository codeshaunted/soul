// codeshaunted - soul
// source/soul/error.hh
// error enum header file
// Copyright 2021 codeshaunted
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

#ifndef SOUL_ERROR_HH
#define SOUL_ERROR_HH

namespace soul {

// undef these macros and replace them with our enum
#undef GLFW_NOT_INITIALIZED
#undef GLFW_NO_CURRENT_CONTEXT
#undef GLFW_INVALID_ENUM
#undef GLFW_INVALID_VALUE
#undef GLFW_OUT_OF_MEMORY
#undef GLFW_API_UNAVAILABLE
#undef GLFW_VERSION_UNAVAILABLE
#undef GLFW_PLATFORM_ERROR
#undef GLFW_FORMAT_UNAVAILABLE
enum class Error {
	SUCCESS,
	UNKNOWN,
	IDX_OUT_OF_RANGE,
	GLFW_NOT_INITIALIZED = 0x00010001,
	GLFW_NO_CURRENT_CONTEXT = 0x00010002,
	GLFW_INVALID_ENUM = 0x00010003,
	GLFW_INVALID_VALUE = 0x00010004,
	GLFW_OUT_OF_MEMORY = 0x00010005,
	GLFW_API_UNAVAILABLE = 0x00010006,
	GLFW_VERSION_UNAVAILABLE = 0x00010007,
	GLFW_PLATFORM_ERROR = 0x00010008,
	GLFW_FORMAT_UNAVAILABLE = 0x00010009
};

} // namespace soul

#endif // SOUL_ERROR_HH