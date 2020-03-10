#pragma once
#include "AnimModel.h"
#include "Camera.h"

//Data
enum AiType { Passive, Hostile };
enum InputType { Keyboard };
struct BoundingBoxS {
	vec3 min = vec3(0);
	vec3 max = vec3(0);
};


//Components
struct TransformC {
	vec3 position;
};

struct MotionC {
	float movementSpeed = 1.0f;
	Camera_Movement direction;
};

struct ModelMeshC {
	AnimModel model;
};

struct HealthC {
	int hp = 0;
	int maxHp = 100;
};

struct AiC {
	AiType type;
};

struct InputC {
	InputType type;
};

struct CollisionC {
	BoundingBoxS boundingBox;
};