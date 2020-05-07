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
	float movementSpeed = 2.0f;
	Camera_Movement direction;
	bool isCamera = false;
};

struct ModelMeshC {
	AnimModel model;
};

struct RenderModeC {
	bool MeshMode = false;
	bool Boundingboxes = false;
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
	vec2 goal = vec2(0);

	bool isHostile = false;
	float movementSpeed = 1.8f;
	bool firstTime = true;
	float damagePerHit = 0.5f;
	float attackSpeekd = 2.3f;
	float timeSinceLastAttack = 0.f;
};

struct NPCC {
	Quest quest;
};

struct DataC {
	bool isCamera;
};

struct PathfindingC {

};

struct XpC {
	int currentLvl = 0;
	int currentXp = 0;

	int skillPoints = 0;
};

struct HealthC {
	int health = 10;
	int maxHealth = 10;

	float timeFromLastDamage = 0;
	float timeBeforeHealing = 150.f;
};

struct SpellC {
	SpellType first = SpellType::spell_unknown;
	SpellType second = SpellType::spell_unknown;
	SpellType third = SpellType::spell_unknown;
};

struct ManaC {
	int mana = 20;
	int maxMana = 20;
};