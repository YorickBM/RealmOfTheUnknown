#pragma once
#include "SOIL2/SOIL2.h"
#pragma region NanoGUI Settings (GLAD & Appel & Prototypes)
#if defined(NANOGUI_GLAD)
#if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
#define GLAD_GLAPI_EXPORT
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
#include <string>
#include <unordered_map>
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

#include <nanogui/opengl.h>
#include <nanogui/checkbox.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>

#include "CustomGrid.h"
#include "GLTexture.h"
#include "NanoUtility.h"
#include "ScreenManager.h"

using namespace nanogui;
using imagesDataType = vector<pair<GLTexture, GLTexture::handleType>>;
extern std::unordered_map<std::string, std::string> settings;
extern std::vector<std::string> resolutions;

class SettingsScreen : public BaseScreen {
public:
	SettingsScreen(Screen* screen, int width, int height, GLFWwindow* window, Screen* backgroundImageScreen) {
		_screen = screen;
		_screenImage = backgroundImageScreen;

		_backgroundImage = new Window(_screenImage, "");

		//SetupLayout
		_backgroundImage->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));

		//Render Background Image
		ImageView* backgroundImg = new ImageView(_backgroundImage, mImagesDataSettingsScreen[NanoUtility::LoadImage("MineRender5", mImagesDataSettingsScreen)].first.texture());
		_backgroundImage->center();

#pragma region Making Windows
		//Setting Background Screen
		_backMenu = new Window(_screenImage, "");

		GLCanvas* topCanvas = new GLCanvas(_backMenu);
		topCanvas->setBackgroundColor({ 46, 48, 52, 255 });
		topCanvas->setFixedSize({ width - 100, height - 100 });
		topCanvas->setPosition({ 50, 50 });

		//Settings FrontMenu Screen
		_frontMenu = new Window(_screen, "");
		_frontMenu->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
		_frontMenu->setFixedSize(Vector2i(60, height));
		_frontMenu->setPosition({ 60, 75 });

		//Settings FrontMenuMiddle screen
		_frontMenuMiddle = new Window(_screen, "");
		_frontMenuMiddle->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill));
		_frontMenuMiddle->setFixedSize(Vector2i(width, height));
		_frontMenuMiddle->setPosition(Vector2i(width / 2 - 100, 50));

		//Settings Menuswitchbuttons
		_frontMenuSwitchButtons = new Window(_screen, "");
		_frontMenuSwitchButtons->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Fill, 0, 20));
		_frontMenuSwitchButtons->setFixedSize(Vector2i(width, 30));
		_frontMenuSwitchButtons->setPosition(Vector2i(width / 2 - 130, 80));

		//Settings different screens
		_graphicsScreen = new Window(_screen, "");
		_graphicsScreen->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill));
		_graphicsScreen->setFixedSize(Vector2i(300, height));
		_graphicsScreen->setPosition(Vector2i(60, 100));

		_controlsScreen = new Window(_screen, "");
		_controlsScreen->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill));
		_controlsScreen->setFixedSize(Vector2i(width, height));
		_controlsScreen->setPosition(Vector2i(60, 100));

		_interfaceScreen = new Window(_screen, "");
		_interfaceScreen->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill));
		_interfaceScreen->setFixedSize(Vector2i(width, height));
		_interfaceScreen->setPosition(Vector2i(60, 100));

		_audioScreen = new Window(_screen, "");
		_audioScreen->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill));
		_audioScreen->setFixedSize(Vector2i(width, height));
		_audioScreen->setPosition(Vector2i(60, 100));

