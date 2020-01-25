#pragma once
#include "Collision.h"

struct CollisionC {
	///BoundingBox boundingBox;
	///std::vector<Face> faces;
	glm::vec3 NextPosition = glm::vec3(0,0,0);
	bool isColliding = false;
};

struct BoundingBoxC {
	///glm::vec3 MaxPos, MinPos;
	BoundingBox boundingBox;
	bool render = false;
	Model BoundingBoxModel;
	int type = -1;
	bool isColliding = false;
};