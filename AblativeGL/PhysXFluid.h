#ifndef PHYSXFLUID_H
#define PHYSXFLUID_H

#include "Simulation.h"
#include "Effect.h"
#include "Camera.h"
#include <memory>

class PhysXFluid : public Simulation
{
	protected:
		virtual void Initialize();
		virtual void LoadResources();
		virtual void PreRender();
		virtual void Render();
		virtual void PostRender();
		virtual void UnloadResources();

		virtual void OnMouseMove(Uint8, Uint16, Uint16, Sint16, Sint16);
		virtual void OnKeyDown(Uint8, SDL_Keysym);

	private:
		Camera* camera;
		float rot_acc;
		Effect* effect;
		GLuint vao;
		GLuint vbo[2];
};

#endif
