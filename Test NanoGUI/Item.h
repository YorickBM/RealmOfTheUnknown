#pragma once
#include <string>
#include <iostream>
#include <nanogui/entypo.h>

enum InventoryCataType { AllItems = 0, Armor, Tools, Miscellaneous };
enum ArmorType { NoArmor, Head, Chest, Legs, Boots };
enum ItemType { game_unknown, game_worn_boots, game_dummy_hammer, game_bone, game_mushroom };
enum ItemTypeE { game_item = 0, game_armor = 1, game_weapon = 2 };

enum QuestCataType { AllQuests = 0, Completed, Active, Open, Closed };
enum QuestType { quest_unknown = 0, quest_forest, quest_repair_boat, quest_protect_camp };
enum QuestTaskType { ItemTask, KillTask, CurrencyTask };

enum SpellType { spell_unknown, spell_hunter_charge, spell_hunter_bash };

class Quest {
public:
	Quest() {}
	Quest(std::string name, std::vector<std::string> desc, QuestCataType sortType, QuestType type, int reqLvl,
		std::vector<std::string> start, std::vector<std::string> working, std::vector<std::string> completed, std::vector<std::string> rewards, std::vector<std::string> taskReq) :
		name(name), desc(desc), sortType(sortType), type(type), reqLevel(reqLvl), 
		start(start), working(working), completed(completed), taskReward(rewards), taskReq(taskReq) {}
	~Quest() {}

	std::string name = "Unknown Quest";
	std::vector<std::string> desc = {};
	std::string image = "empty";

	int reqLevel = 1;

	QuestCataType sortType = QuestCataType::Closed;
	QuestType type = QuestType::quest_unknown;
	QuestTaskType taskType = QuestTaskType::ItemTask;
	std::vector<std::string> taskReq = { "1xgame_bone" };
	bool takeOnComplete = true;
	std::vector<std::string> taskReward = {"XP:16"};

	std::string stat1 = "Min. Level: --1";
	int icon1 = ENTYPO_ICON_LAB_FLASK;
	std::string stat2 = "Req. Quest: ---";
	int icon2 = ENTYPO_ICON_NEWSLETTER;

	int imgLocation = 1;

	std::vector<std::string> start;
	std::vector<std::string> working;
	std::vector<std::string> completed;
};

struct Item {
	std::string name = "N/A";
	std::vector<std::string> desc = {};

	std::string image = "empty";
	InventoryCataType type = InventoryCataType::Miscellaneous;
	int imgLocation = 1;

	std::string stat1 = "Min. Level: --1";
	int icon1 = ENTYPO_ICON_LAB_FLASK;
	std::string stat2 = "Req. Class: ---";
	int icon2 = ENTYPO_ICON_NEWSLETTER;
	std::string stat3 = "Req. Quest: ---";
	int icon3 = ENTYPO_ICON_CLIPBOARD;

	ItemType id = ItemType::game_unknown;
	int amount = -1;

	//Armor Extra / Tools Extra
	bool isStackable = false; //Tools/Armor = false
	ArmorType ArmorType = ArmorType::NoArmor;
	ItemTypeE itemType = ItemTypeE::game_item;
	float damagePerHit = 0.3f;
};

class EnumUtility {
public:
	static QuestType StringToQuestType(string type) {
		if (type == "quest_forest" || type == "quest-forest") return QuestType::quest_forest;
		if (type == "quest_protect_camp" || type == "quest-protect-camp") return QuestType::quest_protect_camp;
		if (type == "quest_repair_boat" || type == "quest-repair-boat") return QuestType::quest_repair_boat;
		if (type == "quest_unknown" || type == "quest-unknown") return QuestType::quest_unknown;
	}
	static ItemType StringToItemType(string type) {
		if (type == "game_bone") return ItemType::game_bone;
		if (type == "game_dummy_hammer") return ItemType::game_dummy_hammer;
		if (type == "game_unknown") return ItemType::game_unknown;
		if (type == "game_worn_boots") return ItemType::game_worn_boots;
		if (type == "game_mushroom") return ItemType::game_mushroom;
	}

	static SpellType StringToSpellType(string type) {
		if (type == "spell_unknown") return SpellType::spell_unknown;
		if (type == "spell_hunter_charge") return SpellType::spell_hunter_charge;
		if (type == "spell_hunter_bash") return SpellType::spell_hunter_bash;
	}
};