#pragma endregion
#pragma region Main Screen
		//Settings main screen
		///new Label(_frontMenuMiddle, "------ Settings ------", "sans-bold", 30);
		new Label(_frontMenuMiddle, "------ Settings ------", "sans-bold", 30);
		new Label(_frontMenu, "Settings", "sans-bold", 20);
		Color Grey = Color(182, 182, 182, 255);
		Button* Graphics = NanoUtility::button(_frontMenuSwitchButtons, "Graphics", {}, [this]() { this->_frontMenu->setVisible(false); this->_controlsScreen->setVisible(false); this->_interfaceScreen->setVisible(false); this->_audioScreen->setVisible(false); this->_graphicsScreen->setVisible(true); }, "comic-sans", 15, Color(Grey));
		Button* Controls = NanoUtility::button(_frontMenuSwitchButtons, "Controls", {}, [this]() { this->_frontMenu->setVisible(false); this->_controlsScreen->setVisible(true); this->_interfaceScreen->setVisible(false); this->_audioScreen->setVisible(false); this->_graphicsScreen->setVisible(false); }, "comic-sans", 15, Color(Grey));
		Button* Interface = NanoUtility::button(_frontMenuSwitchButtons, "Interface", {}, [this]() { this->_frontMenu->setVisible(false); this->_controlsScreen->setVisible(false); this->_interfaceScreen->setVisible(true); this->_audioScreen->setVisible(false); this->_graphicsScreen->setVisible(false); }, "comic-sans", 15, Color(Grey));
		Button* Audio = NanoUtility::button(_frontMenuSwitchButtons, "Audio", {}, [this]() { this->_frontMenu->setVisible(false); this->_controlsScreen->setVisible(false); this->_interfaceScreen->setVisible(false); this->_audioScreen->setVisible(true); this->_graphicsScreen->setVisible(false); }, "comic-sans", 15, Color(Grey));

		Graphics->setFlags(Button::RadioButton);
		Controls->setFlags(Button::RadioButton);
		Interface->setFlags(Button::RadioButton);
		Audio->setFlags(Button::RadioButton);

		Vector2i RadioBttnsize = Vector2i(50, 20);
		Graphics->setFixedSize(RadioBttnsize);
		Controls->setFixedSize(RadioBttnsize);
		Interface->setFixedSize(RadioBttnsize);
		Audio->setFixedSize(RadioBttnsize);
#pragma endregion
#pragma region Graphics Screen
		int RenderDistant;
		RenderDistant = std::stoi(settings.at("RenderDistant"));

		new Label(_graphicsScreen, "Graphics", "sans-bold", 20);
		new Label(_graphicsScreen, "Resolution", "sans", 20);
		new Label(_graphicsScreen, "", "sans", 5);

		ComboBox* CoBo = new ComboBox(_graphicsScreen, resolutions);
		CoBo->setFixedSize(Vector2i(100, 30));
		CoBo->setCallback([&, CoBo](int x) { settings.at("DisplaySize") = CoBo->items().at(x); });

		new Label(_graphicsScreen, "", "sans", 5);
		new Label(_graphicsScreen, "Render distant", "sans", 20);

		Widget* panel = new Widget(_graphicsScreen);
		panel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Slider* RenderSlider = new Slider(panel);
		RenderSlider->setValue(0.1f * RenderDistant);
		RenderSlider->setFixedWidth(100);

		TextBox* textBox = new TextBox(panel);
		textBox->setFixedSize(Vector2i(30, 25));
		textBox->setValue(std::to_string(RenderDistant));
		RenderSlider->setCallback([textBox](float value) { textBox->setValue(std::to_string((int)(value * 10))); });
		RenderSlider->setFinalCallback([&](float value) { settings.at("RenderDistant") = std::to_string((int)(value * 10)); });

		textBox->setFixedSize(Vector2i(65, 25));
		textBox->setFontSize(15);
		textBox->setAlignment(TextBox::Alignment::Left);

		new Label(_graphicsScreen, "Graphics Detail", "sans", 20);
		Widget* GraphicsSettins = new Widget(_graphicsScreen);
		GraphicsSettins->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Button* GraphLow = NanoUtility::button(GraphicsSettins, "Low", {}, [&]() {settings.at("GraphicsDetail") = "low"; }, "comic-sans", 17, Color(Grey));
		Button* GraphMed = NanoUtility::button(GraphicsSettins, "Medium", {}, [&]() {settings.at("GraphicsDetail") = "medium"; }, "comic-sans", 17, Color(Grey));
		Button* GraphHigh = NanoUtility::button(GraphicsSettins, "High", {}, [&]() {settings.at("GraphicsDetail") = "high"; }, "comic-sans", 17, Color(Grey));

		GraphLow->setFlags(Button::RadioButton);
		GraphMed->setFlags(Button::RadioButton);
		GraphHigh->setFlags(Button::RadioButton);

		Vector2i RadioBttnsizeGraphics = Vector2i(80, 50);
		GraphLow->setFixedSize(RadioBttnsizeGraphics);
		GraphMed->setFixedSize(RadioBttnsizeGraphics);
		GraphHigh->setFixedSize(RadioBttnsizeGraphics);



