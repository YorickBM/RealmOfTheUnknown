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

class BaseScreen {
public:
	virtual void ShowContent(bool show = true) = 0;
	virtual void render() = 0;
	virtual void realignWindows(int width, int height) = 0;
	virtual Screen* getScreen() = 0;
};

class StartScreen : public BaseScreen {
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
		NanoUtility::button(_selectionMenu, "Select Class", {}, [this]() { this->ShowContent(false); this->_classSelector->ShowContent(true); }, "comic-sans", 42, Color(255, 255, 255, 255))->setFixedSize(btnSize);
		NanoUtility::title(_selectionMenu, " ", "sans-bold", 5);
		NanoUtility::button(_selectionMenu, "Settings Menu", {}, [this]() { }, "comic-sans", 42, Color(255, 255, 255, 255))->setFixedSize(btnSize);
		NanoUtility::title(_selectionMenu, " ", "sans-bold", 5);
		NanoUtility::button(_selectionMenu, "Quit Game", {}, [this, window]() { glfwSetWindowShouldClose(window, GL_TRUE); }, "comic-sans", 42, Color(255, 255, 255, 255))->setFixedSize(btnSize);

		_selectionMenu->center();
		NanoUtility::applyCustomTheme(_selectionMenu);
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
	Update Parent Classes
	*/
	void UpdateParentClasses(BaseScreen* classSelector) {
		_classSelector = classSelector;
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
	void ShowContent(bool show = true) {
		_selectionMenu->setVisible(show);
		_backgroundImage->setVisible(show);
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

	BaseScreen* _classSelector = nullptr;
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

		NanoUtility::applyCustomTheme(_selectionMenu);

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

class ClassSelector : public BaseScreen {
public:
	ClassSelector(Screen* screen, int width, int height, GLFWwindow* window, Screen* backgroundImageScreen) {
		_screen = screen;
		_screenImage = backgroundImageScreen;
		_screenExtra = new Screen();
		_screenExtra->initialize(window, false);

		_backgroundImage = new Window(_screenImage, "");

		//SetupLayout
		_backgroundImage->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));

		//Render Background Image
		ImageView* backgroundImg = new ImageView(_backgroundImage, mImagesDataStartScreen[NanoUtility::LoadImage("MineRender5", mImagesDataStartScreen)].first.texture());
		_backgroundImage->center();

		//SetupLayout
		_backButton = new Window(_screen, "");
		_backButton->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, -1));

		Vector2i btnSize = Vector2i(90, 40);
		Button* back = NanoUtility::button(_backButton, "Back", {}, [this]() { this->ShowContent(false); this->_startScreen->ShowContent(true); }, "comic-sans", 32, Color(255, 255, 255, 255));
		Button* quit = NanoUtility::button(_backButton, "Quit", {}, [this, window]() {  glfwSetWindowShouldClose(window, GL_TRUE); }, "comic-sans", 32, Color(255, 255, 255, 255));
		back->setFixedSize(btnSize);
		quit->setFixedSize(btnSize);

		_backButton->setPosition({ 0, height - 42 });
		NanoUtility::applyCustomTheme(_backButton);

		_selectionMenu = new Window(_screenExtra, "");
		_selectionMenu->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, -1));
		applyCustomTheme2(_selectionMenu);

		_buttons = vector<Button*>();
		Button* Warrior = NanoUtility::button(_selectionMenu, "Warrior", { 90, 40 }, [&]() { /*for(Button* btn : _buttons) btn->setTextColor({ 89, 91, 91, 255 }); _buttons[0]->setTextColor({ 214, 171, 63, 255 }); */}, "comic-sans", 32, Color(255, 255, 255, 255));
		Warrior->setTextColor({ 214, 171, 63, 255 }); //Selected Color
		_buttons.push_back(Warrior);
		Button* Hunter = NanoUtility::button(_selectionMenu, "Hunter", { 90, 40 }, [&]() {/*for (Button* btn : _buttons) btn->setTextColor({ 89, 91, 91, 255 }); _buttons[1]->setTextColor({ 214, 171, 63, 255 }); */}, "comic-sans", 32, Color(255, 255, 255, 255));
		Hunter->setTextColor({ 126, 25, 27, 255 }); //Disabled Color
		_buttons.push_back(Hunter);
		Button* Wizard = NanoUtility::button(_selectionMenu, "Wizard", { 90, 40 }, [&]() {/*for (Button* btn : _buttons) btn->setTextColor({ 89, 91, 91, 255 }); _buttons[2]->setTextColor({ 214, 171, 63, 255 }); */}, "comic-sans", 32, Color(255, 255, 255, 255));
		Wizard->setTextColor({ 126, 25, 27, 255 }); //Disabled Color
		_buttons.push_back(Wizard);
		Button* Archer = NanoUtility::button(_selectionMenu, "Archer", { 90, 40 }, [&]() {/*for (Button* btn : _buttons) btn->setTextColor({ 89, 91, 91, 255 }); _buttons[3]->setTextColor({ 214, 171, 63, 255 }); */}, "comic-sans", 32, Color(255, 255, 255, 255));
		Archer->setTextColor({ 126, 25, 27, 255 }); //Disabled Color
		_buttons.push_back(Archer);

		
		_windowContext = new Window(_screenImage, "");
		_windowContext->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
		NanoUtility::applyCustomTheme(_windowContext);

		GLCanvas* topCanvas = new GLCanvas(_windowContext);
		topCanvas->setBackgroundColor({ 40, 40, 43, 255 });
		topCanvas->setFixedSize({ 376, 50 });

		GLCanvas* bottomCanvas = new GLCanvas(_windowContext);
		bottomCanvas->setBackgroundColor({ 46, 48, 52, 255 });
		bottomCanvas->setFixedSize({ 376, 228 });
		_windowContext->setSize(bottomCanvas->size());

		menuSizeWidget = Vector2i(376 + 10, 228 + 25);

		//Update Visibility
		_screen->setVisible(true);
		_screen->performLayout();
		_screenImage->setVisible(true);
		_screenImage->performLayout();
		_screenExtra->setVisible(true);
		_screenExtra->performLayout();
		ShowContent(false);
		realignWindows(width, height);
	}

	/*
	 Render the StartScreem
	 */
	void render() {
		_screenImage->drawContents();
		_screenImage->drawWidgets();
		_screen->drawContents();
		_screen->drawWidgets();
		_screenExtra->drawContents();
		_screenExtra->drawWidgets();

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

		_windowContext->setSize({ width, height });
		_windowContext->setPosition({ width / 2 - menuSizeWidget.x() / 2, height / 2 - menuSizeWidget.y() / 2 });
		
		_backButton->setPosition({0, height - 40});
	}

	/*
	Update Parent Classes
	*/
	void UpdateParentClasses(BaseScreen* startScreen) {
		_startScreen = startScreen;
	}

	/*
	*/
	void ShowContent(bool show = true) {
		_selectionMenu->setVisible(show);
		_backButton->setVisible(show);
		_backgroundImage->setVisible(show);
		_windowContext->setVisible(show);
	}

	Screen* getScreen() { return this->_screen; }
	Screen* getScreenOtherTheme() { return this->_screenExtra; }

private:
	int frame = 10;
	bool isActive = true;
	Screen* _screen;
	Screen* _screenImage;
	Screen* _screenExtra;

	Window* _backgroundImage;
	Window* _selectionMenu;
	Window* _backButton;
	Window* _windowContext;
	Vector2i menuSizeWidget;

	BaseScreen* _startScreen;
	vector<Button*> _buttons;

	void applyCustomTheme2(Window* window) {
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