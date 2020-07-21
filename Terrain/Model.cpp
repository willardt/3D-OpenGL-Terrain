#include "Model.h"

#include "ModelLoader.h"

Model::Model() :
	_id(0),
	_program(0)
{}

Model::Model(const GLuint program, const std::string_view directory, const std::string_view model_file) :
	_id(0),
	_program(program)
{
	load_assimp(directory, model_file, _meshes);
}

Model::Model(const size_t id, std::string_view file_path) :
	_id(id)
{
	load_model_file(file_path.data(), _program, _meshes);
}

Model::Model(const Model& rhs) :
	_id(rhs._id),
	_program(rhs._program),
	_meshes(rhs._meshes)
{}

void Model::draw(Transform& transform) const {
	for (auto mesh : _meshes) {
		mesh.draw(_program, transform);
	}
}

GLuint Model::get_program() {
	return _program;
}