#pragma endregion
#pragma region Controls Screen
		new Label(_controlsScreen, "Controls", "sans-bold", 20);
		new Label(_controlsScreen, "", "sans", 5);
		new Label(_controlsScreen, "Movement: ", "sans", 20);
		new Label(_controlsScreen, "Forward:", "sans", 20);
		new Label(_controlsScreen, "Backward:", "sans", 20);
		new Label(_controlsScreen, "Left:", "sans", 20);
		new Label(_controlsScreen, "Right:", "sans", 20);

		/*
		Widget* FwdKey = new Widget(_controlsScreen);
		FwdKey->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		TextBox* textBox = new TextBox(FwdKey);
		textBox->setFixedSize(Vector2i(60, 25));
		textBox->setValue("Hier moet de key komen");
		glfwSetCharCallback->setCallback([textBox](float value) { // iets zoals dit?
			textBox->setValue(std::to_string((int)(value * 10)));
			});
		textBox->setFixedSize(Vector2i(30, 25));
		textBox->setFontSize(15);
		textBox->setAlignment(TextBox::Alignment::Left);
		dit hierboven moet ook voor de andere, maar eerst laten werken*/


#pragma endregion
#pragma region Interface Screen
		new Label(_interfaceScreen, "Interface", "sans-bold", 20);
		new Label(_interfaceScreen, "", "sans-bold", 5);
		new Label(_interfaceScreen, "HUD scale", "sans-bold", 20);
#pragma region HUD
		Widget* HudPanel = new Widget(_interfaceScreen);
		HudPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Slider* HudScaleSlider = new Slider(HudPanel);
		HudScaleSlider->setValue(0.5f);
		HudScaleSlider->setFixedWidth(100);

		TextBox* textBoxHUD = new TextBox(HudPanel);
		textBoxHUD->setFixedSize(Vector2i(30, 25));
		textBoxHUD->setValue("5");
		HudScaleSlider->setCallback([textBoxHUD](float value) {
			textBoxHUD->setValue(std::to_string((int)(value * 10)));
			});

		HudScaleSlider->setFinalCallback([&](float value) {
			cout << "HUD Scale: " << (int)(value * 10) << endl;
			});

		textBoxHUD->setFixedSize(Vector2i(65, 25));
		textBoxHUD->setFontSize(15);
		textBoxHUD->setAlignment(TextBox::Alignment::Left);
#pragma endregion
		new Label(_interfaceScreen, "Combat scale", "sans-bold", 20);
#pragma region Combat
		Widget* CombatPanel = new Widget(_interfaceScreen);
		CombatPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Slider* CombatScaleSlider = new Slider(CombatPanel);
		CombatScaleSlider->setValue(0.5f);
		CombatScaleSlider->setFixedWidth(100);

		TextBox* textBoxCombat = new TextBox(CombatPanel);
		textBoxCombat->setFixedSize(Vector2i(30, 25));
		textBoxCombat->setValue("5");
		CombatScaleSlider->setCallback([textBoxCombat](float value) {
			textBoxCombat->setValue(std::to_string((int)(value * 10)));
			});

		CombatScaleSlider->setFinalCallback([&](float value) {
			cout << "Combat things Scale: " << (int)(value * 10) << endl;
			});

		textBoxCombat->setFixedSize(Vector2i(65, 25));
		textBoxCombat->setFontSize(15);
		textBoxCombat->setAlignment(TextBox::Alignment::Left);
#pragma endregion
#pragma endregion
#pragma region Audio Screen
		int MasterVol;
		int FriendlyCreaturesVol;
		int HostileCreaturesVol;
		int MusicVol;
		int CombatVol;

		MasterVol = std::stoi(settings.at("MasterVol"));
		FriendlyCreaturesVol = std::stoi(settings.at("FriendlyCreaturesVol"));
		HostileCreaturesVol = std::stoi(settings.at("HostileCreaturesVol"));
		MusicVol = std::stoi(settings.at("MusicVol"));
		CombatVol = std::stoi(settings.at("CombatVol"));

		new Label(_audioScreen, "Audio", "sans-bold", 20);
		new Label(_audioScreen, "", "sans", 5);
		new Label(_audioScreen, "Master Volume", "sans", 20);
