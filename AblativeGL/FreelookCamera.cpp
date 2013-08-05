#include "FreelookCamera.h"

FreelookCamera::FreelookCamera()
{

}

glm::fquat fromAxisAngle(const glm::vec3 axis, float degrees)
{
    float halfTheta = (degrees * (glm::pi<float>() / 180.0f)) / 2.0f;
    float s = sinf(halfTheta);
    return glm::fquat(cos(halfTheta), axis.x * s, axis.y * s, axis.z * s);
}

void FreelookCamera::Initialize(glm::vec3 up, glm::vec3 front, glm::vec3 pos)
{
	upVector = up;
	frontVector = front;
	rightVector = glm::cross(upVector, frontVector);
	orientation = glm::quat(1.0f, frontVector.x, frontVector.y, frontVector.z);
	position = pos;
}

glm::vec3 FreelookCamera::Position()
{
	return position;
}

void FreelookCamera::MoveForward()
{
	position -= frontVector;
}

void FreelookCamera::MoveBackward()
{
	position += frontVector;
}

void FreelookCamera::MoveRight()
{
	position += rightVector;
}

void FreelookCamera::MoveLeft()
{
	position -= rightVector;
}

void FreelookCamera::Orient(float pitch, float yaw)
{
	glm::fquat rotation;

	if(pitch != 0)
	{
		rotation = fromAxisAngle(rightVector, pitch);
		orientation = orientation * rotation;
		orientation = glm::normalize(orientation);
	}

	if(yaw != 0)
	{
		rotation = fromAxisAngle(upVector, yaw);
		orientation = orientation * rotation;
		orientation = glm::normalize(orientation);
	}
}

void FreelookCamera::SetRoll(float degrees)
{
	glm::fquat rotation;
	
	if(degrees != 0)
	{
		rotation = fromAxisAngle(frontVector, degrees);
		orientation = rotation * orientation;
		orientation = glm::normalize(orientation);
	}
}

glm::vec3 FreelookCamera::GetUp()
{
	return upVector;
}

glm::vec3 FreelookCamera::GetRight()
{
	return rightVector;
}

glm::vec3 FreelookCamera::GetFront()
{
	return -frontVector;
}

glm::mat4 FreelookCamera::ViewMatrix()
{
	glm::mat4 viewMatrix = glm::toMat4(orientation);
	rightVector = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	upVector = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	frontVector = glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);

	viewMatrix[3][0] = -glm::dot(rightVector, position);
	viewMatrix[3][1] = -glm::dot(upVector, position);
	viewMatrix[3][2] = -glm::dot(frontVector, position);

	return viewMatrix;
}