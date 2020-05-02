#pragma once
#include "Components.h"
#include "ComponentSystemManager.h"

class ChunkSystem : public System
{
public:
	void Init();
	void Update(Camera& camera);
};