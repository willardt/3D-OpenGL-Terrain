#include "Terrain.h"

#include <SOIL/SOIL2.h>

#include <iostream>

Terrain::Terrain(int width, int length, float tile_width, float tile_length) :
	_width					( width ),
	_length					( length ),
	_tile_width				( tile_width ),
	_tile_length			( tile_length )
{

	generate_vertex_data();
	generate_uv_data();
	generate_position_data();

	_height_map.resize(width * length);

	load_textures();

	create_vao();
}

Terrain::~Terrain() {
	glDeleteTextures(1, &_height_texture);
}

void Terrain::generate_vertex_data() {
	_vertex_data.reserve(6 * 5);

	_vertex_data.push_back(glm::vec2(0.0f, 0.0f)); // height
	_vertex_data.push_back(glm::vec2(_tile_width, 0.0f)); // height
	_vertex_data.push_back(glm::vec2(0.0f, 0.0f));
	_vertex_data.push_back(glm::vec2(0.0f, 0.0f));
	_vertex_data.push_back(glm::vec2(_tile_width, 0.0f)); // height
	_vertex_data.push_back(glm::vec2(_tile_width, 0.0f));

	_vertex_data.push_back(glm::vec2(_tile_width, 0.0f)); // height
	_vertex_data.push_back(glm::vec2(_tile_width, _tile_length)); // height
	_vertex_data.push_back(glm::vec2(_tile_width, 0.0f));
	_vertex_data.push_back(glm::vec2(_tile_width, 0.0f));
	_vertex_data.push_back(glm::vec2(_tile_width, _tile_length)); // height
	_vertex_data.push_back(glm::vec2(_tile_width, _tile_length));

	_vertex_data.push_back(glm::vec2(_tile_width, _tile_length)); // height
	_vertex_data.push_back(glm::vec2(0.0f, _tile_length)); // height
	_vertex_data.push_back(glm::vec2(_tile_width, _tile_length));
	_vertex_data.push_back(glm::vec2(_tile_width, _tile_length));
	_vertex_data.push_back(glm::vec2(0.0f, _tile_length)); // height
	_vertex_data.push_back(glm::vec2(0.0f, _tile_length));

	_vertex_data.push_back(glm::vec2(0.0f, _tile_length)); // height
	_vertex_data.push_back(glm::vec2(0.0f, 0.0f)); // height
	_vertex_data.push_back(glm::vec2(0.0f, _tile_length));
	_vertex_data.push_back(glm::vec2(0.0f, _tile_length));
	_vertex_data.push_back(glm::vec2(0.0f, 0.0f)); // height
	_vertex_data.push_back(glm::vec2(0.0f, 0.0f));

	_vertex_data.push_back(glm::vec2(_tile_width, 0.0f)); // height
	_vertex_data.push_back(glm::vec2(0.0f, 0.0f));  // height
	_vertex_data.push_back(glm::vec2(0.0f, _tile_length)); // height
	_vertex_data.push_back(glm::vec2(_tile_width, 0.0f)); // height
	_vertex_data.push_back(glm::vec2(0.0f, _tile_length)); // height
	_vertex_data.push_back(glm::vec2(_tile_width, _tile_length)); // height
}