#pragma region Master Vol
		Widget* MasterVolPanel = new Widget(_audioScreen);
		MasterVolPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Slider* MasterVolSlider = new Slider(MasterVolPanel);
		MasterVolSlider->setValue(0.01f * MasterVol);
		MasterVolSlider->setFixedWidth(100);

		TextBox* testBoxMasterVol = new TextBox(MasterVolPanel);
		testBoxMasterVol->setFixedSize(Vector2i(40, 25));
		testBoxMasterVol->setValue(std::to_string(MasterVol));
		MasterVolSlider->setCallback([testBoxMasterVol](float value) { testBoxMasterVol->setValue(std::to_string((int)(value * 100))); });
		MasterVolSlider->setFinalCallback([&](float value) { settings.at("MasterVol") = std::to_string((int)(value * 100)); });

		testBoxMasterVol->setFixedSize(Vector2i(65, 25));
		testBoxMasterVol->setFontSize(15);
		testBoxMasterVol->setAlignment(TextBox::Alignment::Left);
#pragma endregion
		new Label(_audioScreen, "Music Volume", "sans", 20);
#pragma region Music Vol
		Widget* MusicVolPanel = new Widget(_audioScreen);
		MusicVolPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Slider* MusicVolSlider = new Slider(MusicVolPanel);
		MusicVolSlider->setValue(0.01f * MusicVol);
		MusicVolSlider->setFixedWidth(100);

		TextBox* testBoxMusicVol = new TextBox(MusicVolPanel);
		testBoxMusicVol->setFixedSize(Vector2i(40, 25));
		testBoxMusicVol->setValue(std::to_string(MusicVol));
		MusicVolSlider->setCallback([testBoxMusicVol](float value) { testBoxMusicVol->setValue(std::to_string((int)(value * 100))); });
		MusicVolSlider->setFinalCallback([&](float value) { settings.at("MusicVol") = std::to_string((int)(value * 100)); });

		testBoxMusicVol->setFixedSize(Vector2i(65, 25));
		testBoxMusicVol->setFontSize(15);
		testBoxMusicVol->setAlignment(TextBox::Alignment::Left);
#pragma endregion
		new Label(_audioScreen, "Friendly Creature Volume", "sans", 20);
#pragma region Friendly creatues Vol
		Widget* FriendlyVolPanel = new Widget(_audioScreen);
		FriendlyVolPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Slider* FriendlyVolSlider = new Slider(FriendlyVolPanel);
		FriendlyVolSlider->setValue(0.01f * FriendlyCreaturesVol);
		FriendlyVolSlider->setFixedWidth(100);

		TextBox* testBoxFriendlyVol = new TextBox(FriendlyVolPanel);
		testBoxFriendlyVol->setFixedSize(Vector2i(40, 25));
		testBoxFriendlyVol->setValue(std::to_string(FriendlyCreaturesVol));
		FriendlyVolSlider->setCallback([testBoxFriendlyVol](float value) { testBoxFriendlyVol->setValue(std::to_string((int)(value * 100))); });
		FriendlyVolSlider->setFinalCallback([&](float value) { settings.at("FriendlyCreaturesVol") = std::to_string((int)(value * 100)); });

		testBoxFriendlyVol->setFixedSize(Vector2i(65, 25));
		testBoxFriendlyVol->setFontSize(15);
		testBoxFriendlyVol->setAlignment(TextBox::Alignment::Left);
#pragma endregion
		new Label(_audioScreen, "Hostile Creature Volume", "sans", 20);
#pragma region Hostile creatues Vol
		Widget* HostileVolPanel = new Widget(_audioScreen);
		HostileVolPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Slider* HostileVolSlider = new Slider(HostileVolPanel);
		HostileVolSlider->setValue(0.01f * HostileCreaturesVol);
		HostileVolSlider->setFixedWidth(100);

		TextBox* testBoxHostileVol = new TextBox(HostileVolPanel);
		testBoxHostileVol->setFixedSize(Vector2i(40, 25));
		testBoxHostileVol->setValue(std::to_string(HostileCreaturesVol));
		HostileVolSlider->setCallback([testBoxHostileVol](float value) { testBoxHostileVol->setValue(std::to_string((int)(value * 100))); });
		HostileVolSlider->setFinalCallback([&](float value) { settings.at("HostileCreaturesVol") = std::to_string((int)(value * 100)); });

		testBoxHostileVol->setFixedSize(Vector2i(65, 25));
		testBoxHostileVol->setFontSize(15);
		testBoxHostileVol->setAlignment(TextBox::Alignment::Left);
