#pragma once
#include "ChunkSystem.h"
#include "CollisionUtility.h"

void ChunkSystem::Init()
{
}

extern ComponentSystemManager csm;
void ChunkSystem::Update(Camera &camera, std::set<Entity> collidingEntities)
{
	for (auto const& entity : mEntities)
	{
		auto& collisionC = csm.GetComponent<CollisionC>(entity);
		collisionC.boundingBox.min.y -= 3.f;
		collisionC.boundingBox.max.y += 3.f;

		if (col.detectCollision(collisionC.boundingBox, camera.GetPosition())) {
			auto& chunkC = csm.GetComponent<ChunkC>(entity);
			float yC = CollisionUtility::getNewHeight(chunkC.worldMapData, { camera.GetPosition().x, camera.GetPosition().z }, chunkC.positions);
			camera.SetPosition({ camera.GetPosition().x, (yC + 1.f), camera.GetPosition().z });
		}

		for (auto const& entityCol : collidingEntities) {
			auto& dataC = csm.GetComponent<DataC>(entityCol);
			auto& transformC = csm.GetComponent<TransformC>(entityCol);

			if (!dataC.isCamera) {
				if (col.detectCollision(collisionC.boundingBox, transformC.position)) {
					auto& chunkC = csm.GetComponent<ChunkC>(entity);
					float yC = CollisionUtility::getNewHeight(chunkC.worldMapData, { transformC.position.x,transformC.position.z }, chunkC.positions);
					transformC.position = vec3({ transformC.position.x, (yC + 1.f), transformC.position.z });
				}
			}
		}
	}
}
