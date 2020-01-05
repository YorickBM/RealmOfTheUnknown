#include "CollisionSystem.h"

extern ComponentSystemManager csm;

void EntityCollisionSystem::Init()
{
}

void EntityCollisionSystem::Update(std::set<Entity> SceneEntities)
{
	for (auto const& entity : mEntities)
	{
		try {
			std::cout << "--- Starting Collision Detection ---" << std::endl;
			auto model = csm.GetComponent<RenderObjectC>(entity).model;
			auto BoundingBox = csm.GetComponent<BoundingBoxC>(entity).boundingBox;
			auto CollisionComp = csm.GetComponent<CollisionC>(entity);

			for (auto const& sceneEntity : SceneEntities) {
				auto SceneModel = csm.GetComponent<RenderObjectC>(sceneEntity).model;
				auto SceneBoundingBox = csm.GetComponent<BoundingBoxC>(sceneEntity).boundingBox;

				if (col.detectCollision(BoundingBox, SceneBoundingBox))
					if (col.detectCollision(SceneModel.getModelFaces(), model.getModelVertices()))
						CollisionComp.isColliding = true;
					else {}
				else CollisionComp.isColliding = false;
			}
			std::cout << "--- Finished Collision Detection ---" << std::endl;
		}
		catch (exception ex) {
			std::cout << "Error on getting model from Collision Entity/NonEntity" << std::endl;
		}
	}
}

void NonEntityCollisionSystem::Init()
{
}

void NonEntityCollisionSystem::Update()
{
}

void BoundingBoxSystem::Init()
{
	for (auto const& entity : mEntities)
	{
		auto model = csm.GetComponent<RenderObjectC>(entity).model;
		auto boundingBox = csm.GetComponent<BoundingBoxC>(entity).boundingBox;
		Model newModel(model, boundingBox);

		csm.GetComponent<BoundingBoxC>(entity).model = newModel;

		auto oldMod = csm.GetComponent<BoundingBoxC>(entity).model;
	}
}

void BoundingBoxSystem::Update(Shader shader)
{
	for (auto const& entity : mEntities)
	{
		auto model = csm.GetComponent<BoundingBoxC>(entity).model;
		auto render = csm.GetComponent<BoundingBoxC>(entity).render;

		if(render)
			model.Draw(shader);
	}
}