#pragma endregion
		new Label(_audioScreen, "Combat Volume", "sans", 20);
#pragma region Combat Vol
		Widget* CombatVolPanel = new Widget(_audioScreen);
		CombatVolPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Slider* CombatVolSlider = new Slider(CombatVolPanel);
		CombatVolSlider->setValue(0.01f * CombatVol);
		CombatVolSlider->setFixedWidth(100);

		TextBox* testBoxCombatVol = new TextBox(CombatVolPanel);
		testBoxCombatVol->setFixedSize(Vector2i(40, 25));
		testBoxCombatVol->setValue(std::to_string(CombatVol));
		CombatVolSlider->setCallback([testBoxCombatVol](float value) { testBoxCombatVol->setValue(std::to_string((int)(value * 100))); });
		CombatVolSlider->setFinalCallback([&](float value) { settings.at("CombatVol") = std::to_string((int)(value * 100)); });

		testBoxCombatVol->setFixedSize(Vector2i(65, 25));
		testBoxCombatVol->setFontSize(15);
		testBoxCombatVol->setAlignment(TextBox::Alignment::Left);
#pragma endregion
#pragma endregion

		_backButton = new Window(_screen, "");
		_backButton->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill));
		_backButton->setFixedSize(Vector2i(50, 50));
		_backButton->setPosition(Vector2i(width - 100, 50));

		Vector2i btnSize = Vector2i(50, 50);
		Button* back = NanoUtility::button(_backButton, "X", {}, [this]() { this->ShowContent(false); this->_startScreen->ShowContent(true); }, "comic-sans", 32, Color(Grey));
		///Button* quit = NanoUtility::button(_backButton, "Quit", {}, [this, window]() {  glfwSetWindowShouldClose(window, GL_TRUE); }, "comic-sans", 32, Color(Grey));
		back->setFixedSize(btnSize);

		ShowContent(false);
		realignWindows(width, height);
		_screen->setVisible(true);
		_screen->performLayout();
		_screenImage->setVisible(true);
		_screenImage->performLayout();

		NanoUtility::applyCustomTheme1(_frontMenuSwitchButtons);
	}
	/*
	Render the Settingsscreen
	*/
	void render() {
		_screenImage->drawContents();
		_screenImage->drawWidgets();
		_screen->drawContents();
		_screen->drawWidgets();
	}

	void ShowContent(bool show = true) {
		_backgroundImage->setVisible(show);
		_backMenu->setVisible(show);
		_frontMenu->setVisible(show);
		_backButton->setVisible(show);
		_frontMenuMiddle->setVisible(show);
		_frontMenuSwitchButtons->setVisible(show);
		_graphicsScreen->setVisible(false);
		_controlsScreen->setVisible(false);
		_interfaceScreen->setVisible(false);
		_audioScreen->setVisible(false);
	}

	/*
	Update Parent Classes
	*/
	void UpdateParentClasses(BaseScreen* startScreen) {
		_startScreen = startScreen;
	}

	/*
	Realign the Settings too the center of the window.
	*/
	void realignWindows(int width, int height) {
		Vector2i imgSizeWidget = _backgroundImage->size();
		_backgroundImage->setPosition({ width / 2 - imgSizeWidget.x() / 2, height / 2 - imgSizeWidget.y() / 2 });

		Vector2i menuSizeWidget = Vector2i(_frontMenu->size().x() + 10, _backMenu->size().y() + 25);
		///_backButton->setPosition({ 10, height - 40 });

	}

	Screen* getScreen() { return this->_screen; }

private:
	bool isActive = true;

	imagesDataType mImagesDataSettingsScreen;

	Screen* _screen;
	Screen* _screenImage; //This screen will not have any mouse feedback (User cant Interacti with it)

	Window* _backgroundImage;
	Window* _backMenu;
	Window* _frontMenu;
	Window* _backButton;
	Window* _frontMenuMiddle;
	Window* _frontMenuSwitchButtons;
	Window* _graphicsScreen;
	Window* _controlsScreen;
	Window* _interfaceScreen;
	Window* _audioScreen;

	BaseScreen* _startScreen = nullptr;

	/*
	_graphicsScreen
	_controlsScreen
	_interfaceScreen
	_audioScreen
	*/
};