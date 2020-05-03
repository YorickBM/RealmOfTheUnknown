#include "Inventory.h"

std::vector<Screen*> Inventory::getScreens() { return this->interactiveScreens; }

Inventory::Inventory(GLFWwindow* window, int width, int height, Camera& camera, Vector2i position)
	: _camera(camera), tempWidth(width), tempHeight(height) {

	Screen* backgroundScreen = NanoUtility::CreateScreen(window, nonInteractiveScreens);
	Screen* frontScreen = NanoUtility::CreateScreen(window, interactiveScreens);

	//Create info Screen
	_infoBackgroundWindow = new Window(backgroundScreen, "");
	_infoContextTopWindow = new Window(frontScreen, "");
	_infoContextBottomWindow = new Window(frontScreen, "");

	#pragma region Init Textues
		NanoUtility::LoadImage("backgroundSlot", mImagesData);
		SelectedImageIndex = NanoUtility::LoadImage("Inventory/Selected", mImagesData);
		EmptyImageIndex = NanoUtility::LoadImage("Empty", mImagesData);
	#pragma endregion

	#pragma region Info Background
		_infoBackgroundWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
		_infoBackgroundWindow->setFixedSize({ 236, 600 });

		GLCanvas* topCanvas = new GLCanvas(_infoBackgroundWindow);
		topCanvas->setBackgroundColor({ 46, 48, 52, 255 });
		topCanvas->setFixedSize({ 236, 230 });

		GLCanvas* bottomCanvas = new GLCanvas(_infoBackgroundWindow);
		bottomCanvas->setBackgroundColor({ 40, 40, 43, 255 });
		bottomCanvas->setFixedSize({ 236, 370 });
	#pragma endregion
	#pragma region Info Top Context
	_infoContextTopWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 15, 3));
	_infoContextTopWindow->setFixedSize({ 236, 230 });
	NanoUtility::InventoryTheme(_infoContextTopWindow);

	//Image View
	ImageView* prevImage = new ImageView(_infoContextTopWindow, mImagesData[0].first.texture());
	prevImage->setFixedSize({ 70,70 });
	_infoImages["PREVIEW"] = prevImage;

	//Item Title
	Label* itemTitle = new Label(_infoContextTopWindow, "Lorem ipsum", "sans-bold");
	itemTitle->setFontSize(16);
	itemTitle->setColor(Color(255, 255, 255, 255));

	_infoLabels["TITLE"] = itemTitle;
	#pragma endregion
	#pragma region Info Bottom Context
		_infoContextBottomWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 15, 3));
		_infoContextBottomWindow->setFixedSize({ 236, 370 });
		NanoUtility::InventoryTheme(_infoContextBottomWindow);

		//Item stats
		_infoButtons["1"] = NanoUtility::iconText(_infoContextBottomWindow, "Min. Level: N/A", ENTYPO_ICON_LAB_FLASK, "sand-bold", 16, { 255, 255, 255, 255 }, { 200, 20 });
		_infoLines["1"] = NanoUtility::line(_infoContextBottomWindow);
		_infoButtons["2"] = NanoUtility::iconText(_infoContextBottomWindow, "Req. Class: N/A", ENTYPO_ICON_NEWSLETTER, "sand-bold", 16, { 255, 255, 255, 255 }, { 200, 20 });
		_infoLines["2"] = NanoUtility::line(_infoContextBottomWindow);
		_infoButtons["3"] = NanoUtility::iconText(_infoContextBottomWindow, "Req. Quest: N/A", ENTYPO_ICON_CLIPBOARD, "sand-bold", 16, { 255, 255, 255, 255 }, { 200, 20 });

		//Item Desc
		NanoUtility::emptySpace(_infoContextBottomWindow, 20, { 40, 40, 43, 255 });
		NanoUtility::title(_infoContextBottomWindow, "DESCRIPTION");
		NanoUtility::line(_infoContextBottomWindow);

		desc = NanoUtility::setDesc(_infoContextBottomWindow, "N/A");

	#pragma endregion

	//Create Backpack
	_backpackBackgroundWindow = new Window(backgroundScreen, "");
	_backpacContextTopWindow = new Window(frontScreen, "");
	_backpacContextBottomWindow = new Window(frontScreen, "");

	#pragma region Backpack Background
	_backpackBackgroundWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
	_backpackBackgroundWindow->setFixedSize({ 812, 600 });

	GLCanvas* topCanvasBp = new GLCanvas(_backpackBackgroundWindow);
	topCanvasBp->setBackgroundColor({ 40, 40, 43, 255 });
	topCanvasBp->setFixedSize({ 812, 114 });

	GLCanvas* bottomCanvasBp = new GLCanvas(_backpackBackgroundWindow);
	bottomCanvasBp->setBackgroundColor({ 46, 48, 52, 255 });
	bottomCanvasBp->setFixedSize({ 812, 486 });
