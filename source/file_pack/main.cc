// codeshaunted - soul
// source/file_pack/main.cc
// file_pack entry point file
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

#include <cstdlib>
#include <fstream>
#include <sstream>

#define FILE_PACK_START_SEQUENCE "${"
#define FILE_PACK_END_CHARACTER '}'

int main(int argc, char** argv) {
	if (argc != 4) return EXIT_FAILURE;

	std::string input_file_path = argv[1];
	std::string output_file_path = argv[2];
	std::string files_directory_path = argv[3];

	std::ifstream input_file(input_file_path);
	std::stringstream buffer;
	buffer << input_file.rdbuf();
	input_file.close();
	std::string file_data = buffer.str();

	while (file_data.find(FILE_PACK_START_SEQUENCE) != std::string::npos) {
		size_t replace_start_position = file_data.find(FILE_PACK_START_SEQUENCE);
		size_t replace_end_position = replace_start_position + 2;
		
		for (; file_data.c_str()[replace_end_position] != FILE_PACK_END_CHARACTER; replace_end_position++);

		std::string file_path = file_data.substr(replace_start_position + 2, replace_end_position - replace_start_position - 2);
		std::ifstream file(files_directory_path + "/" + file_path, std::ios::in | std::ios::binary);
		std::string file_array = "";

		if (file.good()) {
			file.seekg(0, std::ios::end);
			size_t file_size = file.tellg();
			file.seekg(0, std::ios::beg);


			for (size_t i = 0; i < file_size; i++) {
				unsigned char byte = file.get();

				char hex_byte[5];
				sprintf(hex_byte, "0x%02x", byte);

				file_array += hex_byte;
				if (i != file_size - 1) file_array += ", ";
			}
		}

		file.close();

		file_data.replace(replace_start_position, replace_end_position - replace_start_position + 1, file_array);
	}
	
	std::ofstream output_file(output_file_path);
	output_file << file_data;
	output_file.close();

	return EXIT_SUCCESS;
}