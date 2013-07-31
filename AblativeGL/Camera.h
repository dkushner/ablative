#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm\ext.hpp>

class Camera
{
	public:
		Camera(int, int);

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();

		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 LookTarget;

	private:
		glm::mat4 CreateView();

		glm::mat4 view;
		glm::mat4 projection;
};

#endif