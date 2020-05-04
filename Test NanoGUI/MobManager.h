#pragma once
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STD Includes
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "AnimModel.h"
#include "ShaderLoader.h"
#include "ComponentSystemManager.h"

enum MobType { mob_spider, mob_skeleton };
extern ComponentSystemManager csm;

class Mob {
public:
	Mob(AnimModel mdl, MobType typ, float scale = 1.f, int health = 10) : model(mdl), type(typ), modelScale(scale), maxHealth(health) {}

	AnimModel model;
	MobType type;

	float modelScale = 1.f;
	int maxHealth = 10;

	int timeAlive = 300;
	Entity entity;
};

class MobManager {
public:
	MobManager() {}
	~MobManager() {}

	/*
	Spawn a mob
	*/
	void SpawnMob(MobType type, vec3 position, bool isHostile = false, float speed = 1.f, int despawntime = 300) {
		Mob mobClass = mobByType.at(type);
		mobClass.timeAlive = despawntime; // 5 min despawn time = default

		auto entity = csm.CreateEntity();
		mobClass.entity = entity;
		csm.AddComponent(entity, TransformC{ position, mobClass.modelScale });
		csm.AddComponent(entity, DataC{ false, TransformC{ position, mobClass.modelScale } });
		csm.AddComponent(entity, EntityC{ Item{}, false, position, isHostile, speed });
		csm.AddComponent(entity, HealthC{ mobClass.maxHealth, mobClass.maxHealth });
		csm.AddComponent(entity, PathfindingC{});
		mobClass.model.SetScale(mobClass.modelScale);
		mobClass.model.SetPosition(position);

		csm.AddComponent(entity, ModelMeshC{ mobClass.model});
		mobs.push_back(mobClass);
	}

	/*
	Remove a mob from our mobs list
	*/
	void RemoveMob(int index) {
		csm.DestroyEntity(mobs.at(index).entity);
		mobs.erase(mobs.begin() + index);
	}

	/*
	Trigger every miliseconds (will remove movs when despawntime is over)
	*/
	void Despawn() {
		for (int i = 0; i < mobs.size(); i++) {
			if (mobs.at(i).timeAlive < 1) RemoveMob(i);
			else mobs.at(i).timeAlive--;
		}
	}

	/*
	Create Mob Data
	*/
	void CreateMob(Mob mob, MobType type) {
		mobByType.insert(make_pair(type, mob));
	}

private:
	std::vector<Mob> mobs;
	std::map<MobType, Mob> mobByType;
};