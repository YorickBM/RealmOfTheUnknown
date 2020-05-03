#pragma once
#pragma region includes
// GLEW (prevent gl.h before glew.h)
#define GLEW_STATIC
#include <GL/glew.h>

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

#include "NanoUtility.h"
#include "Components.h"
#include "ComponentSystemManager.h"
#include "Item.h"

#pragma endregion

using namespace nanogui;
using imagesDataType = vector<pair<GLTexture, GLTexture::handleType>>;
extern ComponentSystemManager csm;

class Inventory {
public:

	/*
	 Create basic inventory with all empty slots & info Widget.
	 Also creates a toolbar
	 */
	Inventory(GLFWwindow* window, int width, int height, Camera& camera, Vector2i position = Vector2i(15, 15));

	/*
	Render the inventory
	*/
	void render();

	/*
	Show or Hide the info widget from the inventory
	IMPORTANT: Won't reset Inventory Offset (yet).
	*/
	void ShowInfo(bool show = true);
	void ShowArmor(bool show = true);
	void ShowQuestInfo(bool show = true);
	void ShowQuest(bool show = true);
	
	std::vector<Screen*> getScreens();

	/*
	Realign the inventory too the center of the window.
	*/
	void realignWindows(int width, int height);

	/*
	Set an item in the users inventory on a specific slot.
	*/
	void SetItem(int slot, Item item);

	/*
	Remove an item from the users inventory
	*/
	void RemoveItem(int slot);

	/*
	*/
	void AddItem(Item item, int count = 1);

	/*
	*/
	void DropItem(Item item, vec3 position);

	/*
	Switch two items from there slot.
	e.g Slot 5 to Slot 0
	*/
	void SwitchItems(int slot1, int slot2);

	/*
	Keyboard logic for the inventory precoded
	*/
	void keyCallbackEvent(int key, int scancode, int action, int mods);

	/*
	Show or Hide the Inventory
	*/
	void ShowInventory(bool show = true);

	/*
	Hide the Inventory & Info Widgets
	*/
	void Hide();
	/*
	Show ONLY the Inventory Widgets
	*/
	void Show();
	/*
	Show the Inventory & Info Widgets
	*/
	void ShowWithInfo();

	/*
	Reoder Inventory Items
	- Type -> Selected catagory
	*/
	void SortInventory(InventoryCataType sortType);

	/*
	*/
	void SetActiveSlot(int slot = -1);

private:
	std::vector<Screen*> interactiveScreens;
	std::vector<Screen*> nonInteractiveScreens;

	Window* _infoBackgroundWindow;
	Window* _infoContextTopWindow;
	Window* _infoContextBottomWindow;

	Window* _infoArmorBackgroundWindow;
	Window* _infoArmorContextTopWindow;
	Window* _infoArmorContextBottomWindow;

	Window* _backpackBackgroundWindow;
	Window* _backpacContextTopWindow;
	Window* _backpacContextBottomWindow;

	Window* _naviButton;

	Window* _toolBarBackgroundWindow;
	Window* _toolBarContextWindow;

	Vector2i _position;
	std::vector<Label*> desc;
	std::vector<Button*> _filterButtons;

	std::vector<Item> allUserItems;

	imagesDataType mImagesData;
	InventoryCataType _activeType = InventoryCataType::All;
	Camera& _camera;

	void createSlotRow(Window* row, vector<pair<int, Window*>>& list, int rows = 10, vector<string> items = { "empty" , "empty" , "empty" , "empty" , "empty" , "empty" , "empty" ,"empty" ,"empty" ,"empty" });
	Widget* createSlot(Widget* window, int x, int y, int slotNum);

	void UpdateInfo(string title, int image, string Stats1, int stats1Icon, string Stats2, int stats2Icon, string Stats3, int stats3Icon);
	void refreshItem(int slotNum);

	std::map<int, Item> _items;
	std::vector<pair<int, Window*>> _invRows;
	std::vector<pair<int, Window*>> _tbRows;

	int uniqueSlotId = 0;
	int rowsVar = 0;
	int _selecteditem = -1;
	int tempSlotNum = -1;

	std::map<std::string, Label*> _infoLabels;
	std::map<std::string, ImageView*> _infoImages;
	std::map<std::string, Button*> _infoButtons;
	std::map<std::string, GLCanvas*> _infoLines;
	std::vector<pair<int, ImageView*>> _selectedItems;
	std::map<int, ImageView*> _slotImages;
	std::map<int, Label*> _slotCounters;
	std::vector<Button*> _naviButtons;

	int frame = 0;

	int tempWidth = 0;
	int tempHeight = 0;
	int SelectedImageIndex = 0;
	int EmptyImageIndex = 0;
	int SelectedSlot = -1;
};