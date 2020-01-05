#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"

class EntityCollisionSystem : public System
{
public:
	void Init();
	void Update(std::set<Entity> SceneEntities);

private:
	que::Collision col;
};

class NonEntityCollisionSystem : public System
{
public:
	void Init();
	void Update();
};

class BoundingBoxSystem : public System
{
public:
	void Init();
	void Update(Shader shader);
};

