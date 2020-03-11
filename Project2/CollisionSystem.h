#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"

class CollisionSystem : public System
{
public:
	void Init();
	void Update();
};