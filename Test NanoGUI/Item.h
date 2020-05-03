#pragma once
#include <string>
#include <iostream>
#include <nanogui/entypo.h>

enum InventoryCataType { AllItems = 0, Armor, Tools, Miscellaneous };
enum ArmorType { NoArmor, Head, Chest, Legs, Boots };
enum ItemType { game_unknown, game_worn_boots, game_dummy_hammer, game_bone };

enum QuestCataType { AllQuests = 0, Completed, Active, Open, Closed };
enum QuestType { quest_unknown = 0, quest_forest, quest_repair_boat, quest_protect_camp };

class Quest {
public:
	Quest() {}
	Quest(std::string name, std::vector<std::string> desc, QuestCataType sortType, QuestType type, int reqLvl) : name(name), desc(desc), sortType(sortType), type(type), reqLevel(reqLvl) {}
	~Quest() {}

	std::string name = "Unknown Quest";
	std::vector<std::string> desc = {};
	std::string image = "empty";

	int reqLevel = 1;

	QuestCataType sortType = QuestCataType::Closed;
	QuestType type = QuestType::quest_unknown;

	std::string stat1 = "Min. Level: --1";
	int icon1 = ENTYPO_ICON_LAB_FLASK;
	std::string stat2 = "Req. Quest: ---";
	int icon2 = ENTYPO_ICON_NEWSLETTER;

	int imgLocation = 1;
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
};