#include "Simulation.h"

Simulation::Simulation()
{
	// Init SDL and subsystems.
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return;
	}
}
Simulation::~Simulation()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Simulation::Run(Configuration& config)
{
	this->config = config;

	Initialize();
	LoadResources();

	bool run = true;
	SDL_Event event;
	while(run)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				run = false;
			
			if((event.type == SDL_KEYDOWN) && event.key.keysym.sym == SDLK_ESCAPE)
				run = false;

			switch(event.type)
			{
				case SDL_MOUSEMOTION:
					OnMouseMove(event.motion.state, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
					break;
				case SDL_KEYDOWN:
					Input::KeyPressed(event.key.keysym);
					break;
				case SDL_KEYUP:
					Input::KeyReleased(event.key.keysym);
					break;
				default:
					break;
			}
		}
		Tick();
		Frame();
	}
	UnloadResources();
}
void Simulation::CreateContext()
{
	window = SDL_CreateWindow(config.WindowTitle.c_str(),
														SDL_WINDOWPOS_CENTERED, 
														SDL_WINDOWPOS_CENTERED, 
														config.WindowWidth,
														config.WindowHeight,
														SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if(config.Fullscreen)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	if(!window)
	{
		fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
		return;
	}

	context = SDL_GL_CreateContext(window);
	if(!context)
	{
		fprintf(stderr, "Failed to create context: %s\n", SDL_GetError());
		return;
	}

	// Set swap interval,
	SDL_GL_SetSwapInterval(1);

	// Context creation successful.
	int major, minor;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);

	printf("Created OpenGL Context (v%d.%d)\n", major, minor);
}

void Simulation::Frame()
{
	PreRender();
	Render();
	PostRender();
}
void Simulation::Tick()
{
	// TODO: Frame time and FPS logic.
	Update();
}

int Simulation::WindowWidth()
{
	return config.WindowWidth;
}
int Simulation::WindowHeight()
{
	return config.WindowHeight;
}
string Simulation::WindowTitle()
{
	return config.WindowTitle;
}
