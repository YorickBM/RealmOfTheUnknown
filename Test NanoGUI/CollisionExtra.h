#pragma once
//GLM Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct BoundingBox
{
	glm::vec3 min; //Contains lowest corner of the box
	glm::vec3 max; //Contains highest corner of the box

	glm::vec3 minWithPos = glm::vec3(0, 0, 0);
	glm::vec3 maxWithPos = glm::vec3(0,0,0);
} AABB;
struct Sphere
{
	glm::vec3 position;   //Center of the sphere
	float radius;       //Radius of the sphere
};
struct Face
{
	glm::vec3 Pos0, Pos1, Pos2;   //All the 3 position that make up 1 face
};