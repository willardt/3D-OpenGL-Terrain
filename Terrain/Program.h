#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <string>

struct Program {
	unsigned short type;
	std::string file_path;
};

// last address in program must end with type = GL_NONE
GLuint load_shaders(const Program* program);

#endif