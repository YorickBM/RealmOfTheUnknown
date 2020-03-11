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
		for (auto const& entityCol : mEntities)
		{
			auto& collisionCCol = csm.GetComponent<CollisionC>(entityCol);
			if(col.detectCollision(collisionCCol.boundingBox, collisionC.boundingBox)) {
				std::cout << "Collision between Models Found" << std::endl;
				transformC.position = modelMeshC.model.GetPosition();

				if (collisionC.type == RayCastingCollision) {
					//col.detectCollision(facesof entityCol, vertices of entity);
				}
			}
		}
	}
}
