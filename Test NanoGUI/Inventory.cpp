#include "Inventory.h"
#include <sstream>
#include <iomanip>

std::vector<Screen*> Inventory::getScreens() { return this->interactiveScreens; }

Inventory::Inventory(GLFWwindow* window, int width, int height, Camera& camera, Vector2i position)
	: _camera(camera), tempWidth(width), tempHeight(height) {

	#pragma region Inventory
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

		_desc.push_back(NanoUtility::createTextLine(_infoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
		_desc.push_back(NanoUtility::createTextLine(_infoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
		_desc.push_back(NanoUtility::createTextLine(_infoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
		_desc.push_back(NanoUtility::createTextLine(_infoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
		_desc.push_back(NanoUtility::createTextLine(_infoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
		_desc.push_back(NanoUtility::createTextLine(_infoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
		_desc.push_back(NanoUtility::createTextLine(_infoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
		_desc.push_back(NanoUtility::createTextLine(_infoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));

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
		SortInventory(InventoryCataType::AllItems, _items, _slotImages, _slotCounters);
		});
	_filterButtons.push_back(b);
	b = new Button(tools, "Tools");
	b->setCallback([&] {
		cout << "Show Tools" << endl;
		for (Button* btn : _filterButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterButtons.at(1)->setTextColor({ 214, 171, 63, 255 });
		SortInventory(InventoryCataType::Tools, _items, _slotImages, _slotCounters);
		});
	_filterButtons.push_back(b);
	b = new Button(tools, "Armor");
	b->setCallback([&] {
		cout << "Show Armor" << endl;
		for (Button* btn : _filterButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterButtons.at(2)->setTextColor({ 214, 171, 63, 255 });
		SortInventory(InventoryCataType::Armor, _items, _slotImages, _slotCounters);
		});
	_filterButtons.push_back(b);
	b = new Button(tools, "Miscellaneous");
	b->setCallback([&] {
		cout << "Show Miscellaneous" << endl;
		for (Button* btn : _filterButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterButtons.at(3)->setTextColor({ 214, 171, 63, 255 });
		SortInventory(InventoryCataType::Miscellaneous, _items, _slotImages, _slotCounters);
		});
	_filterButtons.push_back(b);

	//Bootup first selection
	for (Button* btn : _filterButtons) btn->setTextColor({ 89, 91, 91, 255 });
	_filterButtons.at(0)->setTextColor({ 214, 171, 63, 255 });
#pragma endregion
	#pragma region Backpack Bottom Content
	rowsVar = 0;
	createSlotRow(new Window(frontScreen, ""), _invRows);
	createSlotRow(new Window(frontScreen, ""), _invRows);
	createSlotRow(new Window(frontScreen, ""), _invRows);
	createSlotRow(new Window(frontScreen, ""), _invRows);
	createSlotRow(new Window(frontScreen, ""), _invRows);
#pragma endregion

	for (auto selectedImg : _selectedItems)
		selectedImg.second->bindImage(mImagesData[EmptyImageIndex].first.texture());

	///DEBUG
	///std::cout << _selectedItems.size() << std::endl;

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
	_infoArmorContextTopWindow = new Window(frontScreen, "");
	_infoArmorContextBottomWindow = new Window(frontScreen, "");

	_infoArmorConfirmWindow = new Window(frontScreen, "");

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

	_head = NanoUtility::comboBox(_infoArmorContextTopWindow, itemsHead);
	_shirts = NanoUtility::comboBox(_infoArmorContextTopWindow, itemsChest);
	_trousers = NanoUtility::comboBox(_infoArmorContextTopWindow, itemsLegs);
	_boots = NanoUtility::comboBox(_infoArmorContextTopWindow, itemsBoots);
	_head->setCallback([&](int x) { UpdateArmorInfo(_head->items().at(x)); });
	_shirts->setCallback([&](int x) { UpdateArmorInfo(_shirts->items().at(x)); });
	_trousers->setCallback([&](int x) { UpdateArmorInfo(_trousers->items().at(x)); });
	_boots->setCallback([&](int x) { UpdateArmorInfo(_boots->items().at(x)); });

	_infoArmorConfirmWindow = new Window(frontScreen, "");
	_infoArmorConfirmWindow->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, -6, -1));

	Button* Confirm = NanoUtility::IconButton(_infoArmorConfirmWindow, "", ENTYPO_ICON_CHECK, [&]() {
		realignWindows(tempWidth, tempHeight);
	}, "sans", 20);
	Confirm->setTextColor({ 255, 255, 255, 255});

	Button* Cancel = NanoUtility::IconButton(_infoArmorConfirmWindow, "", ENTYPO_ICON_CROSS, [&]() {
		realignWindows(tempWidth, tempHeight);
	}, "sans", 20);
	Cancel->setTextColor({ 255, 255, 255, 255 });

	#pragma endregion
	#pragma region Armor Info Bottom
	_infoArmorContextBottomWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
	_infoArmorContextBottomWindow->setFixedSize({ 236, 230 });
	#pragma endregion
	#pragma endregion

	#pragma region Quest UI
	//Quest Info
	_questInfoBackgroundWindow = new Window(backgroundScreen, "");
	_questInfoContextTopWindow = new Window(frontScreen, "");
	_questInfoContextBottomWindow = new Window(frontScreen, "");

	//Quest 
	_questBackgroundWindow = new Window(backgroundScreen, "");
	_questContextTopWindow = new Window(frontScreen, "");
	_questContextBottomWindow = new Window(frontScreen, "");

	#pragma region Quest Info Background
	_questInfoBackgroundWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
	_questInfoBackgroundWindow->setFixedSize({ 236, 600 });

	NanoUtility::canvas(_questInfoBackgroundWindow, { 46, 48, 52, 255 }, { 236, 370 });
	NanoUtility::canvas(_questInfoBackgroundWindow, { 40, 40, 43, 255 }, { 236, 230 });
	#pragma endregion
	#pragma region Quest Info Top Context
	_questInfoContextTopWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 15, 3));
	_questInfoContextTopWindow->setFixedSize({ 236, 230 });
	NanoUtility::InventoryTheme(_questInfoContextTopWindow);

	//Image View
	ImageView* prevImageQ = new ImageView(_questInfoContextTopWindow, mImagesData[NanoUtility::LoadImage("Quests/Thumbnail", mImagesData)].first.texture());
	prevImageQ->setFixedSize({ 210, 210 });
	_infoQuestImages["THUMBNAIL"] = prevImageQ;

	//Item Title
	Label* itemTitleQ = new Label(_questInfoContextTopWindow, "QUEST TITLE NEEDS TO BE HERE", "sans-bold", 20);
	itemTitle->setColor(Color(255, 255, 255, 255));

	_infoQuestLabels["TITLE"] = itemTitleQ;
	#pragma endregion
	#pragma region Quest Info Bottom Context
	_questInfoContextBottomWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 15, 3));
	_questInfoContextBottomWindow->setFixedSize({ 236, 370 });
	NanoUtility::InventoryTheme(_questContextBottomWindow);

	//Item stats
	_infoQuestButtons["1"] = NanoUtility::iconText(_questInfoContextBottomWindow, "Min. Level: N/A", ENTYPO_ICON_LAB_FLASK, "sand-bold", 16, { 255, 255, 255, 255 }, { 200, 20 });
	_infoQuestLines["1"] = NanoUtility::line(_questInfoContextBottomWindow);
	_infoQuestButtons["2"] = NanoUtility::iconText(_questInfoContextBottomWindow, "Req. Class: N/A", ENTYPO_ICON_NEWSLETTER, "sand-bold", 16, { 255, 255, 255, 255 }, { 200, 20 });
	_infoQuestLines["2"] = NanoUtility::line(_questInfoContextBottomWindow);
	_infoQuestButtons["3"] = NanoUtility::iconText(_questInfoContextBottomWindow, "Req. Quest: N/A", ENTYPO_ICON_CLIPBOARD, "sand-bold", 16, { 255, 255, 255, 255 }, { 200, 20 });

	//Item Desc
	NanoUtility::emptySpace(_questInfoContextBottomWindow, 20, { 40, 40, 43, 255 });
	NanoUtility::title(_questInfoContextBottomWindow, "DESCRIPTION");
	NanoUtility::line(_questInfoContextBottomWindow);

	_questDesc.push_back(NanoUtility::createTextLine(_questInfoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
	_questDesc.push_back(NanoUtility::createTextLine(_questInfoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
	_questDesc.push_back(NanoUtility::createTextLine(_questInfoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
	_questDesc.push_back(NanoUtility::createTextLine(_questInfoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
	_questDesc.push_back(NanoUtility::createTextLine(_questInfoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
	_questDesc.push_back(NanoUtility::createTextLine(_questInfoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
	_questDesc.push_back(NanoUtility::createTextLine(_questInfoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
	_questDesc.push_back(NanoUtility::createTextLine(_questInfoContextBottomWindow, "ABCDFDREHG3IEKDH44DKEIDJEO$GDKJDH4GD"));
	#pragma endregion

	#pragma region Quest Background
	_questBackgroundWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
	_questBackgroundWindow->setFixedSize({ 812, 600 });

	NanoUtility::canvas(_questBackgroundWindow, { 40, 40, 43, 255 }, { 812, 114 });
	NanoUtility::canvas(_questBackgroundWindow, { 46, 48, 52, 255 }, { 812, 486 });
	#pragma endregion
	#pragma region Quest Top Context
	_questContextTopWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 32, 0));
	_questContextTopWindow->setFixedSize({ 812, 120 });

	///emptySpace(_backpacContextTopWindow, 15, { 40, 40, 43, 255 });
	NanoUtility::iconText(_questContextTopWindow, "QUESTS", ENTYPO_ICON_BOOK, "sand-bold", 30, { 255, 255, 255, 255 }, { 200, 38 });
	NanoUtility::emptySpace(_questContextTopWindow, 10, { 40, 40, 43, 255 });
	Widget* questSort = new Widget(_questContextTopWindow);
	questSort->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 6));

	_filterQuestButtons.push_back(NanoUtility::button(questSort, "To do", {}, [&] {
		for (Button* btn : _filterQuestButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterQuestButtons.at(0)->setTextColor({ 214, 171, 63, 255 });
		SortQuests(QuestCataType::Open, _quests, _questImages, _questCounters);
		}));
	_filterQuestButtons.push_back(NanoUtility::button(questSort, "Active", {}, [&] {
		for (Button* btn : _filterQuestButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterQuestButtons.at(1)->setTextColor({ 214, 171, 63, 255 });
		SortQuests(QuestCataType::Active, _quests, _questImages, _questCounters);
		}));
	_filterQuestButtons.push_back(NanoUtility::button(questSort, "Completed", {}, [&] {
		for (Button* btn : _filterQuestButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterQuestButtons.at(2)->setTextColor({ 214, 171, 63, 255 });
		SortQuests(QuestCataType::Completed, _quests, _questImages, _questCounters);
		}));
	_filterQuestButtons.push_back(NanoUtility::button(questSort, "Locked", {}, [&] {
		for (Button* btn : _filterQuestButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterQuestButtons.at(3)->setTextColor({ 214, 171, 63, 255 });
		SortQuests(QuestCataType::Closed, _quests, _questImages, _questCounters);
		}));
	_filterQuestButtons.push_back(NanoUtility::button(questSort, "All", {}, [&] {
		for (Button* btn : _filterQuestButtons) btn->setTextColor({ 89, 91, 91, 255 });
		_filterQuestButtons.at(4)->setTextColor({ 214, 171, 63, 255 });
		SortQuests(QuestCataType::AllQuests, _quests, _questImages, _questCounters);
		}));

	//Bootup first selection
	for (Button* btn : _filterQuestButtons) btn->setTextColor({ 89, 91, 91, 255 });
	_filterQuestButtons.at(0)->setTextColor({ 214, 171, 63, 255 });

	#pragma endregion
	#pragma region Quest Bottom Context
	createSlotRow(new Window(frontScreen, ""), _questRows, true);
	createSlotRow(new Window(frontScreen, ""), _questRows, true);
	createSlotRow(new Window(frontScreen, ""), _questRows, true);
	createSlotRow(new Window(frontScreen, ""), _questRows, true);
	createSlotRow(new Window(frontScreen, ""), _questRows, true);
	#pragma endregion
	#pragma endregion

	#pragma region Navi Buttons
	_naviButton = new Window(frontScreen, "");
	_naviButton->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, -6));

	Button* Quest = NanoUtility::IconButton(_naviButton, "", ENTYPO_ICON_BOOK, [&]() {
			ShowInventory(false);
			ShowInfo(false);
			ShowArmor(false);
			ShowQuest(true);
			ShowQuestInfo(true);
			
			realignWindows(tempWidth, tempHeight);
	}, "sans", 20);
	_naviButtons.push_back(Quest);
	Button* Inventory = NanoUtility::IconButton(_naviButton, "", ENTYPO_ICON_SUITCASE, [&]() {
			ShowQuest(false);
			ShowQuestInfo(false);
			ShowInfo(false);
			ShowInventory(true);
			ShowArmor(true);
			realignWindows(tempWidth, tempHeight);
	}, "sans", 20);
	_naviButtons.push_back(Inventory);
#pragma endregion

	#pragma region Themes
	NanoUtility::InventoryTheme(_infoArmorBackgroundWindow);
	NanoUtility::InventoryTheme(_infoArmorContextTopWindow);
	NanoUtility::InventoryTheme(_infoArmorContextBottomWindow);

	NanoUtility::InventoryTheme(_toolBarBackgroundWindow);
	NanoUtility::InventoryTheme(_toolBarContextWindow);
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

	//Armor Menu Cancel/Confirm
	_infoArmorConfirmWindow->center();
	_infoArmorConfirmWindow->setPosition(_infoArmorContextTopWindow->position() + _infoArmorContextTopWindow->size() - _infoArmorConfirmWindow->size() - Vector2i(0, 0));


	//Quests
	infoOffset = Vector2i(_questInfoBackgroundWindow->width() / 2 + 10, 0);
	_questBackgroundWindow->center();
	_questContextTopWindow->setPosition(_questBackgroundWindow->position() + Vector2i(0, 0) - infoOffset);
	_questContextBottomWindow->setPosition(_questBackgroundWindow->position() + Vector2i(0, _questContextTopWindow->height()) - infoOffset);
	for (pair<int, Window*> pair : _questRows) {
		pair.second->setPosition(_questBackgroundWindow->position() + Vector2i(0, _questContextTopWindow->height()) + Vector2i(0, 4 + (pair.first * 90)) - infoOffset);
	}
	_questBackgroundWindow->setPosition(_questBackgroundWindow->position() + Vector2i(0, 0) - infoOffset);

	//Quests Info
	_questInfoBackgroundWindow->center();
	_questInfoContextTopWindow->setPosition(_questInfoBackgroundWindow->position() + Vector2i(0, 0) + Vector2i(_questBackgroundWindow->width() / 2 + 10, 0));
	_questInfoContextBottomWindow->setPosition(_questInfoBackgroundWindow->position() + Vector2i(0, _questInfoContextTopWindow->height()) + Vector2i(_questBackgroundWindow->width() / 2 + 10, 0));
	_questInfoBackgroundWindow->setPosition(_questInfoBackgroundWindow->position() + Vector2i(0, 0) + Vector2i(_questBackgroundWindow->width() / 2 + 10, 0));

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

void Inventory::SetItem(int slot, Item item, std::map<int, Item> &list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3) {
	item.imgLocation = NanoUtility::LoadImage(item.image, mImagesData);
	list[slot] = item;

	refreshItem(slot, list, list2, list3);
}
void Inventory::SetQuest(int slot, Quest item, std::map<int, Quest>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3) {
	item.imgLocation = NanoUtility::LoadImage(item.image, mImagesData);
	list[slot] = item;

	refreshQuest(slot, list, list2, list3);
}

void Inventory::RemoveItem(int slot, std::map<int, Item>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3) {
	_items[slot].imgLocation = 1; //Set to image empty
	_items[slot].name = "none"; //Reset name so we now its an empty slot

	refreshItem(slot, list, list2, list3);
}
void Inventory::RemoveQuest(int slot, std::map<int, Quest>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3) {
	list[slot].imgLocation = 1; //Set to image empty
	list[slot].name = "none"; //Reset name so we now its an empty slot

	refreshQuest(slot, list, list2, list3);
}
void Inventory::RemoveItem(Item item) {
	for (int i = 0; i < allUserItems.size(); i++) {
		if (allUserItems[i].id == item.id && allUserItems[i].amount > 1) {
			allUserItems[i].amount -= 1;
			break;
		}
		else if (allUserItems[i].id == item.id) {
			allUserItems.erase(allUserItems.begin() + i);
			break;
		}
	}

	SortInventory(_activeType, _items, _slotImages, _slotCounters);
}

void Inventory::AddItem(Item item, int count) {
	bool newItem = true;
	for (auto& pair : allUserItems) {
		if (pair.id == item.id && item.isStackable) {
			pair.amount += 1;
			newItem = false;
		}
		else if (pair.id == item.id && !item.isStackable) {
			item.amount = 1;
			allUserItems.push_back(item);
		}
	}

	if (newItem) {
		item.amount = 1;
		allUserItems.push_back(item);
	}

	SortInventory(_activeType, _items, _slotImages, _slotCounters);
}
void Inventory::AddQuest(Quest item) {
	allUserQuests.push_back(item);
	SortQuests(_activeTypeQuests, _quests, _questImages, _questCounters);
}
void Inventory::DropItem(Item item, vec3 position) {
	RemoveItem(item);

	///Create CSM entity
	auto Entity = csm.CreateEntity();
	string modelPath = "resources/Inventory/Models/" + item.name + ".fbx";
	NanoUtility::replace(modelPath, " ", "");

	csm.AddComponent(Entity, ModelMeshC{ AnimModel(modelPath, position, 0.3f) });
	csm.AddComponent(Entity, TransformC{ position, 0.3f });
	csm.AddComponent(Entity, EntityC{ item });
}
void Inventory::SwitchItems(int slot1, int slot2, std::map<int, Item>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3) {
	Item itm1 = list[slot1];
	Item itm2 = list[slot2];

	list[slot1] = itm2;
	list[slot2] = itm1;

	refreshItem(slot1, list, list2, list3);
	refreshItem(slot2, list, list2, list3);
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
	_infoArmorConfirmWindow->setVisible(show);
}
void Inventory::ShowQuestInfo(bool show) {
	_questInfoBackgroundWindow->setVisible(show);
	_questInfoContextTopWindow->setVisible(show);
	_questInfoContextBottomWindow->setVisible(show);
}
void Inventory::ShowQuest(bool show) {
	_questBackgroundWindow->setVisible(show);
	_questContextTopWindow->setVisible(show);
	_questContextBottomWindow->setVisible(show);
	ShowQuestInfo(show);

	_naviButton->setVisible(show);
	for (Button* btn : _naviButtons) btn->setTextColor({ 255, 255, 255, 255 });
	_naviButtons.at(0)->setTextColor({ 214, 171, 63, 255 });

	for (pair<int, Window*> pair : _questRows)
		pair.second->setVisible(show);
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
void Inventory::createSlotRow(Window* row, vector<pair<int, Window*>>& list, bool quests, int rows, vector<string> items) {
	row->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 20, 8));
	row->setFixedSize({ 812 , 96 });
	rowsVar += 1;

	for (int i = 0; i < rows; i++) {
		int pos = NanoUtility::LoadImage(items[i], mImagesData);
		if (quests) _quests[uniqueSlotId] = Quest();
		else _items[uniqueSlotId] = Item({ "none", {}, items[i], InventoryCataType::Miscellaneous, pos });
		createSlot(row, 70, 70, uniqueSlotId++, quests);
	}

	int rowNum = list.size();
	if (rowNum < 0) rowNum = 0;

	list.push_back(make_pair(rowNum, row));
}
Widget* Inventory::createSlot(Widget* window, int x, int y, int slotNum, bool quests) {
	if(quests) { //Quests
		Widget* slot = new Widget(window);
		slot->setFixedSize({ x, y });
		slot->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, -x));

		ImageView* backgroundImage = new ImageView(slot, mImagesData[0].first.texture());
		backgroundImage->setFixedSize({ x, y });

		int num = _items.size() - 1;
		ImageView* frontImage = new ImageView(slot, mImagesData[_quests[slotNum].imgLocation].first.texture());
		frontImage->setFixedSize({ x, y });
		_questImages[slotNum] = frontImage;

		Button* selectItem = new Button(slot, " ");
		selectItem->setFixedSize({ x, y });
		selectItem->setCallback([slotNum, this, frontImage]() {
			Quest itm = _quests[slotNum];
			frontImage->bindImage(mImagesData[itm.imgLocation].first.texture());

			if (itm.name != "none" && _backpackBackgroundWindow->visible()) {
				//Update Info widget with correct info
				UpdateQuestsInfo(itm.name, itm.imgLocation, itm.stat1, itm.icon1, itm.stat2, itm.icon2, itm.desc);

				//Show Info Menu
				ShowQuestInfo(true);

				//Realign Windows
				realignWindows(tempWidth, tempHeight);
			}
			else {
				ShowQuestInfo(true);
				UpdateQuestsInfo("Select a Quest", 0, "", 0, "", 0, {"Please select a quest..."});
				realignWindows(tempWidth, tempHeight);
			}

			std::cout << "slot -> " << slotNum << std::endl;
			});

		Label* lbl = new Label(slot, "00", "sans", 16);
		_questCounters.insert(std::make_pair(slotNum, lbl));
		return slot;
	}
	else { //Inventory
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
				UpdateInfo(itm.name, itm.imgLocation, itm.stat1, itm.icon1, itm.stat2, itm.icon2, itm.stat3, itm.icon3, itm.desc);

				//Select Thingy
				_selectedItems.clear();
				_selectedItems.push_back(make_pair(slotNum, selectedImage));

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
			});

		Label* lbl = new Label(slot, "00", "sans", 16);
		_slotCounters.insert(std::make_pair(slotNum, lbl));
		return slot;
	}
}

void Inventory::UpdateInfo(string title, int image, string Stats1, int stats1Icon, string Stats2, int stats2Icon, string Stats3, int stats3Icon, std::vector<std::string> desc) {
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

	for (int i = 0; i < desc.size(); i++) {
		_desc.at(i)->setCaption(desc.at(i));
	}
	for (int i = desc.size(); i < _desc.size(); i++) {
		_desc.at(i)->setCaption("");
	}
}
void Inventory::refreshItem(int slotNum, std::map<int, Item>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3) {
	Item itm = list[slotNum];
	list2[slotNum]->bindImage(mImagesData[itm.imgLocation].first.texture());

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << itm.amount;

	if(itm.amount > 0 && itm.isStackable)
		list3[slotNum]->setCaption(ss.str());
	else list3[slotNum]->setCaption("");
		
	list3[slotNum]->setPosition(list2[slotNum]->position() + Vector2i(50, 50));

	if (itm.type == InventoryCataType::Armor) {
		if (itm.ArmorType == ArmorType::Head) {
		} else if (itm.ArmorType == ArmorType::Chest) {
		} else if (itm.ArmorType == ArmorType::Legs) {
		} else if (itm.ArmorType == ArmorType::Boots) {
		}
	}
}
void Inventory::SortInventory(InventoryCataType sortType, std::map<int, Item>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3) {
	int slot = 0;
	_activeType = sortType;

	switch (sortType) {
	case InventoryCataType::AllItems:
		for (auto item : allUserItems)
			SetItem(slot++, item, list, list2, _slotCounters);
		break;
	case InventoryCataType::Armor:
		for (auto item : allUserItems)
			if (item.type == InventoryCataType::Armor)SetItem(slot++, item, list, list2, list3);
		break;
	case InventoryCataType::Miscellaneous:
		for (auto item : allUserItems)
			if (item.type == InventoryCataType::Miscellaneous)SetItem(slot++, item, list, list2, list3);
		break;
	case InventoryCataType::Tools:
		for (auto item : allUserItems)
			if (item.type == InventoryCataType::Tools)SetItem(slot++, item, list, list2, list3);
		break;
	}

	for (int i = slot; i < 50; i++) {
		RemoveItem(i, _items, _slotImages, _slotCounters);
	}

	for (int i = 50; i < 60; i++) {
		refreshItem(i, _items, _slotImages, _slotCounters);
	}
}
void Inventory::UpdateArmorInfo(string itemName) {

}
void Inventory::SortQuests(QuestCataType sortType, std::map<int, Quest>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3) {
	int slot = 60;
	_activeTypeQuests = sortType;

	switch (sortType) {
	case QuestCataType::AllQuests:
		for (auto item : allUserQuests)
			SetQuest(slot++, item, list, list2, list3);
		break;
	case QuestCataType::Completed:
		for (auto item : allUserQuests)
			if (item.type == InventoryCataType::Armor)SetQuest(slot++, item, list, list2, list3);
		break;
	case QuestCataType::Active:
		for (auto item : allUserQuests)
			if (item.type == InventoryCataType::Miscellaneous)SetQuest(slot++, item, list, list2, list3);
		break;
	case QuestCataType::Open:
		for (auto item : allUserQuests)
			if (item.type == InventoryCataType::Tools)SetQuest(slot++, item, list, list2, list3);
		break;
	case QuestCataType::Closed:
		for (auto item : allUserQuests)
			if (item.type == InventoryCataType::Tools)SetQuest(slot++, item, list, list2, list3);
		break;
	}

	for (int i = slot; i < 110; i++) {
		RemoveQuest(i, list, list2, list3);
	}
}
void Inventory::refreshQuest(int slotNum, std::map<int, Quest>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3) {
	Quest itm = list[slotNum];
	list2[slotNum]->bindImage(mImagesData[itm.imgLocation].first.texture());

	list3[slotNum]->setCaption(std::to_string(itm.reqLevel));

	list3[slotNum]->setPosition(list2[slotNum]->position() + Vector2i(50, 50));
}
void Inventory::UpdateQuestsInfo(string title, int image, string Stats1, int stats1Icon, string Stats2, int stats2Icon, std::vector<std::string> desc) {
	if (Stats1 == "") _infoButtons["1"]->setVisible(false);
	_infoQuestButtons["1"]->setCaption(Stats1);
	_infoQuestButtons["1"]->setIcon(stats1Icon);

	_infoQuestButtons["2"]->setCaption(Stats2);
	_infoQuestButtons["2"]->setIcon(stats2Icon);

	_infoQuestLabels["TITLE"]->setCaption(title);
	//_infoQuestImages["PREVIEW"]->bindImage(mImagesData[image].first.texture());

	for (int i = 0; i < desc.size(); i++) {
		_questDesc.at(i)->setCaption(desc.at(i));
	}
	for (int i = desc.size(); i < _desc.size(); i++) {
		_questDesc.at(i)->setCaption("");
	}
}