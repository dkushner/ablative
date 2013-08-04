#include "SphereMapping.h"

void cubizePoint2(glm::vec3& position)
{
    double x,y,z;
    x = position.x;
    y = position.y;
    z = position.z;

    double fx, fy, fz;
    fx = std::fabsf(x);
    fy = std::fabsf(y);
    fz = std::fabsf(z);

    const double invSqrt2 = 0.70710676908493042;

    if (fy >= fx && fy >= fz) {
        double a2 = x * x * 2.0;
        double b2 = z * z * 2.0;
        double inner = -a2 + b2 -3;
        double innersqrt = -std::sqrtf((inner * inner) - 12.0 * a2);

        if(x == 0.0 || x == -0.0) { 
            position.x = 0.0; 
        }
        else {
            position.x = std::sqrtf(innersqrt + a2 - b2 + 3.0) * invSqrt2;
        }

        if(z == 0.0 || z == -0.0) {
            position.z = 0.0;
        }
        else {
            position.z = std::sqrtf(innersqrt - a2 + b2 + 3.0) * invSqrt2;
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
        double innersqrt = -std::sqrtf((inner * inner) - 12.0 * a2);

        if(y == 0.0 || y == -0.0) { 
            position.y = 0.0; 
        }
        else {
            position.y = std::sqrtf(innersqrt + a2 - b2 + 3.0) * invSqrt2;
        }

        if(z == 0.0 || z == -0.0) {
            position.z = 0.0;
        }
        else {
            position.z = std::sqrtf(innersqrt - a2 + b2 + 3.0) * invSqrt2;
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
        double innersqrt = -std::sqrtf((inner * inner) - 12.0 * a2);

        if(x == 0.0 || x == -0.0) { 
            position.x = 0.0; 
        }
        else {
            position.x = std::sqrtf(innersqrt + a2 - b2 + 3.0) * invSqrt2;
        }

        if(y == 0.0 || y == -0.0) {
            position.y = 0.0;
        }
        else {
            position.y = std::sqrtf(innersqrt - a2 + b2 + 3.0) * invSqrt2;
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

glm::vec3 mapToSphere(float u, float v)
{
	glm::vec3 ret;
	ret.x = std::sinf(u) * std::cosf(v);
	ret.y = std::cosf(u) * std::cosf(v);
	ret.z = std::sinf(v);

	return ret;
}