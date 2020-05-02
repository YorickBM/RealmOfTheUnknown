#pragma once
#pragma once
#include "AnimModel.h"
#include "Camera.h"
#include "CollisionExtra.h"

//Data
enum AiType { Passive, Hostile };
enum InputType { Keyboard };
enum CollisionType { SolidCollision = 0, RayCastingCollision = 1, NoCollision = 2, WorldCollision = 3 };


//Components
struct TransformC {
	vec3 position;
	float scale;
};

struct MotionC {
	float movementSpeed = 1.0f;
	Camera_Movement direction;
};

struct ModelMeshC {
	AnimModel model;
	///AnimModel BoundingBox;
	///vec3 min;
	///vec3 max;
};

struct RenderModeC {
	bool MeshMode = false;
	bool Boundingboxes = false;
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
	CollisionType type;
	BoundingBox boundingBox;
};

struct ChunkC {
	std::map<pair<float, float>, float> worldMapData;
	std::vector<vec2> positions;
};