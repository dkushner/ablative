#ifndef PLANETGENERATOR_H
#define PLANETGENERATOR_H

#include "Simulation.h"
#include "Camera.h"
#include "FreelookCamera.h"
#include "Effect.h"
#include <noise/noise.h>
#include "Icosphere.h"
#include "Input.h"
#include <math.h>

class PlanetGenerator : public Simulation
{
	protected:
		virtual void Initialize();
		virtual void LoadResources();
		virtual void PreRender();
		virtual void Render();
		virtual void PostRender();
		virtual void UnloadResources();

		void MouseMove(int x, int y, int relx, int rely);

	private:
		FreelookCamera freelookCam;
		glm::mat4 worldMatrix;
		Input input;
		GLuint vao;
		GLuint vbo[3];
		Icosphere* planet;
		Camera* camera;
		Effect* effect;
};

#endif