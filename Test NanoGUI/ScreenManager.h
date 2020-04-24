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

class StartScreen {
public:
	StartScreen(Screen* screen, int width, int height, GLFWwindow* window, Screen* backgroundImageScreen) {
		_screen = screen;
		_screenImage = backgroundImageScreen;

		_backgroundImage = new Window(_screenImage, "");

		//SetupLayout
		_backgroundImage->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));

		//Render Background Image
		ImageView* backgroundImg = new ImageView(_backgroundImage, mImagesDataStartScreen[NanoUtility::LoadImage("MineRender5", mImagesDataStartScreen)].first.texture());
		_backgroundImage->center();

		//SetupLayout
		_selectionMenu = new Window(_screen, "");
		_selectionMenu->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));

		Vector2i btnSize = Vector2i( 320, 60 );
		NanoUtility::button(_selectionMenu, "Start Game", {}, [this]() { this->closeStartScreen(); }, "comic-sans", 42, Color(255, 255, 255, 255))->setFixedSize(btnSize);
		NanoUtility::title(_selectionMenu, " ", "sans-bold", 5);
		NanoUtility::button(_selectionMenu, "Settings Menu", {}, [this]() {}, "comic-sans", 42, Color(255, 255, 255, 255))->setFixedSize(btnSize);
		NanoUtility::title(_selectionMenu, " ", "sans-bold", 5);
		NanoUtility::button(_selectionMenu, "Quit Game", {}, [this, window]() { glfwSetWindowShouldClose(window, GL_TRUE); }, "comic-sans", 42, Color(255, 255, 255, 255))->setFixedSize(btnSize);

		_selectionMenu->center();
		applyCustomTheme(_selectionMenu);
		menuSizeWidget = _selectionMenu->size();

		_screen->setVisible(true);
		_screen->performLayout();
		_screenImage->setVisible(true);
		_screenImage->performLayout();
	}

	/*
	 Render the StartScreem
	 */
	void render() {
		_screenImage->drawContents();
		_screenImage->drawWidgets();
		_screen->drawContents();
		_screen->drawWidgets();

		frame--;
	}

	/*
	 Realign the inventory too the center of the window.
	 */
	void realignWindows(int width, int height) {
		Vector2i imgSizeWidget = _backgroundImage->size();
		_backgroundImage->setPosition({ width / 2 - imgSizeWidget.x() / 2, height / 2 - imgSizeWidget.y() / 2 });

		_selectionMenu->setSize({ width, height });
		_selectionMenu->setPosition({ width / 2 - menuSizeWidget.x() / 2, height / 2 - menuSizeWidget.y() / 2 });
	}
	
	/*
	*/
	void ShowStartScreenContent(bool show = true) {
		_selectionMenu->setVisible(show);
	}

	Screen* getScreen() { return this->_screen; }
	bool IsActive() { return this->isActive; }
	void closeStartScreen() { this->isActive = false; }

private:
	int frame = 10;
	bool isActive = true;
	Screen* _screen;
	Screen* _screenImage;

	Window* _backgroundImage;
	Window* _selectionMenu;
	Vector2i menuSizeWidget;

	void applyCustomTheme(Window* window) {
		//*
		window->theme()->mTransparent = Color(29, 0);
		window->theme()->mWindowFillUnfocused = Color(255, 0);
		window->theme()->mWindowFillFocused = Color(255, 0);
		window->theme()->mBorderMedium = Color(255, 0);
		window->theme()->mBorderDark = Color(255, 0);
		window->theme()->mBorderLight = Color(255, 0);
		window->theme()->mDropShadow = Color(255, 0);

		window->theme()->mButtonCornerRadius = 5;

		window->theme()->mButtonGradientTopUnfocused = Color(99,54,11,225);
		window->theme()->mButtonGradientBotUnfocused = Color(99, 54, 11, 225);

		window->theme()->mButtonGradientTopFocused = Color(82, 74, 102, 255);
		window->theme()->mButtonGradientBotFocused = Color(82, 74, 102, 255);

		window->theme()->mButtonGradientTopPushed = Color(255, 255, 255, 255);
		window->theme()->mButtonGradientBotPushed = Color(255, 255, 255, 255);
		//*/
	}
};

