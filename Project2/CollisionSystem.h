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
	void CollisionCheck();

private:
	que::Collision col;
	std::set<Entity> group1;
	std::set<Entity> group2;

};