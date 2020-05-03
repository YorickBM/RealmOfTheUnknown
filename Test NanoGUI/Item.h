#pragma once
#include <string>
#include <iostream>
#include <nanogui/entypo.h>

enum InventoryCataType { All = 0, Armor, Tools, Miscellaneous };

struct Item {
	std::string name = "N/A";
	std::string desc = "N/A";

	std::string image = "empty";
	InventoryCataType type = InventoryCataType::Miscellaneous;
	int imgLocation = 1;

	std::string stat1 = "Min. Level: --1";
	int icon1 = ENTYPO_ICON_LAB_FLASK;
	std::string stat2 = "Req. Class: ---";
	int icon2 = ENTYPO_ICON_NEWSLETTER;
	std::string stat3 = "Req. Quest: ---";
	int icon3 = ENTYPO_ICON_CLIPBOARD;

	int id = -1;
	int amount = 1;
	bool isStackable = true;
	int catagorie = -1;

	bool operator==(const Item& o) const {
		return name == o.name && desc == o.desc && image == o.image && type == o.type && imgLocation == o.imgLocation
			&& stat1 == o.stat1 && icon1 == o.icon1 && stat2 == o.stat2 && icon2 == o.icon2 && stat3 == o.stat3 && icon3 == o.icon3 && catagorie == o.catagorie;
	}

	bool operator<(const Item& o)  const {
		return name == o.name && desc == o.desc && image == o.image && type == o.type && imgLocation == o.imgLocation
			&& stat1 == o.stat1 && icon1 == o.icon1 && stat2 == o.stat2 && icon2 == o.icon2 && stat3 == o.stat3 && icon3 == o.icon3 && catagorie == o.catagorie;
	}
};