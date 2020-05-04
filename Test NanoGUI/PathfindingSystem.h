#pragma once
#include "Components.h"
#include "ComponentSystemManager.h"

class PathfindingSystem : public System
{
public:
	void Init();
	void Update(Camera& camera);
	void MoveToGoal(float deltatime);
};