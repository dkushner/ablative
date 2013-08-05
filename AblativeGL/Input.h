#ifndef INPUT_H
#define INPUT_H

#include <functional>
#include <map>
#include <SDL.h>

class Input
{
public:
	Input(void);
	~Input(void);

	void RegisterKeyEvent(SDL_Keycode key, std::function<void()> func);
	void UnregisterKeyEvent(SDL_Keycode key);

	void RegisterMouseMoveEvent(std::function<void(int, int, int, int)> func);

	void DoKeyEvents();

private:
	std::map<SDL_Scancode, std::function<void()>> function_map;
	std::function<void(int, int, int, int)> mouseMoveEvent;
};

#endif
