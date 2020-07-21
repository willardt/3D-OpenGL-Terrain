#ifndef MODEL_H
#define MODEL_H

#include <string_view>

#include "Mesh.h"

class Model {
public:
	Model();
	Model(const GLuint program, const std::string_view directory, const std::string_view model_file);
	Model(const size_t id, std::string_view file_path);
	Model(const Model& rhs);

	void draw(Transform& transform) const;

	std::vector<Mesh> _meshes;

	GLuint get_program();
private:
	size_t _id;
	GLuint _program;
};

#endif