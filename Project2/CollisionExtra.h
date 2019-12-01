#pragma once
//GLM Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct BoundingBox
{
	glm::vec3 min; //Contains lowest corner of the box
	glm::vec3 max; //Contains highest corner of the box
} AABB;
struct Sphere
{
	glm::vec3 position;   //Center of the sphere
	float radius;       //Radius of the sphere
};