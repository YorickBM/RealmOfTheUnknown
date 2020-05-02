#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"
#include "Inventory.h"

#include <map>
#include <vector>

class ItemEntitySystem : public System
{
public:
	void Init();
	void Update(Camera& camera, Inventory* inv);

private:
	std::map<pair<float, float>, Item> _itemByPos;
	std::vector<glm::vec2> _positions;
};

