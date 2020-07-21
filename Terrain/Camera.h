#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#define CAMERA_FORWARD 1
#define CAMERA_BACKWARD 2
#define CAMERA_LEFT 3
#define CAMERA_RIGHT 4
#define CAMERA_UP 5
#define CAMERA_DOWN 6

class Camera {
public:
	Camera(
		const int window_w,
		const int window_h,
		const float fov,
		const float aspect,
		const float z_near,
		const float z_far,
		const float horizontal_angle,
		const float vertical_angle,
		const glm::vec3 position
		);

	void move(const int direction);
	void move_angle(const float xpos, const float ypos);
	void update();

	void attach_shader(const GLuint program);
	void detach_shader(const GLuint program);

	glm::vec3 get_direction();
	glm::vec3 get_position();

	void set_position(glm::vec3 position);
private:
	int _window_w;
	int _window_h;

	float _fov;
	float _aspect;

	float _z_near;
	float _z_far;

	float _horizontal_angle;
	float _vertical_angle;

	glm::vec3 _direction;
	glm::vec3 _right;
	glm::vec3 _up;

	glm::mat4 _projection;
	glm::mat4 _view;

	glm::vec3 _position;

	std::vector<GLuint> _programs;
};

#endif