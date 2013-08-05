#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include <vector>
#include <map>
#include <math.h>
#include <stdint.h>
#include <glm\glm.hpp>

struct Indices
{
	int v1;
	int v2;
	int v3;

	Indices(int v1, int v2, int v3)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
	}
};

class Icosphere
{
public:
	Icosphere(int iterations);
	~Icosphere();

	void GenerateNormals();

	glm::vec3* GetVertices();
	glm::vec3* GetNormals();
	int* GetIndices();

	int GetNumberOfVertices();
	int GetNumberOfNormals();
	int GetNumberOfIndices();


private:
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertices;
	std::vector<int> indices;
	std::vector<Indices> faces;
	std::map<int64_t, int> middle_point_cache;

	int index;

	int getMiddlePoint(int point1, int point2);
	int addVertex(const glm::vec3& vertex);
};

#endif