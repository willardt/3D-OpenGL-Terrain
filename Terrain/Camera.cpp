#include "Camera.h"

#include "Environment.h"
#include "Clock.h"

constexpr float SPEED = 10000.0f;
constexpr float HALF_PIE = 3.14159f / 2.0f;

Camera::Camera(
	const int   window_w,
	const int   window_h,
	const float fov,
	const float aspect,
	const float z_near,
	const float z_far,
	const float horizontal_angle,
	const float vertical_angle,
	const glm::vec3 position
	) :
	_window_w(window_w),
	_window_h(window_h),
	_fov(fov),
	_aspect(aspect),
	_z_near(z_near),
	_z_far(z_far),
	_horizontal_angle(horizontal_angle),
	_vertical_angle(vertical_angle),
	_position(position),
	_projection(glm::perspective(fov, aspect, z_near, z_far))
{}

void Camera::attach_shader(GLuint program) {
	_programs.push_back(program);

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &_projection[0][0]);
}

void Camera::detach_shader(GLuint program) {
	for (auto it = _programs.begin(); it != _programs.end(); ++it) {
		if (*it == program) {
			_programs.erase(it);
		}
	}
}

void Camera::move(const int direction) {
	if (direction == CAMERA_FORWARD) {
		_position.x += _direction.x * SPEED * (float)Environment::get().get_clock()->get_time();
		_position.z += _direction.z * SPEED * (float)Environment::get().get_clock()->get_time();
	}

	if (direction == CAMERA_BACKWARD) {
		_position.x -= _direction.x * SPEED * (float)Environment::get().get_clock()->get_time();
		_position.z -= _direction.z * SPEED * (float)Environment::get().get_clock()->get_time();
	}

	if (direction == CAMERA_LEFT) {
		_position.x -= _right.x * (float)Environment::get().get_clock()->get_time() * SPEED;
		_position.z -= _right.z * (float)Environment::get().get_clock()->get_time() * SPEED;
	}

	if (direction == CAMERA_RIGHT) {
		_position.x += _right.x * (float)Environment::get().get_clock()->get_time() * SPEED;
		_position.z += _right.z * (float)Environment::get().get_clock()->get_time() * SPEED;
	}

	if (direction == CAMERA_UP) {
		_position.y += (float)Environment::get().get_clock()->get_time() * SPEED;
		//_position.y += 1;
	}

	if (direction == CAMERA_DOWN) {
		_position.y -= (float)Environment::get().get_clock()->get_time() * SPEED;
		//_position.y -= 1;
	}
}

void Camera::move_angle(const float xpos, const float ypos) {
	_horizontal_angle += 0.001f * ((_window_w / 2) - xpos);
	_vertical_angle += 0.001f * ((_window_h / 2) - ypos);
}

void Camera::update() {
	_direction = glm::vec3(
		cos(_vertical_angle) * sin(_horizontal_angle),
		sin(_vertical_angle),
		cos(_vertical_angle) * cos(_horizontal_angle)
		);

	_right = glm::vec3(
		sin(_horizontal_angle - HALF_PIE),
		0,
		cos(_horizontal_angle - HALF_PIE)
		);

	_up = glm::cross(_right, _direction);

	_view = glm::lookAt(
		_position,
		_position + _direction,
		_up
		);

	for (auto p : _programs) {
		glUseProgram(p);
		glUniformMatrix4fv(glGetUniformLocation(p, "view"), 1, GL_FALSE, &_view[0][0]);
	}
}

glm::vec3 Camera::get_direction() {
	return _direction;
}

glm::vec3 Camera::get_position() {
	return _position;
}

void Camera::set_position(glm::vec3 position) {
	_position = position;
}