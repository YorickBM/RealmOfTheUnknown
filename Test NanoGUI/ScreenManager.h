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
	StartScreen(Screen* screen, int width, int height, GLFWwindow* window) {
		_screen = screen;
		_backgroundImage = new Window(_screen, "");


		//Init BackgroundImage
		GLTexture texture("backgroundSlot");
		auto data = texture.load("resources/MineRender5.png");
		mImagesDataStartScreen.emplace_back(std::move(texture), std::move(data));

		//SetupLayout
		_backgroundImage->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
		_backgroundImage->setFixedSize({ width,height });

		//Render Background Image
		ImageView* backgroundImg = new ImageView(_backgroundImage, mImagesDataStartScreen[0].first.texture());
		_backgroundImage->center();

		//SetupLayout
		_selectionMenu = new Window(_screen, "");
		_selectionMenu->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));

		NanoUtility::button(_selectionMenu, "Start Game", {}, [this]() { this->closeStartScreen(); });
		NanoUtility::emptySpace(_selectionMenu);
		NanoUtility::button(_selectionMenu, "Settings Menu", {}, [this]() {});
		NanoUtility::emptySpace(_selectionMenu);
		NanoUtility::button(_selectionMenu, "Exit", {}, [this, window]() { glfwSetWindowShouldClose(window, GL_TRUE); });

		_selectionMenu->center();

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

	/*
	 Realign the inventory too the center of the window.
	 */
	void realignWindows(int width, int height) {
		_backgroundImage->center();
		_selectionMenu->center();
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
		int length = _loadingText->caption().size();
		std::cout << length << std::endl;
		_selectionMenu->setPosition({ 240, height - 40 });
		_backgroundImage->center();
		render();

		glfwSwapBuffers(window);
	}

	/*
	 Realign the inventory too the center of the window.
	 */
	void realignWindows(int width, int height) {
		_backgroundImage->center();
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