#pragma endregion
	#pragma region Backpack Top Content
	_backpacContextTopWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 32, 0));
	_backpacContextTopWindow->setFixedSize({ 812, 120 });

	///emptySpace(_backpacContextTopWindow, 15, { 40, 40, 43, 255 });
	NanoUtility::iconText(_backpacContextTopWindow, "INVENTORY", ENTYPO_ICON_SUITCASE, "sand-bold", 30, { 255, 255, 255, 255 }, { 200, 38 });
	NanoUtility::emptySpace(_backpacContextTopWindow, 10, { 40, 40, 43, 255 });
	Widget* tools = new Widget(_backpacContextTopWindow);
	tools->setLayout(new BoxLayout(Orientation::Horizontal,
		Alignment::Middle, 0, 6));
	Button* b = new Button(tools, "All");
	b->setCallback([&] {
		cout << "Show All" << endl;
		for (Button* btn : _filterButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterButtons.at(0)->setTextColor({ 214, 171, 63, 255 });
		SortInventory(InventoryCataType::All);
		});
	_filterButtons.push_back(b);
	b = new Button(tools, "Tools");
	b->setCallback([&] {
		cout << "Show Tools" << endl;
		for (Button* btn : _filterButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterButtons.at(1)->setTextColor({ 214, 171, 63, 255 });
		SortInventory(InventoryCataType::Tools);
		});
	_filterButtons.push_back(b);
	b = new Button(tools, "Armor");
	b->setCallback([&] {
		cout << "Show Armor" << endl;
		for (Button* btn : _filterButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterButtons.at(2)->setTextColor({ 214, 171, 63, 255 });
		SortInventory(InventoryCataType::Armor);
		});
	_filterButtons.push_back(b);
	b = new Button(tools, "Miscellaneous");
	b->setCallback([&] {
		cout << "Show Miscellaneous" << endl;
		for (Button* btn : _filterButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterButtons.at(3)->setTextColor({ 214, 171, 63, 255 });
		SortInventory(InventoryCataType::Miscellaneous);
		});
	_filterButtons.push_back(b);

	//Bootup first selection
	for (Button* btn : _filterButtons) btn->setTextColor({ 89, 91, 91, 255 });
	_filterButtons.at(0)->setTextColor({ 214, 171, 63, 255 });
#pragma endregion
	#pragma region Backpack Bottom Content
	createSlotRow(new Window(frontScreen, ""), _invRows);
	createSlotRow(new Window(frontScreen, ""), _invRows);
	createSlotRow(new Window(frontScreen, ""), _invRows);
	createSlotRow(new Window(frontScreen, ""), _invRows);
	createSlotRow(new Window(frontScreen, ""), _invRows);
#pragma endregion

	for (auto selectedImg : _selectedItems)
		selectedImg.second->bindImage(mImagesData[EmptyImageIndex].first.texture());

	///DEBUG
	std::cout << _selectedItems.size() << std::endl;

	//Create Toolbar
	_toolBarBackgroundWindow = new Window(backgroundScreen, "");
	_toolBarContextWindow = new Window(frontScreen, "");

	#pragma region ToolBar
	_toolBarBackgroundWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
	_toolBarBackgroundWindow->setFixedSize({ 812, 96 });

	GLCanvas* topCanvasTb = new GLCanvas(_toolBarBackgroundWindow);
	topCanvasTb->setBackgroundColor({ 40, 40, 43, 255 });
	topCanvasTb->setFixedSize({ 812, 96 });

	createSlotRow(_toolBarContextWindow, _tbRows);
#pragma endregion
	
	//Create Armor Info Widget
	_infoArmorBackgroundWindow = new Window(backgroundScreen, "");
	_infoArmorContextTopWindow = new Window(backgroundScreen, "");
	_infoArmorContextBottomWindow = new Window(backgroundScreen, "");

	#pragma region Armor Info Background
	_infoArmorBackgroundWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
	_infoArmorBackgroundWindow->setFixedSize({ 236, 600 });

	NanoUtility::canvas(_infoArmorBackgroundWindow, { 46, 48, 52, 255 }, { 236, 370 });
	NanoUtility::canvas(_infoArmorBackgroundWindow, { 40, 40, 43, 255 }, { 236, 230 });
#pragma endregion
	#pragma region Armor Info Top
	_infoArmorContextTopWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
	_infoArmorContextTopWindow->setFixedSize({ 236, 370 });
	NanoUtility::applyCustomTheme(_infoArmorContextTopWindow);
#pragma endregion
	#pragma region Armor Info Bottom
	_infoArmorContextBottomWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
	_infoArmorContextBottomWindow->setFixedSize({ 236, 230 });
#pragma endregion

	#pragma region Navi Buttons
	_naviButton = new Window(frontScreen, "");
	_naviButton->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, -2, -1));

	Button* Quest = NanoUtility::IconButton(_naviButton, "", ENTYPO_ICON_BOOK, [&]() {
			ShowInventory(false);
			ShowInfo(false);
			ShowArmor(false);
			ShowQuest(true);
			ShowQuestInfo(true);
			
			realignWindows(tempWidth, tempHeight);
	}, "sans", 20);
	_naviButtons.push_back(Quest);
	Button* Inventory = NanoUtility::IconButton(_naviButton, "", ENTYPO_ICON_PROGRESS_FULL, [&]() {
			ShowQuest(false);
			ShowQuestInfo(false);
			ShowInfo(false);
			ShowInventory(true);
			ShowArmor(true);
			realignWindows(tempWidth, tempHeight);
	}, "sans", 20);
	_naviButtons.push_back(Inventory);
#pragma endregion



	frontScreen->setVisible(true);
	frontScreen->performLayout();
	backgroundScreen->setVisible(true);
	backgroundScreen->performLayout();

	Hide();
}

void Inventory::realignWindows(int width, int height) {
	//Info Widget Offset (When not shown set to 0,0)
	Vector2i infoOffset = Vector2i(0, 0);
	if (!_infoBackgroundWindow->visible() && _infoArmorBackgroundWindow->visible()) infoOffset = Vector2i(_infoArmorBackgroundWindow->width() / 2 + 10, 0);
	else if (_infoBackgroundWindow->visible() && !_infoArmorBackgroundWindow->visible()) infoOffset = Vector2i(_infoBackgroundWindow->width() / 2 + 10, 0);

	//Inventory
	_backpackBackgroundWindow->center();
	_backpacContextTopWindow->setPosition(_backpackBackgroundWindow->position() + Vector2i(0, 0) - infoOffset);
	_backpacContextBottomWindow->setPosition(_backpackBackgroundWindow->position() + Vector2i(0, _backpacContextTopWindow->height()) - infoOffset);
	for (pair<int, Window*> pair : _invRows)
		pair.second->setPosition(_backpackBackgroundWindow->position() + Vector2i(0, _backpacContextTopWindow->height()) + Vector2i(0, 4 + (pair.first * 90)) - infoOffset);
	_backpackBackgroundWindow->setPosition(_backpackBackgroundWindow->position() + Vector2i(0, 0) - infoOffset);

	//Toolbar
	_toolBarBackgroundWindow->center();
	_toolBarContextWindow->center();

	_toolBarContextWindow->setPosition(Vector2i(_toolBarContextWindow->position().x(), height - _toolBarContextWindow->height() - 20));
	_toolBarBackgroundWindow->setPosition(Vector2i(_toolBarBackgroundWindow->position().x(), height - _toolBarBackgroundWindow->height() - 20));

	//Info
	_infoBackgroundWindow->center();
	_infoContextTopWindow->setPosition(_infoBackgroundWindow->position() + Vector2i(0, 0) + Vector2i(_backpackBackgroundWindow->width() / 2 + 10, 0));
	_infoContextBottomWindow->setPosition(_infoBackgroundWindow->position() + Vector2i(0, _infoContextTopWindow->height()) + Vector2i(_backpackBackgroundWindow->width() / 2 + 10, 0));
	_infoBackgroundWindow->setPosition(_infoBackgroundWindow->position() + Vector2i(0, 0) + Vector2i(_backpackBackgroundWindow->width() / 2 + 10, 0));

	//Armor Info
	_infoArmorBackgroundWindow->center();
	_infoArmorContextTopWindow->setPosition(_infoArmorBackgroundWindow->position() + Vector2i(0, 0) + Vector2i(_backpackBackgroundWindow->width() / 2 + 10, 0));
	_infoArmorContextBottomWindow->setPosition(_infoArmorBackgroundWindow->position() + Vector2i(0, _infoArmorContextTopWindow->height()) + Vector2i(_backpackBackgroundWindow->width() / 2 + 10, 0));
	_infoArmorBackgroundWindow->setPosition(_infoArmorBackgroundWindow->position() + Vector2i(0, 0) + Vector2i(_backpackBackgroundWindow->width() / 2 + 10, 0));

	//Navi Buttons
	_naviButton->center();
	_naviButton->setPosition(Vector2i(_infoArmorContextTopWindow->position().x(), _infoArmorContextTopWindow->position().y() + _infoArmorContextTopWindow->height() + _infoArmorContextBottomWindow->height() - 30) + Vector2i(_infoArmorContextTopWindow->size().x() - _naviButton->size().x(), 0));

	tempWidth = width;
	tempHeight = height;
}
void Inventory::render() {
	for (Screen* screen : nonInteractiveScreens) {
		screen->drawContents();
		screen->drawWidgets();
	}
	for (Screen* screen : interactiveScreens) {
		screen->drawContents();
		screen->drawWidgets();
	}

	frame--;
}
void Inventory::keyCallbackEvent(int key, int scancode, int action, int mods) {
	if (GLFW_KEY_I == key && GLFW_PRESS == action)
	{
		if (!_backpackBackgroundWindow->visible() && frame <= 0) {
			ShowInventory(true);
			ShowArmor(true);
			realignWindows(tempWidth, tempHeight);

			frame = 10;
		}
		else if (frame <= 0) {
			Hide();

			frame = 10;
		}
	}
	else if (GLFW_KEY_Q == key && GLFW_PRESS == action)
	{
		DropItem(_items[SelectedSlot], _camera.GetPosition());
	}
}

void Inventory::SetItem(int slot, Item item) {
	item.imgLocation = NanoUtility::LoadImage(item.image, mImagesData);
	_items[slot] = item;

	refreshItem(slot);
}
void Inventory::RemoveItem(int slot) {
	_items[slot].imgLocation = 1; //Set to image empty
	_items[slot].name = "none"; //Reset name so we now its an empty slot

	refreshItem(slot);
}
void Inventory::AddItem(Item item, int count) {
	int items = 0;
	for (auto pair : allUserItems) {
		items++;
		if (pair.id == item.id) {
			pair.amount += 1;
		}
		else {
			std::cout << "New Item" << std::endl;
			item.id = NanoUtility::randomInt();
			allUserItems.push_back(item);
		}
	}
	if (items == 0) {
		std::cout << "New Item" << std::endl;
		item.id = NanoUtility::randomInt();
		allUserItems.push_back(item);
	}

	SortInventory(_activeType);
}
void Inventory::DropItem(Item item, vec3 position) {
	for (int i = 0; i < allUserItems.size(); i++) {
		if (allUserItems[i].id == item.id) {
			allUserItems[i].amount -= 1;
		}
		else {
			allUserItems.erase(allUserItems.begin() + i);
		}
	}

	///Create CSM entity
	auto Entity = csm.CreateEntity();
	string modelPath = "resources/Inventory/Models/" + item.name + ".fbx";
	NanoUtility::replace(modelPath, " ", "");

	csm.AddComponent(Entity, ModelMeshC{ AnimModel(modelPath, position, 0.3f) });
	csm.AddComponent(Entity, TransformC{ position, 0.3f });
	csm.AddComponent(Entity, EntityC{ item });

	SortInventory(_activeType);
}
void Inventory::SwitchItems(int slot1, int slot2) {
	Item itm1 = _items[slot1];
	Item itm2 = _items[slot2];

	_items[slot1] = itm2;
	_items[slot2] = itm1;

	refreshItem(slot1);
	refreshItem(slot2);
}

void Inventory::Hide() {
	ShowInfo(false);
	ShowInventory(false);
	ShowArmor(false);
	ShowQuestInfo(false);
	ShowQuest(false);
}
void Inventory::Show() {
	ShowInfo(false);
	ShowInventory(true);
	ShowArmor(true);
	ShowQuestInfo(false);
	ShowQuest(false);
}
void Inventory::ShowWithInfo() {
	ShowInfo(true);
	ShowInventory(true);
}
void Inventory::ShowInfo(bool show) {
	_infoBackgroundWindow->setVisible(show);
	_infoContextBottomWindow->setVisible(show);
	_infoContextTopWindow->setVisible(show);
}
void Inventory::ShowArmor(bool show) {
	_infoArmorBackgroundWindow->setVisible(show);
	_infoArmorContextBottomWindow->setVisible(show);
	_infoArmorContextTopWindow->setVisible(show);
}
void Inventory::ShowQuestInfo(bool show) {
}
void Inventory::ShowQuest(bool show) {
	_naviButton->setVisible(show);
	for (Button* btn : _naviButtons) btn->setTextColor({ 255, 255, 255, 255 });
	_naviButtons.at(0)->setTextColor({ 214, 171, 63, 255 });
}
void Inventory::ShowInventory(bool show) {
	_backpackBackgroundWindow->setVisible(show);
	_backpacContextTopWindow->setVisible(show);
	_backpacContextBottomWindow->setVisible(show);

	_naviButton->setVisible(show);
	for (Button* btn : _naviButtons) btn->setTextColor({ 255, 255, 255, 255 });
	_naviButtons.at(1)->setTextColor({ 214, 171, 63, 255 });

	for (pair<int, Window*> pair : _invRows)
		pair.second->setVisible(show);
}

void Inventory::SetActiveSlot(int slot) {
	//Make all slots NOT active

	if (slot != -1) {
		//Make specificslot active
	}
}
void Inventory::createSlotRow(Window* row, vector<pair<int, Window*>>& list, int rows, vector<string> items) {
	row->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 20, 8));
	row->setFixedSize({ 812 , 96 });
	rowsVar += 1;

	for (int i = 0; i < rows; i++) {
		int pos = NanoUtility::LoadImage(items[i], mImagesData);
		_items[uniqueSlotId] = Item({ "none", "N/A", items[i], InventoryCataType::Miscellaneous, pos });
		createSlot(row, 70, 70, uniqueSlotId++);
	}

	int rowNum = _invRows.size();
	if (rowNum < 0) rowNum = 0;

	list.push_back(make_pair(rowNum, row));
}
Widget* Inventory::createSlot(Widget* window, int x, int y, int slotNum) {
	Widget* slot = new Widget(window);
	slot->setFixedSize({ x, y });
	slot->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, -x));

	ImageView* backgroundImage = new ImageView(slot, mImagesData[0].first.texture());
	backgroundImage->setFixedSize({ x, y });

	int num = _items.size() - 1;
	ImageView* frontImage = new ImageView(slot, mImagesData[_items[slotNum].imgLocation].first.texture());
	frontImage->setFixedSize({ x, y });
	_slotImages[slotNum] = frontImage;

	ImageView* selectedImage = new ImageView(slot, mImagesData[0].first.texture());
	selectedImage->setFixedSize({ x, y });
	_selectedItems.push_back(make_pair(slotNum, selectedImage));

	Button* selectItem = new Button(slot, " ");
	selectItem->setFixedSize({ x, y });
	selectItem->setCallback([slotNum, this, frontImage, selectedImage]() {
		Item itm = _items[slotNum];
		frontImage->bindImage(mImagesData[itm.imgLocation].first.texture());

		if (itm.name != "none" && _backpackBackgroundWindow->visible()) {
			//Update Info widget with correct info
			UpdateInfo(itm.name, itm.imgLocation, itm.stat1, itm.icon1, itm.stat2, itm.icon2, itm.stat3, itm.icon3);

			//Show Info Menu
			ShowArmor(false);
			ShowInfo(true);

			//Realign Windows
			realignWindows(tempWidth, tempHeight);

			//Set selected Slot int to slotNum
			SelectedSlot = slotNum;
		}
		else {
			ShowInfo(false);
			ShowArmor(true);
			realignWindows(tempWidth, tempHeight);
		}

		std::cout << "Slot ->" << slotNum << std::endl;
		});

	Label* lbl = new Label(slot, "0", "sans", 16);
	lbl->setCaption("0");
	_slotCounters.insert(std::make_pair(slotNum, lbl));
	return slot;
}

