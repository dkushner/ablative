#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Configuration.h"

#define FPS_RESOLUTION 10

using namespace std;

class Simulation
{
	public:
		Simulation();
		~Simulation();

		void Run(Configuration&);
		int WindowWidth();
		int WindowHeight();
		string WindowTitle();
		float FrameDelta();

	protected:
		void CreateContext();

		virtual void Initialize(){}
		virtual void LoadResources(){}
		virtual void Update(){}
		virtual void PreRender(){}
		virtual void Render(){}
		virtual void PostRender(){}
		virtual void UnloadResources(){}

		virtual void OnMouseMove(Uint8, Uint16, Uint16, Sint16, Sint16){}
		virtual void OnKeyDown(Uint8, SDL_Keysym){}
		virtual void OnKeyUp(Uint8, SDL_Keysym){}
		
		SDL_Window* window;
		SDL_GLContext context;
	
	private:
		void Tick();
		void Frame();

		Uint32 frame_times[FPS_RESOLUTION];
		Uint32 last_frame;
		Uint32 frame_count;
		float fps;

		SDL_TimerID timer_id;
		Configuration config;

};

#endif
