// codeshaunted - soul
// source/shader_pack/main.cc
// shader_pack entry point file
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

#include <cstdlib>
#include <fstream>
#include <sstream>

#define SHADER_PACK_START_SEQUENCE "${"
#define SHADER_PACK_END_CHARACTER '}'

int main(int argc, char** argv) {
	if (argc != 4) return EXIT_FAILURE;

	std::string input_file_path = argv[1];
	std::string output_file_path = argv[2];
	std::string shader_directory_path = argv[3];

	std::ifstream input_file(input_file_path);
	std::stringstream buffer;
	buffer << input_file.rdbuf();
	input_file.close();
	std::string file_data = buffer.str();

	while (file_data.find(SHADER_PACK_START_SEQUENCE) != std::string::npos) {
		size_t replace_start_position = file_data.find(SHADER_PACK_START_SEQUENCE);
		size_t replace_end_position = replace_start_position + 2;
		
		for (; file_data.c_str()[replace_end_position] != SHADER_PACK_END_CHARACTER; replace_end_position++);

		std::string shader_file_path = file_data.substr(replace_start_position + 2, replace_end_position - replace_start_position - 2);
		std::ifstream shader_file(shader_directory_path + "/" + shader_file_path, std::ios::in | std::ios::binary);
		std::string shader_file_array = "";

		if (shader_file.good()) {
			shader_file.seekg(0, std::ios::end);
			size_t shader_file_size = shader_file.tellg();
			shader_file.seekg(0, std::ios::beg);


			for (size_t i = 0; i < shader_file_size; i++) {
				unsigned char byte = shader_file.get();

				char hex_byte[5];
				sprintf(hex_byte, "0x%02x", byte);

				shader_file_array += hex_byte;
				if (i != shader_file_size - 1) shader_file_array += ", ";
			}
		}

		shader_file.close();

		file_data.replace(replace_start_position, replace_end_position - replace_start_position + 1, shader_file_array);
	}
	
	std::ofstream output_file(output_file_path);
	output_file << file_data;
	output_file.close();

	return EXIT_SUCCESS;
}