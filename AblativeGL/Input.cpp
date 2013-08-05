#include "Input.h"

Input::Input(void)
{
}

void Input::RegisterKeyEvent(SDL_Keycode key, std::function<void()> action)
{
	auto pair = std::pair<SDL_Scancode, std::function<void()>>(SDL_GetScancodeFromKey(key), action);
	function_map.insert(function_map.begin(), pair);
}

void Input::UnregisterKeyEvent(SDL_Keycode key)
{
	SDL_Scancode scan_code = SDL_GetScancodeFromKey(key);
	auto it = function_map.find(scan_code);
	if(it != function_map.end())
	{
		function_map.erase(it);
	}
}

void Input::RegisterMouseMoveEvent(std::function<void(int, int, int, int)> func)
{
	mouseMoveEvent = func;
}

void Input::DoKeyEvents()
{
	int numOfKeys;
	const Uint8* keyStates = SDL_GetKeyboardState(&numOfKeys);
	int mouse_x;
	int mouse_y;
	int mouse_relx;
	int mouse_rely;

	SDL_GetMouseState(&mouse_x, &mouse_y);
	SDL_GetRelativeMouseState(&mouse_relx, &mouse_rely);

	if(mouseMoveEvent)
	{
		mouseMoveEvent(mouse_x, mouse_y, mouse_relx, mouse_rely);
	}

	for(int i = 0; i < numOfKeys; i++)
	{
		if(keyStates[i])
		{
			//calls the function with the associated scan code
			auto it = function_map.find((SDL_Scancode)i);
			if(it != function_map.end())
			{
				it->second();
			}
		}
	}
}

Input::~Input(void)
{
}
