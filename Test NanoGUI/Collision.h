#pragma once
#include "CollisionExtra.h"
#include "Components.h"
#include "RayIntersect.h"

// Std. Includes
#include <vector>
#include <iostream>
#include <list>
#include <cmath>
#include <algorithm> 

//GLM Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define EPSILON  0.0000001
#define MODULUS(p) (sqrt(p.x*p.x + p.y*p.y + p.z*p.z))
#define TWOPI 6.283185307179586476925287
#define RTOD 57.2957795

namespace que {

	typedef struct {
		double x, y, z;
	} XYZ;

	class Collision
	{
	public:
		Collision();
		~Collision();

		bool detectCollision(std::vector<Face> objectFaces, std::vector<glm::vec3> userVertices);
		bool detectCollision(std::vector<Face> objectFaces, std::vector<glm::vec3> userVertices, glm::vec3 PositionToCastTo);
		bool detectCollision(BoundingBox a, BoundingBox b);
		bool detectCollision(BoundingBox box, glm::vec3 point);

		glm::vec3 LineA, LineB, LineC, LineD;

	private:
		//True if collision is detected, false otherwise
		bool detectCollision(Sphere a, Sphere b);
	};
}