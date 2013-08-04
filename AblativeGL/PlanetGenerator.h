#ifndef PLANETGENERATOR_H
#define PLANETGENERATOR_H

#include "Simulation.h"
#include "Camera.h"
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

		virtual void OnKeyDown(Uint8, SDL_Keysym);

		void MoveFoward();
		void MoveBack();
		void MoveLeft();
		void MoveRight();

	private:
		glm::mat4 worldMatrix;
		glm::mat4 viewMatrix;
		Input input;
		GLuint vao;
		GLuint vbo[3];
		Icosphere* planet;
		Camera* camera;
		Effect* effect;
};

#endif