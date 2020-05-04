#pragma once
#include "AnimModel.h"
#include "Camera.h"
#include "CollisionExtra.h"
#include "Item.h"

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
	float movementSpeed = 1.4f;
	Camera_Movement direction;
};

struct ModelMeshC {
	AnimModel model;
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

struct EntityC {
	Item item;

	bool isNpc = false;
};

struct NPCC {
	Quest quest;
};