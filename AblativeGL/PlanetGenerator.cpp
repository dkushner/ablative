#include "PlanetGenerator.h"
#include "SphereMapping.h"
void PlanetGenerator::Initialize()
{
	// Set window hints and context attributes here.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

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

void PlanetGenerator::MouseMove(int x, int y, int relx, int rely)
{
	int relativex = x - WindowWidth()/2;
	int relativey = y - WindowHeight()/2;
	freelookCam.Orient(relativey/3.0f, relativex/3.0f);
}

void PlanetGenerator::LoadResources()
{
		planet = new Icosphere(8);	
		camera = new Camera(WindowWidth(), WindowHeight());
		effect = new Effect("diffuse.vert", "diffuse.frag");

		noise::module::RidgedMulti rigidGen;
		noise::module::Perlin perlinGen;
		perlinGen.SetFrequency(2);
		rigidGen.SetFrequency(3);
		rigidGen.SetSeed(3037);
		noise::module::Max maxCombiner;
		perlinGen.SetSeed(3032);
		maxCombiner.SetSourceModule(0, rigidGen);
		maxCombiner.SetSourceModule(1, perlinGen);
		noise::module::Clamp clampGen;
		clampGen.SetSourceModule(0, maxCombiner);
		clampGen.SetBounds(-1.0, 2.0);
		glm::vec3* vertices;
		vertices = planet->GetVertices();

		for(int i = 0; i < planet->GetNumberOfVertices(); i++)
		{
				float scaler = clampGen.GetValue(vertices[i].x, vertices[i].y, vertices[i].z);
				scaler = scaler * 0.02f;
				scaler += 1.0f;
				glm::vec3 newVertex = vertices[i] * scaler;
				vertices[i] = newVertex;
		}

		std::cout << "Number Of vertices: " << planet->GetNumberOfVertices() << std::endl;

		planet->GenerateNormals();

		for(int i = 0; i < planet->GetNumberOfVertices(); i++)
		{
			cubizePoint2(vertices[i]);
		}

		worldMatrix = glm::scale(glm::vec3(1000, 1000, 1000));

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(3, vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, planet->GetNumberOfVertices() * sizeof(GL_FLOAT)*3, planet->GetVertices(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, planet->GetNumberOfNormals() * sizeof(GL_FLOAT)*3, planet->GetNormals(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, planet->GetNumberOfIndices() * sizeof(GL_UNSIGNED_INT), planet->GetIndices(), GL_STATIC_DRAW);
		glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);
		glEnable(GL_DEPTH_TEST);

		freelookCam.Initialize(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, 3200));

		input.RegisterKeyEvent(SDLK_w, std::bind(&FreelookCamera::MoveForward, &freelookCam));
		input.RegisterKeyEvent(SDLK_s, std::bind(&FreelookCamera::MoveBackward, &freelookCam));
		input.RegisterKeyEvent(SDLK_a, std::bind(&FreelookCamera::MoveLeft, &freelookCam));
		input.RegisterKeyEvent(SDLK_d, std::bind(&FreelookCamera::MoveRight, &freelookCam));
		input.RegisterMouseMoveEvent(std::bind(&PlanetGenerator::MouseMove, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

void PlanetGenerator::PreRender()
{
	SDL_WarpMouseInWindow(window, WindowWidth() / 2, WindowHeight() / 2);
	input.DoKeyEvents();
}

void PlanetGenerator::Render()
{
	(*effect)();
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//worldMatrix = glm::rotate(worldMatrix, 0.1f, glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(45.0f, (float)WindowWidth() / (float)WindowHeight(), 1.0f, 100000.0f);
	float angle = glm::dot(freelookCam.GetRight(), glm::normalize(freelookCam.Position()));
	freelookCam.SetRoll(angle);
	glm::mat4 mvp = projection * freelookCam.ViewMatrix() * worldMatrix;

	effect->SetUniform("MVP", false, mvp);

	glDrawElements(GL_TRIANGLES, planet->GetNumberOfIndices(), GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(window);
}

void PlanetGenerator::PostRender()
{
}

void PlanetGenerator::UnloadResources(){}
