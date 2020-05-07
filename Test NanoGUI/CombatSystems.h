#pragma once
#include "Components.h"
#include "ComponentSystemManager.h"
#include "Inventory.h" 
#include "CollisionUtility.h"
#include "Item.h"
#include "FileLoader.h"
#include "MobManager.h"
#include "AudioMaster.h"

#include <map>
#include <iostream>
#include <vector>
#include <string>

class AttackSystem : public System
{
public:
	void Init();
	void Update(Camera& camera, Inventory* inv, Entity player, float deltaTime);
	void ExecuteAttack(Camera& camera, Inventory* inv, int key, int action);
};

class HealthSystem : public System
{
public:
	void Init();
	void Update(float deltaTime, Inventory* inv, Entity player);

private:
	std::set<Entity> deadThings;
};

class XpSystem : public System
{
public:
	void Init();
	void Update(Inventory* inv);

	void CreateLevel(int lvl, int reqXp, std::vector<std::string> rewards);
	std::vector<std::string> GetRewardInfo(int lvl);
	int ReqXp(int lvl);

private:
	std::map<int, std::pair<int, std::vector<std::string>>> levels;
};

class SpellSystem : public System
{
public:
	void Init();
	void Update(Inventory* inv);
};