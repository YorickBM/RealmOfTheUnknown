#pragma once
#include "SOIL2/SOIL2.h"
#pragma region NanoGUI Settings (GLAD & Appel & Prototypes)
#if defined(NANOGUI_GLAD)
#if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
#define GLAD_GLAPI_EXPORT
#endif

#ifndef NULL
#define NULL 0
#endif

#include <glad/glad.h>
#else
#if defined(__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#else
#define GL_GLEXT_PROTOTYPES
#endif
#endif
#pragma endregion
#include <nanogui/nanogui.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/glutil.h>
#include <nanogui/label.h>
#include <nanogui/theme.h>
#include <nanogui/formhelper.h>
#include <nanogui/slider.h>
#include <map>

#include "CustomGrid.h"
#include "GLTexture.h"
#include "NanoUtility.h"
#include "ComponentSystemManager.h"
#include "Components.h"
#include "Item.h"

using namespace nanogui;
using imagesDataType = vector<pair<GLTexture, GLTexture::handleType>>;
extern ComponentSystemManager csm;

class Inventory {
public:

	/*
	 Create basic inventory with all empty slots & info Widget.
	 Also creates a toolbar
	 */
	Inventory(GLFWwindow* window, int width, int height, Camera& camera, Vector2i position = Vector2i(15,15)) 
		: _camera(camera), tempWidth(width), tempHeight(height) {

		Screen* backgroundScreen = NanoUtility::CreateScreen(window, nonInteractiveScreens);
		Screen* frontScreen = NanoUtility::CreateScreen(window, interactiveScreens);

		//Create info Screen
		_infoBackgroundWindow = new Window(backgroundScreen, "");
		_infoContextTopWindow = new Window(frontScreen, "");
		_infoContextBottomWindow = new Window(frontScreen, "");

		#pragma region Init Textues
		//Gen Textures
		GLTexture texture("backgroundSlot");
		auto data = texture.load("resources/backgroundSlot.png");
		mImagesData.emplace_back(std::move(texture), std::move(data));

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
		_infoButtons["1"] = NanoUtility::iconText(_infoContextBottomWindow, "Min. Level: N/A", ENTYPO_ICON_LAB_FLASK, "sand-bold", 16, { 255, 255, 255, 255 }, { 200, 20});
		_infoLines["1"] = NanoUtility::line(_infoContextBottomWindow);
		_infoButtons["2"] = NanoUtility::iconText(_infoContextBottomWindow, "Req. Class: N/A", ENTYPO_ICON_NEWSLETTER, "sand-bold", 16, { 255, 255, 255, 255 }, { 200, 20 });
		_infoLines["2"] = NanoUtility::line(_infoContextBottomWindow);
		_infoButtons["3"] = NanoUtility::iconText(_infoContextBottomWindow, "Req. Quest: N/A", ENTYPO_ICON_CLIPBOARD, "sand-bold", 16, { 255, 255, 255, 255 }, {200, 20});

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
		NanoUtility::iconText(_backpacContextTopWindow, "INVENTORY", ENTYPO_ICON_SUITCASE, "sand-bold", 30, { 255, 255, 255, 255 }, {200, 38});
		NanoUtility::emptySpace(_backpacContextTopWindow, 10, { 40, 40, 43, 255 });
		Widget* tools = new Widget(_backpacContextTopWindow);
		tools->setLayout(new BoxLayout(Orientation::Horizontal,
			Alignment::Middle, 0, 6));
		Button* b = new Button(tools, "All");
		b->setCallback([&] {
			cout << "Show All" << endl;
			for (Button* btn : _filterButtons) btn->setTextColor({89, 91, 91, 255});
			_filterButtons.at(0)->setTextColor({214, 171, 63, 255});
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

		frontScreen->setVisible(true);
		frontScreen->performLayout();
		backgroundScreen->setVisible(true);
		backgroundScreen->performLayout();

		Hide();
	}

	/*
	Render the inventory
	*/
	void render() {
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

	/*
	Show or Hide the info widget from the inventory
	IMPORTANT: Won't reset Inventory Offset (yet).
	*/
	void ShowInfo(bool show = true) {
		_infoBackgroundWindow->setVisible(show);
		_infoContextBottomWindow->setVisible(show);
		_infoContextTopWindow->setVisible(show);
	}
	
	std::vector<Screen*> getScreens() { return this->interactiveScreens; }

	/*
	Realign the inventory too the center of the window.
	*/
	void realignWindows(int width, int height) {
		//Info Widget Offset (When not shown set to 0,0)
		Vector2i infoOffset = Vector2i(_infoBackgroundWindow->width() / 2 + 10, 0);
		if (!_infoBackgroundWindow->visible()) infoOffset = Vector2i(0, 0);

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

		tempWidth = width;
		tempHeight = height;
	}

	/*
	Set an item in the users inventory on a specific slot.
	*/
	void SetItem(int slot, Item item) {
		item.imgLocation = NanoUtility::LoadImage(item.image, mImagesData);
		_items[slot] = item;

		refreshItem(slot);
	}

	/*
	Remove an item from the users inventory
	*/
	void RemoveItem(int slot) {
		_items[slot].imgLocation = 1; //Set to image empty
		_items[slot].name = "none"; //Reset name so we now its an empty slot

		refreshItem(slot);
	}

	/*
	*/
	void AddItem(Item item, int count = 1) {
		allUserItems.insert(make_pair(item, count));
		SortInventory(_activeType);
	}

	/*
	*/
	void DropItem(Item item, vec3 position) {
		if (allUserItems.at(item) > 1) {
			allUserItems.at(item) -= 1;
		}
		else {
			allUserItems.erase(item);
		}

		///Create CSM entity
		auto Entity = csm.CreateEntity();
		string modelPath = "resources/Inventory/Models/" + item.name + ".fbx";
		NanoUtility::replace(modelPath, " ", "");

		csm.AddComponent(Entity, ModelMeshC{AnimModel(modelPath, position, 0.3f)});
		csm.AddComponent(Entity, TransformC{ position, 0.3f });
		csm.AddComponent(Entity, EntityC{ item });
		
		SortInventory(_activeType);
	}

	/*
	Switch two items from there slot.
	e.g Slot 5 to Slot 0
	*/
	void SwitchItems(int slot1, int slot2) {
		Item itm1 = _items[slot1];
		Item itm2 = _items[slot2];

		_items[slot1] = itm2;
		_items[slot2] = itm1;

		refreshItem(slot1);
		refreshItem(slot2);
	}

	/*
	Keyboard logic for the inventory precoded
	*/
	void keyCallbackEvent(int key, int scancode, int action, int mods) {
		if (GLFW_KEY_I == key && GLFW_PRESS == action)
		{
			if (!_backpackBackgroundWindow->visible() && frame <= 0) {
				ShowWithInfo();
				realignWindows(tempWidth, tempHeight);

				frame = 10;
			}
			else if(frame <= 0){
				Hide();

				frame = 10;
			}
		}
		else if (GLFW_KEY_Q == key && GLFW_PRESS == action)
		{
			DropItem(_items[SelectedSlot], _camera.GetPosition());
		}
	}

	/*
	Show or Hide the Inventory
	*/
	void ShowInventory(bool show = true) {
		_backpackBackgroundWindow->setVisible(show);
		_backpacContextTopWindow->setVisible(show);
		_backpacContextBottomWindow->setVisible(show);

		for (pair<int, Window*> pair : _invRows)
			pair.second->setVisible(show);
	}

	/*
	Hide the Inventory & Info Widgets
	*/
	void Hide() {
		ShowInfo(false);
		ShowInventory(false);
	}
	/*
	Show ONLY the Inventory Widgets
	*/
	void Show() {
		ShowInfo(false);
		ShowInventory(true);
	}
	/*
	Show the Inventory & Info Widgets
	*/
	void ShowWithInfo() {
		ShowInfo(true);
		ShowInventory(true);
	}

	/*
	Reoder Inventory Items
	- Type -> Selected catagory
	*/
	void SortInventory(InventoryCataType sortType) {
		int slot = 0;
		_activeType = sortType;

		switch (sortType) {
		case InventoryCataType::All:
			for(auto item : allUserItems)
				SetItem(slot++, item.first);
			break;
		case InventoryCataType::Armor:
			for (auto item : allUserItems)
				if(item.first.type == InventoryCataType::Armor)SetItem(slot++, item.first);
			break;
		case InventoryCataType::Miscellaneous:
			for (auto item : allUserItems)
				if (item.first.type == InventoryCataType::Miscellaneous)SetItem(slot++, item.first);
			break;
		case InventoryCataType::Tools:
			for (auto item : allUserItems)
				if (item.first.type == InventoryCataType::Tools)SetItem(slot++, item.first);
			break;
		}

		for (int i = slot; slot < 40; slot++) {
			RemoveItem(slot);
		}
	}

	/*
	*/
	void SetActiveSlot(int slot = -1) {
		//Make all slots NOT active

		if (slot != -1) {
			//Make specificslot active
		}
	}

private:
	std::vector<Screen*> interactiveScreens;
	std::vector<Screen*> nonInteractiveScreens;

	Window* _infoBackgroundWindow;
	Window* _infoContextTopWindow;
	Window* _infoContextBottomWindow;

	Window* _backpackBackgroundWindow;
	Window* _backpacContextTopWindow;
	Window* _backpacContextBottomWindow;

	Window* _toolBarBackgroundWindow;
	Window* _toolBarContextWindow;

	Vector2i _position;
	vector<Label*> desc;
	vector<Button*> _filterButtons;

	map<Item, int> allUserItems;

	imagesDataType mImagesData;
	InventoryCataType _activeType = InventoryCataType::All;
	Camera& _camera;

	void createSlotRow(Window* row, vector<pair<int, Window*>> &list, int rows = 10, vector<string> items = { "empty" , "empty" , "empty" , "empty" , "empty" , "empty" , "empty" ,"empty" ,"empty" ,"empty" }) {
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
	Widget* createSlot(Widget* window, int x, int y, int slotNum) {
		Widget* slot = new Widget(window);
		slot->setFixedSize({x, y});
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
		selectItem->setCallback([slotNum, this, frontImage, selectedImage, x, y]() {
			Item itm = _items[slotNum];
			frontImage->bindImage(mImagesData[itm.imgLocation].first.texture());

			if (itm.name != "none" && _backpackBackgroundWindow->visible()) {
				//Update Info widget with correct info
				UpdateInfo(itm.name, itm.imgLocation, itm.stat1, itm.icon1, itm.stat2, itm.icon2, itm.stat3, itm.icon3);

				//Render Count 
				int amount = allUserItems.at(itm);
				
				//Show Info Menu
				ShowInfo(true);

				//Realign Windows
				realignWindows(tempWidth, tempHeight);

				//Set selected Slot int to slotNum
				SelectedSlot = slotNum;
			}
			else {
				ShowInfo(false);
				realignWindows(tempWidth, tempHeight);
			}
			
			std::cout << "Slot ->" << slotNum << std::endl; 
		});

		return slot;
	}

	void UpdateInfo(string title, int image, string Stats1, int stats1Icon, string Stats2, int stats2Icon, string Stats3, int stats3Icon) {
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
	void refreshItem(int slotNum) {
		_slotImages[slotNum]->bindImage(mImagesData[_items[slotNum].imgLocation].first.texture());
	}

	map<int, Item> _items;
	vector<pair<int, Window*>> _invRows;
	vector<pair<int, Window*>> _tbRows;

	int uniqueSlotId = 0;
	int rowsVar = 0;
	int _selecteditem = -1;
	int tempSlotNum = -1;

	map<string, Label*> _infoLabels;
	map<string, ImageView*> _infoImages;
	map<string, Button*> _infoButtons;
	map<string, GLCanvas*> _infoLines;
	vector<pair<int, ImageView*>> _selectedItems;

	map<int, ImageView*> _slotImages;
	int frame = 0;

	int tempWidth = 0;
	int tempHeight = 0;
	int SelectedImageIndex = 0;
	int EmptyImageIndex = 0;
	int SelectedSlot = -1;
};