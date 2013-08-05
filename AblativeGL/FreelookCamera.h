#ifndef FREELOOKCAMERA_H
#define FREELOOKCAMERA_H

#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm/ext.hpp>

class FreelookCamera
{
public:
	FreelookCamera();
	void Initialize(glm::vec3 up, glm::vec3 front, glm::vec3 pos);

	void SetRoll(float degrees);

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	void Orient(float pitch, float yaw);

	glm::vec3 GetUp();
	glm::vec3 GetRight();
	glm::vec3 GetFront();
	glm::vec3 Position();
	glm::mat4 ViewMatrix();

private:
	glm::quat orientation;

	glm::vec3 position;

	glm::vec3 upVector;
	glm::vec3 rightVector;
	glm::vec3 frontVector;
};

#endif