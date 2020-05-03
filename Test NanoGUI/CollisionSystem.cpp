#pragma once
#include "CollisionSystem.h"

extern ComponentSystemManager csm;
void CollisionSystem::Init()
{
}

void CollisionSystem::Update(Camera &camera)
{
	for (auto const& entity : mEntities)
	{
		auto& modelMeshC = csm.GetComponent<ModelMeshC>(entity);
		auto& transformC = csm.GetComponent<TransformC>(entity);
		auto& collisionC = csm.GetComponent<CollisionC>(entity);

		if(collisionC.type != CollisionType::WorldCollision)
			if (col.detectCollision(collisionC.boundingBox, camera.GetPosition())) {
				camera.SetPosition(camera.GetPrevPosition());
			}
	}
}
