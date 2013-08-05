#ifndef SPHEREMAPPING_H
#define SPHEREMAPPING_H

#include <glm\glm.hpp>

void cubizePoint2(glm::vec3& position);
glm::vec3 mapToSphere(float u, float v);

#endif