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

	void DoKeyEvents();

	//Backend
	static void KeyPressed(SDL_Keysym key);
	static void KeyReleased(SDL_Keysym key);

private:
	std::map<SDL_Scancode, std::function<void()>> function_map;
	static bool keyStates[512];
};

#endif
