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
#include <thread>
#include <chrono>
#include <sstream>

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
	void SetItem(int slot, Item item, std::map<int, Item>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3);
	void SetQuest(int slot, Quest item, std::map<int, Quest>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3);

	/*
	Remove an item from the users inventory
	*/
	void RemoveItem(int slot, std::map<int, Item>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3);
	void RemoveQuest(int slot, std::map<int, Quest>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3);
	void RemoveItem(Item item);

	/*
	*/
	void AddItem(Item item, int count = 1);
	void AddQuest(Quest item);

	/*
	*/
	void DropItem(Item item, vec3 position, bool removeItem = true);

	/*
	Switch two items from there slot.
	e.g Slot 5 to Slot 0
	*/
	void SwitchItems(int slot1, int slot2, std::map<int, Item>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3);

	/*
	Keyboard logic for the inventory precoded
	*/
	void keyCallbackEvent(int key, int scancode, int action, int mods, GLFWwindow* window);

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
	void SortInventory(InventoryCataType sortType, std::map<int, Item>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3);

	/*
	*/
	void SortQuests(QuestCataType sortType, std::map<int, Quest>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3);

	/*
	*/
	void SetActiveSlot(int slot = -1);

	/*
	*/
	std::vector<Item> GetInvItems() { return this->allUserItems; }

	/*
	*/
	Item GetActiveItem() { return this->_items[SelectedSlot]; }

	void SetHealth(int min, int max);
	void SetMana(int min, int max);
	void SetXp(int xpI);
	void SetCurrencry(int currencyI);
	void AddCurrency(int amount);
	void AddXp(int amount);

	string GetXp() { return xp->caption(); }
	string GetCurrency() { return currency->caption(); }

	void SetNpcText(string line1, string line2, string line3, bool show = true);

private:
	//Private Func
	void createSlotRow(Window* row, vector<pair<int, Window*>>& list, bool quests = false, int rows = 10,  vector<string> items = { "empty" , "empty" , "empty" , "empty" , "empty" , "empty" , "empty" ,"empty" ,"empty" ,"empty" });
	Widget* createSlot(Widget* window, int x, int y, int slotNum, bool quests);

	void UpdateInfo(string title, int image, string Stats1, int stats1Icon, string Stats2, int stats2Icon, string Stats3, int stats3Icon, std::vector<std::string> desc = { "" });
	void refreshItem(int slotNum, std::map<int, Item>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3);
	void refreshQuest(int slotNum, std::map<int, Quest>& list, std::map<int, ImageView*>& list2, std::map<int, Label*>& list3);
	void UpdateArmorInfo(string itemName);
	void UpdateQuestsInfo(string title, int image, string Stats1, int stats1Icon, string Stats2, int stats2Icon, std::vector<std::string> desc = { "" });

	//Private Vars
	std::vector<Screen*> interactiveScreens;
	std::vector<Screen*> nonInteractiveScreens;


	//Inventory Windows
	Window* _infoBackgroundWindow;
	Window* _infoContextTopWindow;
	Window* _infoContextBottomWindow;

	Window* _infoArmorBackgroundWindow;
	Window* _infoArmorContextTopWindow;
	Window* _infoArmorContextBottomWindow;
	Window* _infoArmorConfirmWindow;

	Window* _backpackBackgroundWindow;
	Window* _backpacContextTopWindow;
	Window* _backpacContextBottomWindow;

	Window* _naviButton;

	Window* _npcTextBackground;
	Window* _npcText;
	Window* _notifications;

	Window* _toolBarBackgroundWindow;
	Window* _toolBarContextWindow;
	Window* _toolBarHudContextWindow;

	Button* health;
	Button* mana;
	Button* xp;
	Button* currency;

	//Quest Windows
	Window* _questBackgroundWindow;
	Window* _questContextTopWindow;
	Window* _questContextBottomWindow;

	Window* _questInfoBackgroundWindow;
	Window* _questInfoContextTopWindow;
	Window* _questInfoContextBottomWindow;

	Vector2i _position;

	imagesDataType mImagesData;
	InventoryCataType _activeType = InventoryCataType::AllItems;
	QuestCataType _activeTypeQuests = QuestCataType::AllQuests;
	Camera& _camera;

	//Inventory Maps & Vectors
	std::map<std::string, Label*> _infoLabels;
	std::map<std::string, ImageView*> _infoImages;
	std::map<std::string, Button*> _infoButtons;
	std::map<std::string, GLCanvas*> _infoLines;
	std::map<int, ImageView*> _slotImages;
	std::map<int, Label*> _slotCounters;
	std::map<int, Item> _items;
	std::vector<pair<int, ImageView*>> _selectedItems;
	std::vector<Button*> _naviButtons;
	std::vector<Button*> _filterButtons;
	std::vector<pair<int, Window*>> _invRows;
	std::vector<Label*> _desc;
	std::vector<Item> allUserItems;

	//Quests Maps & Vectors
	std::map<std::string, Button*> _infoQuestButtons;
	std::map<std::string, GLCanvas*> _infoQuestLines;
	std::map<std::string, Label*> _infoQuestLabels;
	std::map<std::string, ImageView*> _infoQuestImages;
	std::map<int, ImageView*> _questImages;
	std::map<int, Label*> _questCounters;
	std::map<int, Quest> _quests;
	std::vector<Button*> _filterQuestButtons;
	std::vector<pair<int, Window*>> _questRows;
	std::vector<Label*> _questDesc;
	std::vector<Quest> allUserQuests;

	//Npc & Other
	std::vector<Label*> _NpcDesc;

	//Armor Vectors & Maps
	std::vector<std::string> itemsHead = { "No Helmet" };
	std::vector<std::string> itemsChest = { "No Chestplate" };
	std::vector<std::string> itemsLegs = { "No Leggings" };
	std::vector<std::string> itemsBoots = { "No Boots" };

	//????
	std::vector<pair<int, Window*>> _tbRows;


	//Other
	int uniqueSlotId = 0;
	int rowsVar = 0;
	int _selecteditem = -1;
	int tempSlotNum = -1;

	int frame = 0;

	int tempWidth = 0;
	int tempHeight = 0;
	int SelectedImageIndex = 0;
	int EmptyImageIndex = 0;
	int SelectedSlot = -1;

	ComboBox* _head;
	ComboBox* _shirts;
	ComboBox* _trousers;
	ComboBox* _boots;
};