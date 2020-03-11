#include "CollisionSystem.h"

extern ComponentSystemManager csm;
void CollisionSystem::Init()
{
}

void CollisionSystem::Update()
{
	group1.clear();
	group2.clear();
	for (auto const& entity : mEntities)
	{
		auto& collisionC = csm.GetComponent<CollisionC>(entity);
		auto& modelMeshC = csm.GetComponent<ModelMeshC>(entity);

		if (collisionC.staticCollision) {
			group2.insert(entity);
			modelMeshC.BoundingBox.GenerateMinAndMaxVerticesTranslated(collisionC.boundingBox.min, collisionC.boundingBox.max, modelMeshC.model.GetPosition());
		}
		else {
			group1.insert(entity);
			modelMeshC.BoundingBox.GenerateMinAndMaxVerticesTranslated(collisionC.boundingBox.min, collisionC.boundingBox.max, modelMeshC.model.GetPosition());
		}
	}
}

void CollisionSystem::CollisionCheck()
{
	for (auto const& entity : group1)
	{
		auto& modelMeshC = csm.GetComponent<ModelMeshC>(entity);
		auto& transformC = csm.GetComponent<TransformC>(entity);
		auto& collisionC = csm.GetComponent<CollisionC>(entity);

		modelMeshC.BoundingBox.GenerateMinAndMaxVerticesTranslated(collisionC.boundingBox.min, collisionC.boundingBox.max, transformC.position);

		for (auto const& entityCol : group2)
		{
			auto& collisionCCol = csm.GetComponent<CollisionC>(entityCol);
			//auto& modelMeshCCol = csm.GetComponent<ModelMeshC>(entityCol);
			//modelMeshCCol.BoundingBox.GenerateMinAndMaxVerticesTranslated(collisionC.boundingBox.min, collisionC.boundingBox.max);

			if (col.detectCollision(collisionCCol.boundingBox, collisionC.boundingBox)) {
				std::cout << collisionC.boundingBox.min.x << ";" << collisionC.boundingBox.min.y << ";" << collisionC.boundingBox.min.z << std::endl;
				std::cout << collisionCCol.boundingBox.min.x << ";" << collisionCCol.boundingBox.min.y << ";" << collisionCCol.boundingBox.min.z << std::endl;
				std::cout << "------------------------------"<< std::endl;
				//transformC.position = modelMeshC.model.GetPosition();

				if (collisionC.type == RayCastingCollision) {
					//col.detectCollision(facesof entityCol, vertices of entity);
				}
			}
		}
	}
}
