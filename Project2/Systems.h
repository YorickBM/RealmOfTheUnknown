#pragma once
#include "InputSystem.h"
#include "MovementSystem.h"

/*
>>> XSystem.h <<<
#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"

ComponentSystemManager csm;
class XSystem : public System
{
public:
	void Init();
	void Update();
};


>>> XSystem.cpp <<<
ComponentSystemManager csm;
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