void Terrain::generate_uv_data() {
	_uv_data.reserve(6 * 5);
	
	_uv_data.push_back(glm::vec2(0, 0));
	_uv_data.push_back(glm::vec2(0.5, 0));
	_uv_data.push_back(glm::vec2(0, 1));
	_uv_data.push_back(glm::vec2(0, 1));
	_uv_data.push_back(glm::vec2(0.5, 0));
	_uv_data.push_back(glm::vec2(0.5, 1));

	_uv_data.push_back(glm::vec2(0, 0));
	_uv_data.push_back(glm::vec2(0.5, 0));
	_uv_data.push_back(glm::vec2(0, 1));
	_uv_data.push_back(glm::vec2(0, 1));
	_uv_data.push_back(glm::vec2(0.5, 0));
	_uv_data.push_back(glm::vec2(0.5, 1));

	_uv_data.push_back(glm::vec2(0, 0));
	_uv_data.push_back(glm::vec2(0.5, 0));
	_uv_data.push_back(glm::vec2(0, 1));
	_uv_data.push_back(glm::vec2(0, 1));
	_uv_data.push_back(glm::vec2(0.5, 0));
	_uv_data.push_back(glm::vec2(0.5, 1));

	_uv_data.push_back(glm::vec2(0, 0));
	_uv_data.push_back(glm::vec2(0.5, 0));
	_uv_data.push_back(glm::vec2(0, 1));
	_uv_data.push_back(glm::vec2(0, 1));
	_uv_data.push_back(glm::vec2(0.5, 0));
	_uv_data.push_back(glm::vec2(0.5, 1));

	_uv_data.push_back(glm::vec2(1, 1));
	_uv_data.push_back(glm::vec2(0.5, 1));
	_uv_data.push_back(glm::vec2(0.5, 0.5));
	_uv_data.push_back(glm::vec2(1, 1));
	_uv_data.push_back(glm::vec2(0.5, 0.5));
	_uv_data.push_back(glm::vec2(1, 0.5));
}

void Terrain::generate_position_data() {
	for (int i = 0; i < _length; ++i) {
		for (int j = 0; j < _width; ++j) {
			_position_data.push_back(glm::vec2(j * _tile_width, i * _tile_length));
		}
	}
}

