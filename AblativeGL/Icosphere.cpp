#include "Icosphere.h"

Icosphere::Icosphere(int iterations)
{
	index = 0;

	float t = (float)(1.0 + std::sqrt(5.0)) / 2.0f;

	addVertex(glm::vec3(-1, t, 0));
	addVertex(glm::vec3(1, t, 0));
	addVertex(glm::vec3(-1, -t, 0));
	addVertex(glm::vec3(1, -t, 0));

	addVertex(glm::vec3(0, -1, t));
	addVertex(glm::vec3(0, 1, t));
	addVertex(glm::vec3(0, -1, -t));
	addVertex(glm::vec3(0, 1, -t));

	addVertex(glm::vec3(t, 0, -1));
	addVertex(glm::vec3(t, 0, 1));
	addVertex(glm::vec3(-t, 0, -1));
	addVertex(glm::vec3(-t, 0, 1));

	faces.push_back(Indices(0, 11, 5));
	faces.push_back(Indices(0, 5, 1));
	faces.push_back(Indices(0, 1, 7));
	faces.push_back(Indices(0, 7, 10));
	faces.push_back(Indices(0, 10, 11));

	faces.push_back(Indices(1, 5, 9));
	faces.push_back(Indices(5, 11, 4));
	faces.push_back(Indices(11, 10, 2));
	faces.push_back(Indices(10, 7, 6));
	faces.push_back(Indices(7, 1, 8));

	faces.push_back(Indices(3, 9, 4));
	faces.push_back(Indices(3, 4, 2));
	faces.push_back(Indices(3, 2, 6));
	faces.push_back(Indices(3, 6, 8));
	faces.push_back(Indices(3, 8, 9));

	faces.push_back(Indices(4, 9, 5));
	faces.push_back(Indices(2, 4, 11));
	faces.push_back(Indices(6, 2, 10));
	faces.push_back(Indices(8, 6, 7));
	faces.push_back(Indices(9, 8, 1));

	for(int i = 0; i < iterations; i++)
	{
		std::vector<Indices> faces2;
		faces2.reserve(faces.size());
		for(unsigned int i = 0; i < faces.size(); i++)
		{
			int a = getMiddlePoint(faces[i].v1, faces[i].v2);
			int b = getMiddlePoint(faces[i].v2, faces[i].v3);
			int c = getMiddlePoint(faces[i].v3, faces[i].v1);

			faces2.push_back(Indices(faces[i].v1, a, c));
			faces2.push_back(Indices(faces[i].v2, b, a));
			faces2.push_back(Indices(faces[i].v3, c, b));
			faces2.push_back(Indices(a, b, c));
		}
		faces = faces2;
	}

	for(unsigned int i = 0; i < faces.size(); i++)
	{
		indices.push_back(faces[i].v1);
		indices.push_back(faces[i].v2);
		indices.push_back(faces[i].v3);
	}
}

int Icosphere::addVertex(const glm::vec3& vertex)
{
	vertices.push_back(glm::normalize(vertex));
	return index++;
}

void Icosphere::GenerateNormals()
{
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		normals.push_back(glm::vec3(0, 0, 0));
	}

	//Normals calculation with cross product
	for(unsigned int i = 0; i < faces.size(); i++)
	{
		glm::vec3 vertex_a = vertices[faces[i].v1];
		glm::vec3 vertex_b = vertices[faces[i].v2];
		glm::vec3 vertex_c = vertices[faces[i].v3];

		glm::vec3 first_edge = vertex_b - vertex_a;
		glm::vec3 second_edge = vertex_c - vertex_a;

		glm::vec3 normal = glm::cross(first_edge, second_edge);
		if(glm::length(normal) > 0.0f)
		{
			normal = glm::normalize(normal);
		}

		normals[faces[i].v1] += normal;
		normals[faces[i].v2] += normal;
		normals[faces[i].v3] += normal;
	}

	//Normalize again due to addition of normals
	for(unsigned int i = 0; i < normals.size(); i++)
	{
		if(glm::length(normals[i]) > 0.0f)
		{
			normals[i] = glm::normalize(normals[i]);
		}
	}
}

int Icosphere::getMiddlePoint(int point1, int point2)
{
	bool is_first_smaller = point1 < point2;
	int64_t smaller_index = is_first_smaller ? point1 : point2;
	int64_t greater_index = is_first_smaller ? point2 : point1;
	//pack the smaller and greater index into a 64 bit key
	int64_t key = (smaller_index << 32) + greater_index;

	auto it = middle_point_cache.find(key);

	if(it != middle_point_cache.end())
	{
		return it->second;
	}

	glm::vec3 p1 = vertices.at(point1);
	glm::vec3 p2 = vertices.at(point2);

	glm::vec3 middle = glm::vec3((p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, (p1.z + p2.z) / 2.0f);
	int i = addVertex(middle);

	middle_point_cache.insert(std::pair<int64_t, int>(key, i));
	return i;
}

int* Icosphere::GetIndices()
{
	return &indices[0];
}

glm::vec3* Icosphere::GetVertices()
{
	return &vertices[0];
}

glm::vec3* Icosphere::GetNormals()
{
	return &normals[0];
}

int Icosphere::GetNumberOfIndices()
{
	return indices.size();
}

int Icosphere::GetNumberOfVertices()
{
	return vertices.size();
}

int Icosphere::GetNumberOfNormals()
{
	return normals.size();
}