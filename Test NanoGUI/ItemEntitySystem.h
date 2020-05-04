#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"

#include "Inventory.h"

#include <map>
#include <functional>
#include <vector>

class ItemEntitySystem : public System
{
public:
	void Init();
	void Update(Camera& camera, Inventory* inv, int key, int scancode, int action, int mods);
	void loopUpdate(Camera& camera);

private:
	int frames = 10;
	std::vector<Entity> pickedUpEntities;
};

