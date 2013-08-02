#ifndef PLANETGENERATOR_H
#define PLANETGENERATOR_H

#include "Simulation.h"
#include "Camera.h"
#include "Effect.h"
#include <noise/noise.h>
#include "Icosphere.h"

class PlanetGenerator : public Simulation
{
	protected:
		virtual void Initialize();
		virtual void LoadResources();
		virtual void PreRender();
		virtual void Render();
		virtual void PostRender();
		virtual void UnloadResources();

	private:
		glm::mat4 worldMatrix;
		GLuint vao;
		GLuint vbo[3];
		Icosphere* planet;
		Camera* camera;
		Effect* effect;
};

#endif