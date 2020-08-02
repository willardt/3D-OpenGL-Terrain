#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include <vector>

#include "Texture.h"

struct Tile_Height {
	GLfloat height[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

class Tile_Highlight {
public:
private:

};

class Terrain {
public:
	Terrain(int width, int length, float tile_width, float tile_length);
	~Terrain();

	void draw(int mode);
	void adjust_tile_height(int x, int z, float height);
	void adjust_ramp_height(int x, int z);
	void adjust_vertex_height(int index, int vertex, float height);

	float get_tile_width();
	float get_tile_length();
	std::vector<GLfloat> get_tile_height(int x, int z);
	float get_vertex_height(int index, int vertex);
	bool get_tile_is_flat(int index);
private:
	void generate_vertex_data();
	void generate_uv_data();
	void generate_position_data();
	void load_textures();
	void create_vao();
private:
	int _width;
	int _length;
	float _tile_width;
	float _tile_length;

	std::vector<glm::vec2> _vertex_data;
	std::vector<glm::vec2> _uv_data;
	std::vector<glm::vec2> _position_data;
	std::vector<Tile_Height> _height_map;

	GLuint _vao;
	GLuint _program;
	GLuint _vertex_buffer;
	GLuint _uv_buffer;
	GLuint _position_buffer;
	GLuint _height_buffer;
	GLuint _height_texture;

	Texture _tile_texture;
};

#endif