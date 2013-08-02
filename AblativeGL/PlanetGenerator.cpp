#include "PlanetGenerator.h"

void cubizePoint2(glm::vec3& position)
{
    double x,y,z;
    x = position.x;
    y = position.y;
    z = position.z;

    double fx, fy, fz;
    fx = fabsf(x);
    fy = fabsf(y);
    fz = fabsf(z);

    const double inverseSqrt2 = 0.70710676908493042;

    if (fy >= fx && fy >= fz) {
        double a2 = x * x * 2.0;
        double b2 = z * z * 2.0;
        double inner = -a2 + b2 -3;
        double innersqrt = -sqrtf((inner * inner) - 12.0 * a2);

        if(x == 0.0 || x == -0.0) { 
            position.x = 0.0; 
        }
        else {
            position.x = sqrtf(innersqrt + a2 - b2 + 3.0) * inverseSqrt2;
        }

        if(z == 0.0 || z == -0.0) {
            position.z = 0.0;
        }
        else {
            position.z = sqrtf(innersqrt - a2 + b2 + 3.0) * inverseSqrt2;
        }

        if(position.x > 1.0) position.x = 1.0;
        if(position.z > 1.0) position.z = 1.0;

        if(x < 0) position.x = -position.x;
        if(z < 0) position.z = -position.z;

        if (y > 0) {
            // top face
            position.y = 1.0;
        }
        else {
            // bottom face
            position.y = -1.0;
        }
    }
    else if (fx >= fy && fx >= fz) {
        double a2 = y * y * 2.0;
        double b2 = z * z * 2.0;
        double inner = -a2 + b2 -3;
        double innersqrt = -sqrtf((inner * inner) - 12.0 * a2);

        if(y == 0.0 || y == -0.0) { 
            position.y = 0.0; 
        }
        else {
            position.y = sqrtf(innersqrt + a2 - b2 + 3.0) * inverseSqrt2;
        }

        if(z == 0.0 || z == -0.0) {
            position.z = 0.0;
        }
        else {
            position.z = sqrtf(innersqrt - a2 + b2 + 3.0) * inverseSqrt2;
        }

        if(position.y > 1.0) position.y = 1.0;
        if(position.z > 1.0) position.z = 1.0;

        if(y < 0) position.y = -position.y;
        if(z < 0) position.z = -position.z;

        if (x > 0) {
            // right face
            position.x = 1.0;
        }
        else {
            // left face
            position.x = -1.0;
        }
    }
    else {
        double a2 = x * x * 2.0;
        double b2 = y * y * 2.0;
        double inner = -a2 + b2 -3;
        double innersqrt = -sqrtf((inner * inner) - 12.0 * a2);

        if(x == 0.0 || x == -0.0) { 
            position.x = 0.0; 
        }
        else {
            position.x = sqrtf(innersqrt + a2 - b2 + 3.0) * inverseSqrt2;
        }

        if(y == 0.0 || y == -0.0) {
            position.y = 0.0;
        }
        else {
            position.y = sqrtf(innersqrt - a2 + b2 + 3.0) * inverseSqrt2;
        }

        if(position.x > 1.0) position.x = 1.0;
        if(position.y > 1.0) position.y = 1.0;

        if(x < 0) position.x = -position.x;
        if(y < 0) position.y = -position.y;

        if (z > 0) {
            // front face
            position.z = 1.0;
        }
        else {
            // back face
            position.z = -1.0;
        }
	}
}

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
}

void PlanetGenerator::PreRender(){}

void PlanetGenerator::Render()
{
	(*effect)();
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	worldMatrix = glm::rotate(worldMatrix, 0.1f, glm::vec3(0, 1, 0));
	glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(45.0f, (float)WindowWidth() / (float)WindowHeight(), 1.0f, 1000.0f);
	glm::mat4 mvp = projection * view * worldMatrix;

	effect->SetUniform("MVP", false, mvp);

	glDrawElements(GL_TRIANGLES, planet->GetNumberOfIndices(), GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(window);
}

void PlanetGenerator::PostRender(){}

void PlanetGenerator::UnloadResources(){}