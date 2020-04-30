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

using namespace nanogui;
using imagesDataType = vector<pair<GLTexture, GLTexture::handleType>>;
imagesDataType mImagesDataStartScreen;

class SettingsScreen {
public:
	SettingsScreen(Screen* screen, int width, int height, GLFWwindow* window, Screen* backgroundImageScreen) {
		_screen = screen;
		_screenImage = backgroundImageScreen;

		_backgroundImage = new Window(_screenImage, "");

		//SetupLayout
		_backgroundImage->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));

		//Render Background Image
		ImageView* backgroundImg = new ImageView(_backgroundImage, mImagesDataStartScreen[NanoUtility::LoadImage("MineRender5", mImagesDataStartScreen)].first.texture());
		_backgroundImage->center();

		//Setting Background Screen
		_backMenu = new Window(_screenImage, "");
		
		GLCanvas* topCanvas = new GLCanvas(_backMenu);
		topCanvas->setBackgroundColor({ 46, 48, 52, 255 });
		topCanvas->setFixedSize({ 236, 230 });
		/* topCanvas->setPosition({ , }) */

		//Settings FrontMenu Screen
		_frontMenu = new Window(_screen);
		_frontMenu->setLayout(new BoxLayout(Orientation::Horizontal))
	}

	Screen* getScreen() { return this->_screen; }

private:
	int frame = 10;
	bool isActive = true;
	Screen* _screen;
	Screen* _screenImage; //This screen will not have any mouse feedback (User cant Interacti with it)

	Window* _backgroundImage;
	Window* _backMenu;
	Window* _frontMenu;
};