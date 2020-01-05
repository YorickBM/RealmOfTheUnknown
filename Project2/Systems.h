#pragma once
#include "RenderSystem.h"
#include "MovementSystem.h"
#include "CollisionSystem.h"

/*
>>> XSystem.h <<<
#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"

class XSystem : public System
{
public:
	void Init();
	void Update();
};


>>> XSystem.cpp <<<
void XSystem::Init()
{
}

void XSystem::Update()
{
	for (auto const& entity : mEntities)
	{
	}
}

*/