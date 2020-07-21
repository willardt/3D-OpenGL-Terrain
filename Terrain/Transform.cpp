#include "Transform.h"

Transform::Transform(
	const glm::vec3 position, const glm::vec3 scale, const glm::vec3 rotation) :
	_position(position),
	_scale(scale),
	_rotation(rotation),
	_rotation_matrix(glm::mat4(1))
{
	setup_matrices();
}

Transform::Transform(const Transform& rhs) :
	_position(rhs._position),
	_scale(rhs._scale),
	_rotation(rhs._rotation),
	_position_matrix(rhs._position_matrix),
	_scale_matrix(rhs._scale_matrix),
	_rotation_matrix(rhs._rotation_matrix),
	_model(rhs._model)
{}

void Transform::setup_matrices() {
	_position_matrix = glm::translate(glm::mat4(1.0f), _position);
	_scale_matrix = glm::scale(glm::mat4(1.0f), _scale);
	_rotation_matrix_x = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	_rotation_matrix_y = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	_rotation_matrix_z = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0, 0, 1));

	_rotation_matrix = _rotation_matrix_x * _rotation_matrix_y * _rotation_matrix_z;
	_model = _position_matrix * _rotation_matrix * _scale_matrix;
}

void Transform::set_position(const glm::vec3 posiiton) {
	_position = posiiton;
	_position_matrix = glm::translate(glm::mat4(1.0f), _position);

	_model = _position_matrix * _rotation_matrix * _scale_matrix;
}

void Transform::set_scale(const glm::vec3 scale) {
	_scale = scale;
	_scale_matrix = glm::scale(glm::mat4(1.0f), _scale);

	_model = _position_matrix * _rotation_matrix * _scale_matrix;
}

void Transform::set_rotation(const glm::vec3 rotation) {
	_rotation = rotation;
	_rotation_matrix_x = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0));
	_rotation_matrix_y = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0));
	_rotation_matrix_z = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));

	_rotation_matrix = _rotation_matrix_x * _rotation_matrix_y * _rotation_matrix_z;
	_model = _position_matrix * _rotation_matrix * _scale_matrix;
}

glm::mat4 Transform::get_model() {
	return _model;
}

glm::vec3 Transform::get_position() {
	return _position;
}

glm::vec3 Transform::get_scale() {
	return _scale;
}

glm::vec3 Transform::get_rotation() {
	return _rotation;
}