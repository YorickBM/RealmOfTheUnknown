#pragma once
#include "ItemEntitySystem.h"
#include "CollisionUtility.h"

void ItemEntitySystem::Init()
{
}

extern ComponentSystemManager csm;
void ItemEntitySystem::Update(Camera& camera, Inventory* inv)
{
	for (auto const& entity : mEntities)
	{
		auto& transformC = csm.GetComponent<TransformC>(entity);
		auto& entityC = csm.GetComponent<EntityC>(entity);

		if (std::find(_positions.begin(), _positions.end(), vec2(transformC.position.x, transformC.position.z)) == _positions.end()) {
			_positions.push_back(vec2(transformC.position.x, transformC.position.z));
			_itemByPos.insert(make_pair(make_pair(transformC.position.x, transformC.position.z), entityC.item));
		}
	}

	std::vector<glm::vec2> inRangeItems = CollisionUtility::getClosesPointsInRange(4, _positions, camera.GetPosition(), 0.4f);
	for (glm::vec2 pos : inRangeItems) {
		Item itm = _itemByPos.at(make_pair(pos.x, pos.y));
		inv->AddItem(itm);
	}
}
