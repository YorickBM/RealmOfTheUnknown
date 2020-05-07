#pragma once
#include "ItemEntitySystem.h"
#include "CollisionUtility.h"
#include "FileLoader.h"

void ItemEntitySystem::Init()
{
}

extern std::unordered_map<string, Item> invItems;
extern std::unordered_map<string, string> settings;
extern ComponentSystemManager csm;
void ItemEntitySystem::Update(Camera& camera, Inventory* inv, int key, int scancode, int action, int mods)
{
	pickedUpEntities.clear();
	if (npcActive > 0) npcActive--;
	else if (npcActive <= 0) {
		inv->SetNpcText("", "", "", false);
	}
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
				bool QuestCompleted = true;
				switch (npcC.quest.taskType) {
				case QuestTaskType::ItemTask:
					QuestCompleted = true;
					for (int i = 0; i < npcC.quest.taskReq.size(); i++) {
						std::vector<string> data = FileLoader::Split(npcC.quest.taskReq[i] + "x0", "x");
						int amountNeeded = std::stoi(data[0]);
						ItemType itm = EnumUtility::StringToItemType(data[1]);
						
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

							npcActive = 500;
							inv->SetNpcText(npcC.quest.completed[0], npcC.quest.completed[1], npcC.quest.completed[2]);

							for (std::string reward : npcC.quest.taskReward) {
								std::vector<std::string> rewards = FileLoader::Split(reward += ":x", ":");
								if (rewards[0] == "XP") {
									inv->AddXp(std::stoi(rewards[1]));
								} else if (rewards[0] == "Currency") {
									inv->AddCurrency(std::stoi(rewards[1]));
								}
								else if (rewards[0] == "Item") {
									inv->AddItem(invItems.at(rewards[1]));
								}
							}
						}
						else {
							npcActive = 500;
							inv->SetNpcText(npcC.quest.working[0], npcC.quest.working[1], npcC.quest.working[2]);
						}
					}
					break;
				case QuestTaskType::CurrencyTask:
					QuestCompleted = true;
					for (int i = 0; i < npcC.quest.taskReq.size(); i++) {
						std::vector<string> data = FileLoader::Split(npcC.quest.taskReq[i] + "x0", "x");
						int amountNeeded = std::stoi(data[0]);

						if (std::stoi(inv->GetCurrency()) < amountNeeded) QuestCompleted = false;

						if (QuestCompleted) {
							npcC.quest.sortType = QuestCataType::Completed;

							inv->AddCurrency(-amountNeeded);

							npcActive = 500;
							inv->SetNpcText(npcC.quest.completed[0], npcC.quest.completed[1], npcC.quest.completed[2]);

							for (std::string reward : npcC.quest.taskReward) {
								std::vector<std::string> rewards = FileLoader::Split(reward += ":x", ":");
								if (rewards[0] == "XP") {
									inv->AddXp(std::stoi(rewards[1]));
								}
								else if (rewards[0] == "Currency") {
									inv->AddCurrency(std::stoi(rewards[1]));
								}
								else if (rewards[0] == "Item") {
									inv->AddItem(invItems.at(rewards[1]));
								}
							}
						}
						else {
							npcActive = 500;
							inv->SetNpcText(npcC.quest.working[0], npcC.quest.working[1], npcC.quest.working[2]);
						}
					}
					break;
				}
			} else if (npcC.quest.sortType == QuestCataType::Open) {
				//Hasn't started yet
				npcC.quest.sortType = QuestCataType::Active;

				npcActive = 500;
				inv->SetNpcText(npcC.quest.start[0], npcC.quest.start[1], npcC.quest.start[2]);
			} else if (npcC.quest.sortType == QuestCataType::Closed) {
				//Not avaible yet
			} else if (npcC.quest.sortType == QuestCataType::Completed) {
				//Already Done Quest
			}
		}
	}

	for (int i = 0; i < pickedUpEntities.size(); i++) {
		std::cout << "Destorying an entity" << std::endl;
		csm.DestroyEntity(pickedUpEntities.at(i));
	}
}
