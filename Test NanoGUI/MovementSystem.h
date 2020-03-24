#pragma once
#include "Components.h"
#include "ComponentSystemManager.h"

class MovementSystem : public System
{
public:
	void Init();
	void Update(float deltatime, Camera& camera);
};