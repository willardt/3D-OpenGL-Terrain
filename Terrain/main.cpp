#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Environment.h"
#include "Clock.h"
#include "Window.h"
#include "Camera.h"
#include "Program.h"
#include "Mesh.h"
#include "Model.h"
#include "Terrain.h"

#define DEFAULT_DRAW GL_LINES
int TERRAIN_DRAW_MODE = DEFAULT_DRAW;

constexpr GLfloat BACKGROUND_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLuint SHADER = 0;

Mesh* MESH = nullptr;
Terrain* TERRAIN = nullptr;

float DEBUG_TILE_DRAW_X = 0.0f;
float DEBUG_TILE_DRAW_Z = 0.0f;
bool UPDATE_DRAW_TILE = false;

void build_environment() {
	Clock* clock = new Clock(0);
	Environment::get().set_clock(clock);

	Window* window = new Window();
	Environment::get().set_window(window);
}

void mouse_scroll(GLFWwindow* window, double xoffset, double yoffset) {
	auto camera = Environment::get().get_window()->get_camera();
	if (yoffset < 0) {
		camera->move(CAMERA_UP, 50000.0f);
	}
	if (yoffset > 0) {
		camera->move(CAMERA_DOWN, 50000.0f);
	}
}

void setup() {
	Program shader[] = { GL_VERTEX_SHADER, "Data\\vertex_shader.txt", GL_FRAGMENT_SHADER, "Data\\fragment_shader.txt", GL_NONE, "" };
	SHADER = load_shaders(shader);
	Environment::get().get_window()->get_camera()->attach_shader(SHADER);

	Program shader2[] = { GL_VERTEX_SHADER, "Data\\vertex_shader2.txt", GL_FRAGMENT_SHADER, "Data\\fragment_shader2.txt", GL_NONE, "" };
	GLuint shader2_id = load_shaders(shader2);
	Environment::get().get_window()->get_camera()->attach_shader(shader2_id);

	TERRAIN = new Terrain(100, 100, 10, 10);
	//	MESH = new Mesh();
		//for(int i = 0; i < sizeof(VERTEX_BUFFER_DATA) / sizeof(*VERTEX_BUFFER_DATA); i+=3) {
		//	MESH->_vertices.push_back(glm::vec3(VERTEX_BUFFER_DATA[i], VERTEX_BUFFER_DATA[i + 1], VERTEX_BUFFER_DATA[i + 2]));
		//}

	//	MESH->_vertices = TERRAIN->get_vertices();
	//	MESH->load_buffers();

	glfwSetScrollCallback(Environment::get().get_window()->get_glfw_window(), &mouse_scroll);

	
}

void mouse_hover() {
	auto window = Environment::get().get_window()->get_glfw_window();
	auto camera = Environment::get().get_window()->get_camera();

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	const float xpos_norm = xpos / (width / 2) - 1.0f;
	const float ypos_norm = -(ypos / (height / 2) - 1.0f);

	const glm::vec4 clip_space(xpos_norm, ypos_norm, -1.0f, 1.0f);
	const glm::mat4 inverse_projection = glm::inverse(camera->get_projection());
	glm::vec4 view_space = inverse_projection * clip_space;
	view_space.z = -1.0f;
	view_space.w = 0.0f;
	const glm::mat4 inverse_view = glm::inverse(camera->get_view());
	glm::vec3 world_space = inverse_view * view_space;
	world_space = glm::normalize(world_space);

	const float y_dif = abs(camera->get_position().y / world_space.y);
	const float tile_x = (y_dif * world_space.x + camera->get_position().x) / TERRAIN->get_tile_width();
	const float tile_z = (y_dif * world_space.z + camera->get_position().z) / TERRAIN->get_tile_length();

	DEBUG_TILE_DRAW_X = (float)(int)tile_x * TERRAIN->get_tile_width();
	DEBUG_TILE_DRAW_Z = (float)(int)tile_z * TERRAIN->get_tile_length();
}

