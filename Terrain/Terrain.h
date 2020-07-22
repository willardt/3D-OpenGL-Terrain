#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include <vector>

/*
struct Triangle {
	glm::vec3 vertex1 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 vertex2 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 vertex3 = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct Segment {
	Triangle triangle1;
	Triangle triangle2;
};
*/

// six vertices per segment
struct Segment_Height {
	GLfloat height[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

class Terrain {
public:
	Terrain(int width, int length, float segment_width, float segment_length);

	void draw();
	void adjust_vertex_height(int index, int vertex, float height);
	void adjust_height(int x, int z, float height);
private:
	void generate_vertex_data();
	void generate_position_data();
	//void build_vertex_array();
	void build_buffers();
private:
	int _width;
	int _length;
	float _segment_width;
	float _segment_length;

	//std::vector<std::vector<Segment>> _segments;

	std::vector<glm::vec2> _vertex_data;
	std::vector<glm::vec2> _positions;
	std::vector<Segment_Height> _height_map;

	GLuint _vao;
	GLuint _program;
	GLuint _vertex_buffer;
	GLuint _position_buffer;
	GLuint _height_buffer;
	GLuint _height_texture;
};

#endif