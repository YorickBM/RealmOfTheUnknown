#pragma once
#include "ChunkSystem.h"
#include "CollisionUtility.h"

void ChunkSystem::Init()
{
}

extern ComponentSystemManager csm;
void ChunkSystem::Update(Camera &camera)
{
	for (auto const& entity : mEntities)
	{
		auto& collisionC = csm.GetComponent<CollisionC>(entity);
		if (col.detectCollision(collisionC.boundingBox, vec2(camera.GetPosition().x, camera.GetPosition().z))) {
			auto& chunkC = csm.GetComponent<ChunkC>(entity);
			float yC = CollisionUtility::getNewHeight(chunkC.worldMapData, { camera.GetPosition().x, camera.GetPosition().z }, chunkC.positions);
			camera.SetPosition({camera.GetPosition().x, (yC + 1.f), camera.GetPosition().z});
		}
	}
}
