#pragma once
#include "Collision.h"

struct CollisionC {
	///BoundingBox boundingBox;
	///std::vector<Face> faces;
	bool isColliding = false;
};

struct BoundingBoxC {
	///glm::vec3 MaxPos, MinPos;
	BoundingBox boundingBox;
	bool render = false;
	Model model;
};