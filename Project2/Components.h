#pragma once
#include "AnimModel.h"
#include "Camera.h"

//Data
enum AiType { Passive, Hostile };
enum InputType { Keyboard };
struct BoundingBoxS { //Non Translated Or Translated???
	vec3 min = vec3(0);
	vec3 max = vec3(0);
};


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
	AnimModel BoundingBox;
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
	BoundingBoxS boundingBox; //BoundingBox Model of Model in ModelMeshC
};