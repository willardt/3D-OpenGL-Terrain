#include "Program.h"

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <iostream>

GLuint load_shaders(const Program* program) {
	const GLuint program_id = glCreateProgram();
	std::vector<GLuint> shader_ids;
	const char* data;
	GLint result;
	GLint info_log_length;
	bool loaded = true;

	for (unsigned int i = 0; program[i].type != GL_NONE; ++i) {
		GLuint shader_id = glCreateShader(program[i].type);
		shader_ids.push_back(shader_id);

		std::string sdata;
		std::stringstream ssdata;
		std::fstream shader_file(program[i].file_path, std::ios::in);
		if (shader_file.is_open()) {
			ssdata << shader_file.rdbuf();
			shader_file.seekg(0, std::ios::end);
			sdata.reserve((size_t)shader_file.tellg());
			sdata = ssdata.str();
			data = sdata.c_str();
		}
		else {
			std::cout << "Couldn't open shader file -- " << program[i].file_path << '\n';
			loaded = false;
		}

		if (loaded) {
			glShaderSource(shader_id, 1, &data, NULL);
			glCompileShader(shader_id);

			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
			std::cout << "Shader Result -- " << program[i].file_path << " -- " << ((result == GL_TRUE) ? "Good" : "Bad") << '\n';
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length != 0) {
				GLchar* info_log = new GLchar[info_log_length];
				glGetShaderInfoLog(shader_id, info_log_length, 0, info_log);
				std::cout << "-- Shader Log --" << std::endl << info_log << '\n';
				std::cout << "-- " << program[i].file_path << "-- " << std::endl << data << '\n';
				delete[] info_log;
			}
		}
	}

	for (auto shader_id : shader_ids) {
		glAttachShader(program_id, shader_id);
	}

	glLinkProgram(program_id);

	for (auto shader_id : shader_ids) {
		glDetachShader(program_id, shader_id);
		glDeleteShader(shader_id);
	}

	std::cout << "Program id: " << program_id << std::endl;

	return program_id;
}