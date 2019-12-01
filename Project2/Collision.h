#pragma once
#include "CollisionExtra.h"

// Std. Includes
#include <vector>
#include <iostream>
#include <list>
#include <math.h>

//GLM Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Collision
{
public:
	Collision();
	~Collision();

	void detectCollision(std::vector<glm::vec3> objectVertices, std::vector<glm::vec3> userVertices);
	//void detectCollision(Model Object, Model User);
private:
	bool polyPoly(std::vector<glm::vec3> objectModel, std::vector<glm::vec3> userModel);
	bool polyLine(std::vector<glm::vec3> vertices, float x1, float y1, float z1, float x2, float y2, float z2);
	bool lineLine(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4);

	//True if collision is detected, false otherwise
	bool detectCollision(BoundingBox a, BoundingBox b);
	bool detectCollision(Sphere a, Sphere b);
};

