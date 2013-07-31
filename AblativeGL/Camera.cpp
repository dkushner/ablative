#define _USE_MATH_DEFINES
#define GLM_SWIZZLE

#include <math.h>
#include "Camera.h"

Camera::Camera(int width, int height)
{
	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);
	LookTarget = glm::vec3(0.0f, 0.0f, 1.0f);
	
	float aspect = (float)(width / height);
	float fov = 40.0f * (M_PI / 180.0f);
	projection = glm::perspectiveFov(fov, (float)width, (float)height, 1.0f, 1000.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
	return CreateView();
}
glm::mat4 Camera::GetProjectionMatrix()
{
	return projection;
}
glm::mat4 Camera::CreateView()
{
	glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 look = glm::vec4(LookTarget.x, LookTarget.y, LookTarget.z, 0.0f);
	glm::vec3 rot = Rotation * ((float)M_PI / 180.0f);

	glm::quat rot_quat = glm::quat(rot.yxz());

	look = rot_quat * look;
	up = rot_quat * up;
	return glm::lookAt(Position, Position + look.xyz(), up.xyz());
}
