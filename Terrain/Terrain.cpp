#include "Terrain.h"

constexpr GLfloat SEGMENT_VERTEX_DATA[12] =   { 0.0f, 0.0f,
												1.0f, 0.0f,
												0.0f, 1.0f,
												0.0f, 1.0f,
												1.0f, 0.0f,
												1.0f, 1.0f};

Terrain::Terrain(int width, int length, float segment_width, float segment_length) :
	_width(width),
	_length(length),
	_segment_width(segment_width),
	_segment_length(segment_length)
{
	/*
	_segments.resize(_length);
	for (auto& col : _segments) {
		col.resize(_width);
	}
	*/

	generate_vertex_data();
	generate_position_data();

	_height_map.resize(width * length);

	adjust_height(1, 1, 5);
	adjust_height(0, 1, 5);
	adjust_height(0, 0, 5);
	adjust_height(1, 0, 5);

	adjust_height(5, 5, 1);
	adjust_height(5, 4, 1);
	adjust_height(4, 5, 1);
	adjust_height(4, 4, 1);

	//build_vertex_array();
	build_buffers();
}

/*
void Terrain::generate_terrain() {
	Triangle t_one;
	Triangle t_two;

	for (int i = 0; i < _length; ++i) {
		t_one.vertex1.z = _segment_length * (i + 1);
		t_one.vertex2.z = _segment_length * i;
		t_one.vertex3.z = _segment_length * i;

		t_two.vertex1.z = t_one.vertex1.z;
		t_two.vertex2.z = t_one.vertex1.z;
		t_two.vertex3.z = t_one.vertex3.z;
		for (int j = 0; j < _width; ++j) {
			t_one.vertex1.x = _segment_width * j;
			t_one.vertex2.x = _segment_width * j;
			t_one.vertex3.x = _segment_width * (j + 1);

			t_two.vertex1.x = t_one.vertex1.x;
			t_two.vertex2.x = t_one.vertex3.x;
			t_two.vertex3.x = t_one.vertex3.x;

			_segments[i][j] = { t_one, t_two };
		}
	}
}
*/

void Terrain::generate_vertex_data() {
	_vertex_data.reserve(6);
	_vertex_data.push_back(glm::vec2(0.0f, 0.0f));
	_vertex_data.push_back(glm::vec2(_segment_width, 0.0f));
	_vertex_data.push_back(glm::vec2(0.0f, _segment_length));
	_vertex_data.push_back(glm::vec2(0.0f, _segment_length));
	_vertex_data.push_back(glm::vec2(_segment_width, 0.0f));
	_vertex_data.push_back(glm::vec2(_segment_width, _segment_length));
}

void Terrain::generate_position_data() {
	for (int i = 0; i < _length; ++i) {
		for (int j = 0; j < _width; ++j) {
			_positions.push_back(glm::vec2(j * _segment_width, i * _segment_length));
		}
	}
}

/*
void Terrain::build_vertex_array() {
	_vertices.clear();
	for (auto& col : _segments) {
		for (auto& segment : col) {
			_vertices.push_back(segment.triangle1.vertex1);
			_vertices.push_back(segment.triangle1.vertex2);
			_vertices.push_back(segment.triangle1.vertex3);
			_vertices.push_back(segment.triangle2.vertex1);
			_vertices.push_back(segment.triangle2.vertex2);
			_vertices.push_back(segment.triangle2.vertex3);
		}
	}
}
*/

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

void Terrain::adjust_vertex_height(int index, int vertex, float height) {
	if(index < 0 || index >= _height_map.size() ||
	   vertex < 0 || vertex > 6) {
		return;
	}

	_height_map[index].height[vertex] = height;
}

void Terrain::build_buffers() {
	glCreateVertexArrays(1, &_vao);

	glCreateBuffers(1, &_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glNamedBufferStorage(_vertex_buffer, sizeof(glm::vec2) * _vertex_data.size(), &_vertex_data[0], 0);

	glCreateBuffers(1, &_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _position_buffer);
	glNamedBufferStorage(_position_buffer, sizeof(glm::vec2) * _positions.size(), &_positions[0], 0);

	_program = 1;
}

void Terrain::draw() {
	glBindVertexArray(_vao);
	glUseProgram(_program);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _position_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(1, 1);


	glUniform1fv(glGetUniformLocation(_program, "height_map"), _height_map.size() * 6, &_height_map[0].height[0]);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _positions.size());
}