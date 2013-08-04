#include "PhysXFluid.h"

void PhysXFluid::Initialize()
{
	// Set window hints and context attributes here.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	// Ask parent to generate our context.
	CreateContext();

	// Set up some interaction;
	SDL_ShowCursor(false);
	SDL_GetWindowGrab(window);


	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if(GLEW_OK != error)
	{
		fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(error));
		return;
	}
}
void PhysXFluid::LoadResources()
{
	const GLfloat pyramid[15] = 
		{
			0.0, 0.5, 0.0,
			-1.0,  -0.5, 1.0,
			1.0, -0.5, 1.0, 
			1.0,  -0.5, -1.0, 
			-1.0, -0.5, -1.0 
		};
		const GLfloat colors[15] = 
		{
			0.0,  0.0,  0.0,
			1.0,  0.0,  0.0,
			0.0,  1.0,  0.0,
			0.0,  0.0,  1.0, 
			1.0,  1.0,  0.0 
		};

		rot_acc = 0.0f;
		effect = new Effect("BasicVertex.vert", "BasicFragment.frag");
		camera = new Camera(WindowWidth(), WindowHeight());
		camera->Position.z = -50.0f;
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(2, vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), pyramid, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glEnable(GL_DEPTH_TEST);
}
void PhysXFluid::PreRender(){}
void PhysXFluid::Render()
{
	(*effect)();
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 identity(1.0f);
	glm::mat4 translation = glm::translate(identity, glm::vec3(0.0f, 0.0f, -20.0f));
	glm::mat4 rotation = glm::rotate(translation, rot_acc, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * rotation;

	effect->SetUniform("MVP", false, mvp);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
	SDL_GL_SwapWindow(window);
	rot_acc += 0.5f;
}
void PhysXFluid::PostRender(){}
void PhysXFluid::UnloadResources(){}

void PhysXFluid::OnKeyDown(Uint8 state, SDL_Keysym key)
{
	switch(key.sym)
	{
		case SDLK_w:
			camera->Position += glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		case SDLK_a:
			camera->Position += glm::vec3(-1.0f, 0.0f, 0.0f);
			break;
		case SDLK_s:
			camera->Position += glm::vec3(0.0f, 0.0f, -1.0f);
			break;
		case SDLK_d:
			camera->Position += glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case SDLK_SPACE:
			camera->Position += glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		default:
			break;
	}
}
void PhysXFluid::OnMouseMove(Uint8 state, Uint16 x, Uint16 y, Sint16 relX, Sint16 relY)
{
	glm::vec2 motion(-1.0f * (x - (WindowWidth() / 2.0f)), y - (WindowHeight() / 2.0f));
	motion *= (1 / 100.0f);
	
	camera->Rotation += glm::vec3(motion, 0.0f);
	glm::clamp(camera->Rotation, glm::vec3(-180.0f, -180.0f, -180.0f), glm::vec3(180.0f, 180.0f, 180.0f));

	SDL_WarpMouseInWindow(window, WindowWidth() / 2.0f, WindowHeight() / 2.0f);
}
