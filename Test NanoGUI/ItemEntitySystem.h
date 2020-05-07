#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"
#include "Inventory.h"

#include <map>
#include <functional>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>

class ItemEntitySystem : public System
{
public:
	void Init();
	void Update(Camera& camera, Inventory* inv, int key, int scancode, int action, int mods);

private:
	int frames = 10;
	int npcActive = 0;
	std::vector<Entity> pickedUpEntities;
};

