#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"
#include "Camera.h"

class EntityCollisionSystem : public System
{
public:
	void Init();
	void Update(std::set<Entity> SceneEntities, glm::vec3 nextPosition);
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
	void Update(ShaderLoader* shader);
};