void input() {
	glfwPollEvents();

	auto main_window = Environment::get().get_window();
	auto window = Environment::get().get_window()->get_glfw_window();
	auto camera = Environment::get().get_window()->get_camera();

	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		return;
	}

	mouse_hover();

	if (camera->get_mode() == CAMERA_FREE) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		camera->move_angle((float)xpos, (float)ypos);
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glfwSetCursorPos(window, width / 2, height / 2);
	}

	static bool key_1 = false;
	if (glfwGetKey(window, GLFW_KEY_1)) {
		if (!key_1) {
			camera->mode(CAMERA_TOGGLE);
			key_1 = true;
		}
	}
	else {
		key_1 = false;
	}

	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera->move(CAMERA_FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera->move(CAMERA_BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera->move(CAMERA_LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera->move(CAMERA_RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		camera->move(CAMERA_DOWN);
	}
	if (glfwGetKey(window, GLFW_KEY_E)) {
		camera->move(CAMERA_UP);
	}

	static bool key_2 = false;
	if (glfwGetKey(window, GLFW_KEY_2)) {
		if (!key_2) {
			if (TERRAIN_DRAW_MODE != GL_TRIANGLES) {
				TERRAIN_DRAW_MODE = GL_TRIANGLES;
			}
			else if (TERRAIN_DRAW_MODE == GL_TRIANGLES) {
				TERRAIN_DRAW_MODE = DEFAULT_DRAW;
			}
			key_2 = true;
		}
	}
	else {
		key_2 = false;
	}

	TERRAIN->select(DEBUG_TILE_DRAW_X / TERRAIN->get_tile_width(), DEBUG_TILE_DRAW_Z / TERRAIN->get_tile_length());
	static bool mouse_1 = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
		if (!mouse_1) {
			TERRAIN->adjust_tile_height(8);
			UPDATE_DRAW_TILE = true;
			mouse_1 = true;
		}
	}
	else {
		mouse_1 = false;
	}

	static bool mouse_2 = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		if (!mouse_2) {
			TERRAIN->adjust_ramp_height();
			UPDATE_DRAW_TILE = true;
			mouse_2 = true;
		}
	}
	else {
		mouse_2 = false;
	}
}

void debug_draw_tile() {
	/*
	static std::vector<glm::vec3> vertices;
	static Mesh tile;

	static float tile_x, tile_z;
	if (UPDATE_DRAW_TILE || tile_x != DEBUG_TILE_DRAW_X || tile_z != DEBUG_TILE_DRAW_Z)
	{
		tile_x = DEBUG_TILE_DRAW_X / TERRAIN->get_tile_width();
		tile_z = DEBUG_TILE_DRAW_Z / TERRAIN->get_tile_length();;
		vertices.clear();
		auto height = TERRAIN->get_tile_height(tile_x, tile_z);
		const float width = TERRAIN->get_tile_width();
		const float length = TERRAIN->get_tile_length();
		vertices.push_back({ width, height[0] + 0.01f, 0.0f });
		vertices.push_back({ 0.0f, height[1] + 0.01f, 0.0f });
		vertices.push_back({ 0.0f, height[2] + 0.01f, length });
		vertices.push_back({ width, height[3] + 0.01f, 0.0f });
		vertices.push_back({ 0.0f, height[4] + 0.01f, length });
		vertices.push_back({ width, height[5] + 0.01f, length });

		tile._vertices = vertices;
		tile.load_buffers();
		UPDATE_DRAW_TILE = false;
	}

	static bool load = false;
	if (!load) {
		const float width = TERRAIN->get_tile_width();
		const float length = TERRAIN->get_tile_length();
		vertices.push_back({ 0.0f, 0.01f, 0.0f });
		vertices.push_back({ width, 0.01f, 0.0f });
		vertices.push_back({ 0.0f, 0.01f, length });
		vertices.push_back({ 0.0f, 0.01f, length });
		vertices.push_back({ width, 0.01f, 0.0f });
		vertices.push_back({ width, 0.01f, length });

		tile._vertices = vertices;
		tile.load_buffers();
		load = true;
	}

	Transform position({ DEBUG_TILE_DRAW_X, 0.0f, DEBUG_TILE_DRAW_Z });
	tile.draw_vertices(2, position, GL_TRIANGLES);
	*/
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, BACKGROUND_COLOR);

	//static Transform transform;
	//MESH->draw_vertices(SHADER, transform, GL_LINE_STRIP);

	TERRAIN->draw(TERRAIN_DRAW_MODE);
	//debug_draw_tile();

	//static Model model(1, "Data\\", "link.obj");
	//model.draw(transform);

	glfwSwapBuffers(Environment::get().get_window()->get_glfw_window());
}

int main() {

	glfwInit();

	Environment environment;
	build_environment();

	setup();

	auto window = Environment::get().get_window()->get_glfw_window();

	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		render();
		input();
		Environment::get().get_clock()->update();
		Environment::get().get_window()->update();
	}

	delete MESH;

	return 0;
}