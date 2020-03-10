#pragma once
#include "InputSystem.h"
#include "MovementSystem.h"
#include "ModelMeshSystem.h"

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
extern ComponentSystemManager csm;
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