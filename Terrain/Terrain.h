#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include <vector>

struct Tile_Height {
	GLfloat height[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

class Terrain {
public:
	Terrain(int width, int length, float tile_width, float tile_length);
	~Terrain();

	void draw(int mode);
	void adjust_tile_height(int x, int z, float height);
	void adjust_vertex_height(int index, int vertex, float height);
	void adjust_height(int x, int z, float height);

	float get_tile_width();
	float get_tile_length();
private:
	void generate_vertex_data();
	void generate_position_data();
	void build_buffers();
private:
	int _width;
	int _length;
	float _tile_width;
	float _tile_length;

	std::vector<glm::vec2> _vertex_data;
	std::vector<glm::vec2> _positions;
	std::vector<Tile_Height> _height_map;

	GLuint _vao;
	GLuint _program;
	GLuint _vertex_buffer;
	GLuint _position_buffer;
	GLuint _height_buffer;
	GLuint _height_texture;
};

#endif