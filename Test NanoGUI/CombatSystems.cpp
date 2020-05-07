#include "CombatSystems.h"

extern std::vector<std::string, std::string> settings;
extern ComponentSystemManager csm;
extern MobManager mobManager;
extern AudioMaster* audioMaster;

void AttackSystem::Init() {

}
void AttackSystem::Update(Camera& camera, Inventory* inv, Entity player, float deltaTime) {
	auto& healthC = csm.GetComponent<HealthC>(player);
	for (auto const& entity : mEntities) {
		auto& transformC = csm.GetComponent<TransformC>(entity);
		auto& entityC = csm.GetComponent<EntityC>(entity);

		float attackDamage = entityC.damagePerHit;
		float distance = CollisionUtility::distanceBetweenTwoPoints(camera.GetPosition().x, camera.GetPosition().z, transformC.position.x, transformC.position.z);
		if (distance < 1.f && entityC.isHostile && entityC.timeSinceLastAttack >= entityC.attackSpeekd) {
			healthC.timeFromLastDamage = 0.f;
			healthC.health -= attackDamage;
			entityC.timeSinceLastAttack = 0;
			audioMaster->PlaySound(audioMaster->GetEffectsSoundEngine(), "resources/Sounds/Damage.mp3", false);
		}
		entityC.timeSinceLastAttack += deltaTime;
	}
	inv->SetHealth(healthC.health, healthC.maxHealth);
}
void AttackSystem::ExecuteAttack(Camera& camera, Inventory* inv, int key, int action) {
	Item item = inv->GetActiveItem();
	if (item.itemType == ItemTypeE::game_weapon && action == GLFW_PRESS && key == GLFW_MOUSE_BUTTON_1) {
		for (auto const& entity : mEntities) {
			auto& transformC = csm.GetComponent<TransformC>(entity);
			auto& healthC = csm.GetComponent<HealthC>(entity);

			float attackDamage = item.damagePerHit;
			float distance = CollisionUtility::distanceBetweenTwoPoints(camera.GetPosition().x, camera.GetPosition().z, transformC.position.x, transformC.position.z);
			if (distance < 1.f) {
				healthC.timeFromLastDamage = 0.f;
				healthC.health -= attackDamage;
				audioMaster->PlaySound(audioMaster->GetEffectsSoundEngine(), "resources/Sounds/Hit.mp3", false);
			}
		}
	}
}

void HealthSystem::Init() {

}
void HealthSystem::Update(float deltaTime, Inventory* inv, Entity player) {
	for (auto const& entity : mEntities) {
		auto& healthC = csm.GetComponent<HealthC>(entity);
		if (healthC.timeFromLastDamage >= healthC.timeBeforeHealing && healthC.health < healthC.maxHealth) {
			healthC.health += 1;
			healthC.timeFromLastDamage = 110.f; //So it doesnt insta heal
		}
		healthC.timeFromLastDamage += deltaTime;
		if (healthC.health <= 0) {
			if (entity == player) {
				//Player died
				//Respawn
				//min few Xp
				//min some currency
				//Drop all above
			}
			else {
				deadThings.insert(entity);
			}
		}
	}

	for (auto const& entity : deadThings) {
		mobManager.KillMob(entity, inv, player);
	}
	deadThings.clear();
}

void XpSystem::Init() {

}
void XpSystem::Update(Inventory* inv) {
	for (auto const& entity : mEntities) {
		auto& xpC = csm.GetComponent<XpC>(entity);
		auto& healthC = csm.GetComponent<HealthC>(entity);
		auto& spellC = csm.GetComponent<SpellC>(entity);
		
		if (xpC.currentXp >= levels.at(xpC.currentLvl + 1).first) {
			//Send Notification For levelUp
			std::cout << "You leveld Up!" << std::endl;
			xpC.currentLvl += 1;
			xpC.currentXp = 0;

			for (std::string reward : levels.at(xpC.currentLvl).second) {
				std::vector<std::string> rewardsSplited = FileLoader::Split(reward += ":0", ":");
				if (rewardsSplited[0] == "Spell") {
					if (spellC.first == SpellType::spell_unknown) spellC.first = EnumUtility::StringToSpellType(rewardsSplited[1]);
					else if (spellC.second == SpellType::spell_unknown) spellC.second = EnumUtility::StringToSpellType(rewardsSplited[1]);
					else if (spellC.third == SpellType::spell_unknown) spellC.third = EnumUtility::StringToSpellType(rewardsSplited[1]);
				} else if (rewardsSplited[0] == "Health") {
					healthC.maxHealth += std::stoi(rewardsSplited[1]);
				} else if (rewardsSplited[0] == "SkillPoints") {
					xpC.skillPoints += std::stoi(rewardsSplited[1]);
				}
			}
		}
		inv->SetXp(xpC.currentXp);
	}
}

void XpSystem::CreateLevel(int lvl, int reqXp, std::vector<std::string> rewards) {
	levels.insert(make_pair(lvl, make_pair(reqXp, rewards)));
}

std::vector<std::string> XpSystem::GetRewardInfo(int lvl) {
	return levels.at(lvl).second;
}
int XpSystem::ReqXp(int lvl) {
	return levels.at(lvl).first;
}

void SpellSystem::Init() {

}
void SpellSystem::Update(Inventory* inv) {
	for (const auto& entity : mEntities) {
		auto& manaC = csm.GetComponent<ManaC>(entity);
		inv->SetMana(manaC.mana, manaC.maxMana);
	}
}