#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "Environment.h"
#include "Clock.h"
#include "Window.h"
#include "Camera.h"
#include "Program.h"
#include "Mesh.h"
#include "Model.h"
#include "Terrain.h"

constexpr GLfloat BACKGROUND_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLuint SHADER = 0;

Mesh* MESH = nullptr;
Terrain* TERRAIN = nullptr;

static const GLfloat VERTEX_BUFFER_DATA[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

void build_environment() {
	Clock* clock = new Clock(0);
	Environment::get().set_clock(clock);

	Window* window = new Window();
	Environment::get().set_window(window);
}

void setup() {
	Program shader[] = { GL_VERTEX_SHADER, "Data\\vertex_shader.txt", GL_FRAGMENT_SHADER, "Data\\fragment_shader.txt", GL_NONE, "" };
	SHADER = load_shaders(shader);
	Environment::get().get_window()->get_camera()->attach_shader(SHADER);

	TERRAIN = new Terrain(100, 100, 0.1, 0.1);
//	MESH = new Mesh();
	//for(int i = 0; i < sizeof(VERTEX_BUFFER_DATA) / sizeof(*VERTEX_BUFFER_DATA); i+=3) {
	//	MESH->_vertices.push_back(glm::vec3(VERTEX_BUFFER_DATA[i], VERTEX_BUFFER_DATA[i + 1], VERTEX_BUFFER_DATA[i + 2]));
	//}

//	MESH->_vertices = TERRAIN->get_vertices();
//	MESH->load_buffers();
}

void input() {
	glfwPollEvents();

	auto window = Environment::get().get_window()->get_glfw_window();
	auto camera = Environment::get().get_window()->get_camera();

	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		return;
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	camera->move_angle((float)xpos, (float)ypos);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);

	if(glfwGetKey(window, GLFW_KEY_W)) {
		camera->move(CAMERA_FORWARD);
	}
	if(glfwGetKey(window, GLFW_KEY_S)) {
		camera->move(CAMERA_BACKWARD);
	}
	if(glfwGetKey(window, GLFW_KEY_A)) {
		camera->move(CAMERA_LEFT);
	}
	if(glfwGetKey(window, GLFW_KEY_D)) {
		camera->move(CAMERA_RIGHT);
	}
	if(glfwGetKey(window, GLFW_KEY_Q)) {
		camera->move(CAMERA_DOWN);
	}
	if(glfwGetKey(window, GLFW_KEY_E)) {
		camera->move(CAMERA_UP);
	}
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, BACKGROUND_COLOR);

	//static Transform transform;
	//MESH->draw_vertices(SHADER, transform, GL_LINE_STRIP);
	
	TERRAIN->draw();

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

	while(!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		render();
		input();
		Environment::get().get_clock()->update();
		Environment::get().get_window()->update();
	}

	delete MESH;

	return 0;
}