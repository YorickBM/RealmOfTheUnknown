#include "CollisionSystem.h"

extern ComponentSystemManager csm;
void CollisionSystem::Init()
{
}

void CollisionSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		auto& modelMeshC = csm.GetComponent<ModelMeshC>(entity);
		auto& transformC = csm.GetComponent<TransformC>(entity);
		auto& collisionC = csm.GetComponent<CollisionC>(entity);

		
	}
}
