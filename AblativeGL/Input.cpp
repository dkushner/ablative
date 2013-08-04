#include "Input.h"

bool Input::keyStates[512];

Input::Input(void)
{
}

void Input::KeyPressed(SDL_Keysym key)
{
	keyStates[key.scancode] = true;
}

void Input::KeyReleased(SDL_Keysym key)
{
	keyStates[key.scancode] = false;
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

void Input::DoKeyEvents()
{
	for(int i = 0; i < 512; i++)
	{
		if(keyStates[i])
		{
			//calls the function with the assocaited scan code
			auto it = function_map.find((const SDL_Scancode)i);
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
