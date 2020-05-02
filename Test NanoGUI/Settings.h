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
		
		//Labels/Buttons Settings
		new Label(_frontMenuMiddle, "------ Settings ------", "sans-bold", 30);

		new Label(_frontMenu, "Settings", "sans-bold", 20);

		#pragma region Graphics Screen
		new Label(_graphicsScreen, "Graphics", "sans-bold", 20);
		new Label(_graphicsScreen, "Resolution", "sans", 20);
		new Label(_graphicsScreen, "", "sans", 5);

		ComboBox* CoBo = new ComboBox(_graphicsScreen, { "640x360", "800x600", "1024x768", "1280x720", "1280x800", "1280x1024"});
		CoBo->setFixedSize(Vector2i(100, 30));

		new Label(_graphicsScreen, "", "sans", 5);
		new Label(_graphicsScreen, "Render distant", "sans", 20);

		Widget* panel = new Widget(_graphicsScreen);
		panel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 20));
		Slider* RenderSlider = new Slider(panel);
		RenderSlider->setValue(0.5f);
		RenderSlider->setFixedWidth(100);

		TextBox* textBox = new TextBox(panel);
		textBox->setFixedSize(Vector2i(60, 25));
		textBox->setValue("5");
		textBox->setUnits("Chunks");
		RenderSlider->setCallback([textBox](float value) {
			textBox->setValue(std::to_string((int)(value * 10)));
			});

		RenderSlider->setFinalCallback([&](float value) {
			cout << "Render Distance: " << (int)(value * 10) << endl;
			});

		textBox->setFixedSize(Vector2i(65, 25));
		textBox->setFontSize(15);
		textBox->setAlignment(TextBox::Alignment::Left);

		#pragma endregion
		#pragma region Controls Screen
		new Label(_controlsScreen, "Controls", "sans-bold", 20);
		#pragma endregion
		#pragma region Interface Screen
		new Label(_interfaceScreen, "Interface", "sans-bold", 20);
		#pragma endregion
		#pragma region Audio Screen
		new Label(_audioScreen, "Audio", "sans-bold", 20);
		#pragma endregion

		Button* Graphics = NanoUtility::button(_frontMenuSwitchButtons, "Graphics", {}, [this]() { this->_frontMenu->setVisible(false);this->_controlsScreen->setVisible(false); this->_interfaceScreen->setVisible(false); this->_audioScreen->setVisible(false); this->_graphicsScreen->setVisible(true); }, "comic-sans", 15, Color(255, 255, 255, 255));
		Button* Controls = NanoUtility::button(_frontMenuSwitchButtons, "Controls", {}, [this]() { this->_frontMenu->setVisible(false);this->_controlsScreen->setVisible(true); this->_interfaceScreen->setVisible(false); this->_audioScreen->setVisible(false); this->_graphicsScreen->setVisible(false);}, "comic-sans", 15, Color(255, 255, 255, 255));
		Button* Interface = NanoUtility::button(_frontMenuSwitchButtons, "Interface", {}, [this]() { this->_frontMenu->setVisible(false);this->_controlsScreen->setVisible(false); this->_interfaceScreen->setVisible(true); this->_audioScreen->setVisible(false); this->_graphicsScreen->setVisible(false);}, "comic-sans", 15, Color(255, 255, 255, 255));
		Button* Audio = NanoUtility::button(_frontMenuSwitchButtons, "Audio", {}, [this]() { this->_frontMenu->setVisible(false);this->_controlsScreen->setVisible(false); this->_interfaceScreen->setVisible(false); this->_audioScreen->setVisible(true); this->_graphicsScreen->setVisible(false);}, "comic-sans", 15, Color(255, 255, 255, 255));
		
		Graphics->setFlags(Button::RadioButton);
		Controls->setFlags(Button::RadioButton);
		Interface->setFlags(Button::RadioButton);
		Audio->setFlags(Button::RadioButton);

		Vector2i RadioBttnsize = Vector2i(50, 20);
		Graphics->setFixedSize(RadioBttnsize);
		Controls->setFixedSize(RadioBttnsize);
		Interface->setFixedSize(RadioBttnsize);
		Audio->setFixedSize(RadioBttnsize);


		_backButton = new Window(_screen, "");
		_backButton->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, -1));

		Vector2i btnSize = Vector2i(90, 40);
		Button* back = NanoUtility::button(_backButton, "Back", {}, [this]() { this->ShowContent(false); this->_startScreen->ShowContent(true); }, "comic-sans", 32, Color(255, 255, 255, 255));
		Button* quit = NanoUtility::button(_backButton, "Quit", {}, [this, window]() {  glfwSetWindowShouldClose(window, GL_TRUE); }, "comic-sans", 32, Color(255, 255, 255, 255));
		back->setFixedSize(btnSize);
		quit->setFixedSize(btnSize);

		ShowContent(false);
		realignWindows(width, height);
		_screen->setVisible(true);
		_screen->performLayout();
		_screenImage->setVisible(true);
		_screenImage->performLayout();
		_backButton->setPosition({ 10, height - 40 });

		NanoUtility::applyCustomTheme(_backButton);
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
		///_frontMenu->center();
		///_frontMenu->setSize({ width, height });
		///_backMenu->center();
		_backButton->setPosition({ 10, height - 40 });

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