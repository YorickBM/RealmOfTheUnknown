#pragma once
#include "Components.h"
#include "ComponentSystemManager.h"
#include "Collision.h"

class DataSystem : public System
{
public:
	void Init();
	std::set<Entity> GetEntities();
};