class LoadingScreen {
public:
	LoadingScreen(Screen* screen, int width, int height) {
		_screen = screen;
		_backgroundImage = new Window(_screen, "");

		//Init BackgroundImage
		GLTexture texture("backgroundSlot");
		auto data = texture.load("resources/ForestRender9.png");
		mImagesDataStartScreen.emplace_back(std::move(texture), std::move(data));

		//SetupLayout
		_backgroundImage->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));

		//Render Background Image
		ImageView* backgroundImg = new ImageView(_backgroundImage, mImagesDataStartScreen[1].first.texture());
		backgroundImg->center();
		_backgroundImage->center();

		//SetupLayout
		_selectionMenu = new Window(_screen, "");
		_selectionMenu->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));

		_loadingText = NanoUtility::title(_selectionMenu, "........................................................................................", "sans-bold", 24);
		Vector2i imgSizeWidget = _backgroundImage->size();
		_backgroundImage->setPosition({ width / 2 - imgSizeWidget.x() / 2, height / 2 - imgSizeWidget.y() / 2 });

		_selectionMenu->setPosition({ 240, height - 40 });

		applyCustomTheme(_selectionMenu);

		_screen->setVisible(true);
		_screen->performLayout();
	}

	/*
	 Render the StartScreem
	 */
	void render() {
		_screen->drawContents();
		_screen->drawWidgets();

		frame--;
	}

	void specialRender(GLFWwindow* window, string text, int width, int height) {
		#pragma region Frame & Poll Events & Clear Buffers/Color
		// Check and call events
		glfwPollEvents();

		//Clear Buffers & Color
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		#pragma endregion

		_loadingText->setCaption(text);
		render();
		glfwSwapBuffers(window);
	}

	/*
	 Realign the inventory too the center of the window.
	 */
	void realignWindows(int width, int height) {
		Vector2i imgSizeWidget = _backgroundImage->size();
		_backgroundImage->setPosition({ width / 2 - imgSizeWidget.x() / 2, height / 2 - imgSizeWidget.y() / 2 });

		_selectionMenu->setPosition({ 240, height - 40 });
	}

	/*
	*/
	void ShowStartScreenContent(bool show = true) {
		_selectionMenu->setVisible(show);
	}

	Screen* getScreen() { return this->_screen; }
	bool IsActive() { return this->isActive; }
	void closeStartScreen() { this->isActive = false; }

private:
	int frame = 10;
	bool isActive = true;
	Screen* _screen;

	Window* _backgroundImage;
	Window* _selectionMenu;

	Label* _loadingText;

	void applyCustomTheme(Window* window) {
		//*
		window->theme()->mTransparent = Color(29, 0);
		window->theme()->mWindowFillUnfocused = Color(255, 0);
		window->theme()->mWindowFillFocused = Color(255, 0);
		window->theme()->mBorderMedium = Color(255, 0);
		window->theme()->mBorderDark = Color(255, 0);
		window->theme()->mBorderLight = Color(255, 0);
		window->theme()->mDropShadow = Color(255, 0);
		window->theme()->mButtonFontSize = 16;
		window->theme()->mButtonGradientTopUnfocused = Color(255, 0);
		window->theme()->mButtonGradientBotUnfocused = Color(255, 0);
		window->theme()->mButtonGradientTopFocused = Color(255, 0);
		window->theme()->mButtonGradientBotFocused = Color(255, 0);
		window->theme()->mButtonGradientTopPushed = Color(255, 0);
		window->theme()->mButtonGradientBotPushed = Color(255, 0);
		//*/
	}
};