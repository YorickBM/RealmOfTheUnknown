#pragma once
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "Components.h"
#include "ToolBox.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>

class ComponentSystemManager
{
public:
	void Init()
	{
		// Create pointers to each manager
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}


	// Entity methods
	Entity CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		mEntityManager->DestroyEntity(entity);

		mComponentManager->EntityDestroyed(entity);

		mSystemManager->EntityDestroyed(entity);
	}


	// Component methods
	template<typename T>
	void RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		mComponentManager->AddComponent<T>(entity, component);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		mComponentManager->RemoveComponent<T>(entity);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	bool HasComponent(Entity entity) {
		return mComponentManager->HasComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}


	// System methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}

	void InitEntities(const char *path) {
		std::ifstream infile(path);
		std::string line = "";
		while (std::getline(infile, line)) {
			auto currentEntity = CreateEntity();
			std::vector<std::string> components = split(line, ';');

			try {
				for (std::string component : components) {
					std::vector<std::string> componentData;

					std::string componentName = "";
					if (component.find("::")) {
						componentData = split(component, '::');
						componentName = componentData.at(0);
					}
					else componentName = component;

					if ((componentName == "RenderObjectC") == 1) {
						std::vector<std::string> coords = split(componentData.at(4) + "", ',');

						std::string path = "res/models/" + componentData.at(2);
						glm::vec3 position(std::stof(coords.at(0)), std::stof(coords.at(1)), std::stof(coords.at(2)));
						float scale = std::stof(componentData.at(6));

						AddComponent(currentEntity, RenderObjectC{ Model(path.c_str(), position, scale) });
					}
					else
						if ((componentName == "NonEntityC") == 1) {
							AddComponent(currentEntity, NonEntityC{});
						}
						else
							if ((componentName == "EntityC") == 1) {
								AddComponent(currentEntity, EntityC{});
							}
							else
								if ((componentName == "BoundingBoxC") == 1) {
									AddComponent(currentEntity, BoundingBoxC{
										BoundingBox{
											GetComponent<RenderObjectC>(currentEntity).model.GetMaxVertice(),
											GetComponent<RenderObjectC>(currentEntity).model.GetMinVertice()
										},
										true, nullptr });
								}
								else
									if ((componentName == "CollisionC") == 1) {
										AddComponent(currentEntity, CollisionC{ GetComponent<RenderObjectC>(currentEntity).model.getPosition(), false});
									}
									else
										if ((componentName == "MovementC") == 1) {
											std::cout << componentData.at(componentData.size() - 1) << " < - > " << componentData.size() << std::endl;
											MovementType type = EnumUtil::StringToMovementType(componentData.at(componentData.size() - 1));

											AddComponent(currentEntity, MovementC{ type });
										}
				}
			}
			catch (exception ex) {
				std::cout << "We encountert an error..." << std::endl;
				continue;
			}
		}
	}

private:
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<SystemManager> mSystemManager;

	std::vector<std::string> split(const std::string& s, char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}
};