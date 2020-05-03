#pragma once
#include "Components.h"
#include "ComponentSystemManager.h"
#include "Collision.h"

class ChunkSystem : public System
{
public:
	void Init();
	void Update(Camera& camera);

private:
	que::Collision col;
};