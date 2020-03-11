#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"
#include "Collision.h"

class CollisionSystem : public System
{
public:
	void Init();
	void Update();

private:
	que::Collision col;
};