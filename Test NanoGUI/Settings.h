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

		//Setting Background Screen
		_backMenu = new Window(_screenImage, "");
		
		GLCanvas* topCanvas = new GLCanvas(_backMenu);
		topCanvas->setBackgroundColor({ 46, 48, 52, 255 });
		topCanvas->setFixedSize({ 236, 230 });
		/* topCanvas->setPosition({ , }) */

		//Settings FrontMenu Screen
		_frontMenu = new Window(_screen, "");
		_frontMenu->setLayout(new BoxLayout(Orientation::Horizontal));

		
		_backButton = new Window(_screen, "");


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

		NanoUtility::applyCustomTheme(_frontMenu);
		}
	/*
	Render the Settingsscreen
	*/
	void render() {
		_screenImage->drawContents();
		_screenImage->drawWidgets();
		_screen->drawContents();
		_screen->drawWidgets();

		frame--;
	}

	void ShowContent(bool show = true) {
		_backgroundImage->setVisible(show);
		_backMenu->setVisible(show);
		_frontMenu->setVisible(show);
		_backButton->setVisible(show);
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

		Vector2i menuSizeWidget = _frontMenu->size();
		_frontMenu->setPosition({ width / 2 - menuSizeWidget.x() / 2, height / 2 - menuSizeWidget.y() / 2 });

		_backMenu->setPosition({ width / 2 - menuSizeWidget.x() / 2, height / 2 - menuSizeWidget.y() / 2 });

		_backButton->setPosition({ 10, height - 40 });

	}

	Screen* getScreen() { return this->_screen; }

private:
	int frame = 10;
	bool isActive = true;

	imagesDataType mImagesDataSettingsScreen;

	Screen* _screen;
	Screen* _screenImage; //This screen will not have any mouse feedback (User cant Interacti with it)

	Window* _backgroundImage;
	Window* _backMenu;
	Window* _frontMenu;
	Window* _backButton;

	BaseScreen* _startScreen = nullptr;
};