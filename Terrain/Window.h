#ifndef WINDOW_H
#define WINODW_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <string>

#define WINDOW_FILE "Data/system.txt"

constexpr GLfloat WINDOW_BACKGROUND_COLOR[4] = { 0.0, 0.0, 0.0, 1.0 };

struct Window_Settings {
	int width = 1000, height = 800;
	std::string title = "Window";
	int x_pos = 400, y_pos = 200;
};

class Camera;

class Window {
public:
	Window();
	~Window();

	void update();

	GLFWwindow* get_glfw_window();
	Camera* get_camera();
private:
	GLFWwindow* _window;
	Camera* _camera;
};

#endif