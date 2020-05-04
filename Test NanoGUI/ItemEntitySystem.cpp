#pragma once
#include "ItemEntitySystem.h"
#include "CollisionUtility.h"
#include "FileLoader.h"

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
		if(distance <= 1.f && !entityC.isNpc && action == GLFW_PRESS && key == GLFW_KEY_E) {
			inv->AddItem(entityC.item);
			pickedUpEntities.push_back(entity);
			
		}

		distance = CollisionUtility::distanceBetweenTwoPoints(transformC.position.x, transformC.position.z, camera.GetPosition().x, camera.GetPosition().z);
		if (distance <= 1.f && entityC.isNpc && action == GLFW_PRESS && key == GLFW_KEY_E) {
			auto& npcC = csm.GetComponent<NPCC>(entity);
			if (npcC.quest.sortType == QuestCataType::Active) {
				//Currently doing quest

				std::vector<string> data = FileLoader::Split(npcC.quest.taskReq + "x0", "x");
				switch (npcC.quest.taskType) {
				case QuestTaskType::ItemTask:
					bool QuestCompleted = true;
					for (int i = 0; i < npcC.quest.amountReq; i++) {
						int amountNeeded = std::stoi(data[i++]);
						ItemType itm = EnumUtility::StringToItemType(data[i]);
						
						for (auto invItm : inv->GetInvItems()) {
							if (invItm.type != itm && invItm.amount < amountNeeded) {
								QuestCompleted = false;
							}
						}

						if (QuestCompleted) {
							npcC.quest.sortType = QuestCataType::Completed;

							for (auto invItm : inv->GetInvItems()) {
								if (invItm.type == itm) {
									for (int i = 0; i < amountNeeded; i++) {
										inv->RemoveItem(invItm);
									}
								}
							}

							for (std::string str : npcC.quest.completed) {
								std::cout << str << std::endl;
							}
						}
						else {
							for (std::string str : npcC.quest.working) {
								std::cout << str << std::endl;
							}
						}
					}
					break;
				}
			} else if (npcC.quest.sortType == QuestCataType::Open) {
				//Hasn't started yet
				npcC.quest.sortType = QuestCataType::Active;

				for (std::string str : npcC.quest.start) {
					std::cout << str << std::endl;
				}
			} else if (npcC.quest.sortType == QuestCataType::Closed) {
				//Not avaible yet
			} else if (npcC.quest.sortType == QuestCataType::Completed) {
				//Already Done Quest
			}
		}
	}

	for (int i = 0; i < pickedUpEntities.size(); i++) {
		csm.DestroyEntity(pickedUpEntities.at(i));
	}
}

void ItemEntitySystem::loopUpdate(Camera& camera) {
	for (auto const& entity : mEntities)
	{
		auto& transformC = csm.GetComponent<TransformC>(entity);
		auto& entityC = csm.GetComponent<EntityC>(entity);

		
	}
}
