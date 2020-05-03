#pragma once
#include "ItemEntitySystem.h"
#include "CollisionUtility.h"

void ItemEntitySystem::Init()
{
}

extern ComponentSystemManager csm;
void ItemEntitySystem::Update(Camera& camera, Inventory* inv, int key, int scancode, int action, int mods)
{
	pickedUpEntities.clear();
	for (auto const& entity : mEntities)
	{
		if (frames < 2) frames = 10;
		auto& transformC = csm.GetComponent<TransformC>(entity);
		auto& entityC = csm.GetComponent<EntityC>(entity);

		float distance = CollisionUtility::distanceBetweenTwoPoints(transformC.position.x, transformC.position.z, camera.GetPosition().x, camera.GetPosition().z);
		if(distance <= 1.f && action == GLFW_PRESS && key == GLFW_KEY_E) {
			inv->AddItem(entityC.item);
			pickedUpEntities.push_back(entity);
			
		}
	}

	for (int i = 0; i < pickedUpEntities.size(); i++) {
		csm.DestroyEntity(pickedUpEntities.at(i));
	}
}