void Terrain::load_textures() {
	_tile_texture.id = SOIL_load_OGL_texture("Data\\tile2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	if (_tile_texture.id == 0) {
		std::cout << SOIL_last_result() << '\n';
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Terrain::adjust_tile_height(int x, int z, float height) {
	if (z < 0 || z >= _length ||
		x < 0 || x >= _width) {
		return;
	}

	const int index = z * _width + x;
	adjust_vertex_height(index, 0, height);
	adjust_vertex_height(index, 1, height);
	adjust_vertex_height(index, 2, height);
	adjust_vertex_height(index, 3, height);
	adjust_vertex_height(index, 4, height);
	adjust_vertex_height(index, 5, height);
}

void Terrain::adjust_ramp_height(int x, int z) {
	if (z < 0 || z >= _length ||
		x < 0 || x >= _width) {
		return;
	}

	const int back_index = (z - 1) * _width + x;
	const int front_index = (z + 1) * _width + x;
	const int left_index = z * _width + x - 1;
	const int right_index = z * _width + x + 1;
	const int index = z * _width + x;
	
	if (get_tile_is_flat(back_index)) {
		adjust_vertex_height(index, 1, get_vertex_height(back_index, 2));
		adjust_vertex_height(index, 0, get_vertex_height(back_index, 3));
		adjust_vertex_height(index, 3, get_vertex_height(back_index, 5));
	}

	if (get_tile_is_flat(front_index)) {
		adjust_vertex_height(index, 2, get_vertex_height(front_index, 1));
		adjust_vertex_height(index, 4, get_vertex_height(front_index, 1));
		adjust_vertex_height(index, 5, get_vertex_height(front_index, 0));
	}

	if (get_tile_is_flat(right_index)) {
		adjust_vertex_height(index, 0, get_vertex_height(right_index, 1));
		adjust_vertex_height(index, 3, get_vertex_height(right_index, 1));
		adjust_vertex_height(index, 5, get_vertex_height(right_index, 2));
	}

	if (get_tile_is_flat(left_index)) {
		adjust_vertex_height(index, 1, get_vertex_height(left_index, 0));
		adjust_vertex_height(index, 2, get_vertex_height(left_index, 5));
		adjust_vertex_height(index, 4, get_vertex_height(left_index, 5));
	}
}

/*
void Terrain::adjust_height(int x, int z, float height) {
	if(z < 0 || z >= _length ||
	   x < 0 || x >= _width) {
		return;
	}

	adjust_vertex_height((z * _width) + x, 0, height);
	if (x > 0) {
		adjust_vertex_height((z * _width) + x - 1, 1, height);
		adjust_vertex_height((z * _width) + x - 1, 4, height);
	}
	adjust_vertex_height((z - 1) * _width + x, 2, height);
	adjust_vertex_height((z - 1) * _width + x, 3, height);
	if (x > 0) {
		adjust_vertex_height((z - 1) * _width + x - 1, 5, height);
	}
}
*/



void Terrain::adjust_vertex_height(int index, int vertex, float height) {
	if(index < 0 || index >= _height_map.size() ||
	   vertex < 0 || vertex > 6) {
		return;
	}

	_height_map[index].height[vertex] = height;

	glBindBuffer(GL_ARRAY_BUFFER, _height_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * index + sizeof(GLfloat) * vertex, sizeof(GLfloat), &_height_map[index].height[vertex]);
}

void Terrain::create_vao() {
	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glCreateBuffers(1, &_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glNamedBufferStorage(_vertex_buffer, sizeof(glm::vec2) * _vertex_data.size(), &_vertex_data[0], 0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glCreateBuffers(1, &_uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uv_buffer);
	glNamedBufferStorage(_uv_buffer, sizeof(glm::vec2) * _uv_data.size(), &_uv_data[0], 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glCreateBuffers(1, &_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _position_buffer);
	glNamedBufferStorage(_position_buffer, sizeof(glm::vec2) * _position_data.size(), &_position_data[0], 0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(2, 1);

	glCreateBuffers(1, &_height_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _height_buffer);
	glNamedBufferStorage(_height_buffer, sizeof(GLfloat) * 6 * _height_map.size(), &_height_map[0], GL_DYNAMIC_STORAGE_BIT);

	glCreateTextures(GL_TEXTURE_BUFFER, 1, &_height_texture);

	_program = 1;

	glUniform1i(glGetUniformLocation(_program, "height"), 0);
	glUniform1i(glGetUniformLocation(_program, "tile_texture"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, _height_texture);
	glTextureBuffer(_height_texture, GL_R32F, _height_buffer);
	glBindTextureUnit(GL_TEXTURE0, _height_buffer);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _tile_texture.id);
}

void Terrain::draw(int mode) {
	glBindVertexArray(_vao);
	glUseProgram(_program);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawArraysInstanced(mode, 0, 6 * 5, _position_data.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glDisable(GL_CULL_FACE);
}

float Terrain::get_tile_width() {
	return _tile_width;
}

float Terrain::get_tile_length() {
	return _tile_length;
}

std::vector<GLfloat> Terrain::get_tile_height(int x, int z) {
	std::vector<GLfloat> heights;
	heights.resize(6);

	if(z < 0 || z >= _length ||
		x < 0 || x >= _width) {
		return heights;
	}

	const int index = z * _width + x;
	heights[0] = _height_map[index].height[0];
	heights[1] = _height_map[index].height[1];
	heights[2] = _height_map[index].height[2];
	heights[3] = _height_map[index].height[3];
	heights[4] = _height_map[index].height[4];
	heights[5] = _height_map[index].height[5];

	return heights;
}

float Terrain::get_vertex_height(int index, int vertex) {
	if(index < 0 || index > _height_map.size() ||
	   vertex < 0 || vertex > 5) {
		return 0.0f;
	}

	return _height_map[index].height[vertex];
}

bool Terrain::get_tile_is_flat(int index) {
	if(index < 0 || index > _height_map.size()) {
		return false;
	}

	return _height_map[index].height[0] &&
		   _height_map[index].height[1] &&
		   _height_map[index].height[2] &&
		   _height_map[index].height[3] &&
		   _height_map[index].height[4] &&
		   _height_map[index].height[5];
}