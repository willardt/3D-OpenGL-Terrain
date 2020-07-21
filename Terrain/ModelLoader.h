#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "Mesh.h"

#include <string>
#include <vector>
#include <memory>

bool load_assimp(const std::string_view directory, const std::string_view path, std::vector<Mesh>& meshes);

bool load_model_file(const char* file_path, GLuint& program, std::vector<Mesh>& meshes);

#endif