void Inventory::UpdateInfo(string title, int image, string Stats1, int stats1Icon, string Stats2, int stats2Icon, string Stats3, int stats3Icon) {
	if (Stats1 == "") _infoButtons["1"]->setVisible(false);
	_infoButtons["1"]->setCaption(Stats1);
	_infoButtons["1"]->setIcon(stats1Icon);

	///if (Stats2 != "") _infoLines["1"]->setVisible(true);
	///if (Stats2 == "") _infoLines["1"]->setVisible(false);
	///if (Stats2 == "") _infoButtons["2"]->setVisible(false);
	_infoButtons["2"]->setCaption(Stats2);
	_infoButtons["2"]->setIcon(stats2Icon);

	///if(Stats3 != "") _infoLines["2"]->setVisible(true);
	///if(Stats3 == "") _infoLines["2"]->setVisible(false);
	///if(Stats3 == "") _infoButtons["3"]->setVisible(false);
	_infoButtons["3"]->setCaption(Stats3);
	_infoButtons["3"]->setIcon(stats3Icon);

	_infoLabels["TITLE"]->setCaption(title);
	_infoImages["PREVIEW"]->bindImage(mImagesData[image].first.texture());
}
void Inventory::refreshItem(int slotNum) {
	_slotImages[slotNum]->bindImage(mImagesData[_items[slotNum].imgLocation].first.texture());
	_slotCounters[slotNum]->setCaption(std::to_string(_items[slotNum].amount));
	_slotCounters[slotNum]->setPosition(_slotImages[slotNum]->position() + Vector2i(50, 50));
}
void Inventory::SortInventory(InventoryCataType sortType) {
	int slot = 0;
	_activeType = sortType;

	switch (sortType) {
	case InventoryCataType::All:
		for (auto item : allUserItems)
			SetItem(slot++, item);
		break;
	case InventoryCataType::Armor:
		for (auto item : allUserItems)
			if (item.type == InventoryCataType::Armor)SetItem(slot++, item);
		break;
	case InventoryCataType::Miscellaneous:
		for (auto item : allUserItems)
			if (item.type == InventoryCataType::Miscellaneous)SetItem(slot++, item);
		break;
	case InventoryCataType::Tools:
		for (auto item : allUserItems)
			if (item.type == InventoryCataType::Tools)SetItem(slot++, item);
		break;
	}

	for (int i = slot; slot < 50; slot++) {
		RemoveItem(slot);
	}
}