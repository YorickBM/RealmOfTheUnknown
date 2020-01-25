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
			///std::cout << "--- Starting Collision Detection ---" << std::endl;
			auto& model = csm.GetComponent<RenderObjectC>(entity).model;
			auto& boundingBoxC = csm.GetComponent<BoundingBoxC>(entity);
			auto& CollisionComp = csm.GetComponent<CollisionC>(entity);

			for (auto const& sceneEntity : SceneEntities) {
				auto& SceneModel = csm.GetComponent<RenderObjectC>(sceneEntity).model;
				auto& sceneBoundingBoxC = csm.GetComponent<BoundingBoxC>(sceneEntity);

				if (col.detectCollision(boundingBoxC.boundingBox, sceneBoundingBoxC.boundingBox)) {
					std::cout << "Bounding Box Collision" << std::endl;
					sceneBoundingBoxC.isColliding = true;
					if (col.detectCollision(SceneModel.getModelFaces(), model.getModelVertices())) {
						std::cout << "Ray Casting Collision" << std::endl;
					} else { model.setPosition(CollisionComp.NextPosition); }
				} else { sceneBoundingBoxC.isColliding = false; model.setPosition(CollisionComp.NextPosition); }
			}
			///std::cout << "--- Finished Collision Detection ---" << std::endl;
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

		std::cout << glm::to_string(newModel.getPosition()) << std::endl;

		csm.GetComponent<BoundingBoxC>(entity).BoundingBoxModel = newModel;
	}
}

void BoundingBoxSystem::Update(Shader shader)
{
	for (auto const& entity : mEntities)
	{
		auto& MainModel = csm.GetComponent<RenderObjectC>(entity).model;
		auto& boundingBoxC = csm.GetComponent<BoundingBoxC>(entity);
		auto& render = csm.GetComponent<BoundingBoxC>(entity).render;
		auto& CollisionComp = csm.GetComponent<CollisionC>(entity);

		boundingBoxC.BoundingBoxModel.setPosition(MainModel.getPosition());
		boundingBoxC.boundingBox.min = boundingBoxC.BoundingBoxModel.GetMinVerticeWithPos();
		boundingBoxC.boundingBox.max = boundingBoxC.BoundingBoxModel.GetMaxVerticeWithPos();

		
		if (!boundingBoxC.isColliding)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (render)
			boundingBoxC.BoundingBoxModel.Draw(shader);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
