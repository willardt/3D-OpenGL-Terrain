#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Clock;
class Window;

class Environment {
public:
	Environment();
	~Environment();

	void set_clock(Clock* clock);
	void set_window(Window* window);

	Clock* get_clock();
	Window* get_window();

	void shut_down();

	static Environment& get();
private:
	Clock* _clock;
	Window* _window;

	static Environment* _instance;
};

#endif