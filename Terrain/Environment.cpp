#include "Environment.h"

#include "Window.h"
#include "Clock.h"

#include <cassert>

Environment* Environment::_instance = nullptr;

Environment::Environment() :
	_clock(nullptr),
	_window(nullptr)
{
	assert(!_instance);
	_instance = this;
}

Environment::~Environment() {
	shut_down();
	_instance = nullptr;
}

Environment& Environment::get() {
	assert(_instance);
	return *_instance;
}

void Environment::set_clock(Clock* clock) {
	_clock = clock;
}

void Environment::set_window(Window* window) {
	_window = window;
}

Clock* Environment::get_clock() {
	return _clock;
}

Window* Environment::get_window() {
	return _window;
}

void Environment::shut_down() {
	if (_window) {
		delete _window;
		_window = nullptr;
	}

	if (_clock) {
		delete _clock;
		_clock = nullptr;
	}
}