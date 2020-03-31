#pragma once
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

using namespace nanogui;

class Inventory {
public:
	Inventory(Screen *screen, Vector2i position = Vector2i(15,15)) {
		_screen = screen;

		_infoBackgroundWindow = new Window(_screen, "");
		_infoContextTopWindow = new Window(_screen, "");
		_infoContextBottomWindow = new Window(_screen, "");

		setPosition(15, 16);
		#pragma region Info Background
		_infoBackgroundWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, -1));
		_infoBackgroundWindow->setFixedSize({ 236, 234 + 369 });

		GLCanvas* topCanvas = new GLCanvas(_infoBackgroundWindow);
		topCanvas->setBackgroundColor({ 46, 48, 52, 255 });
		topCanvas->setFixedSize({ 236, 234 });

		GLCanvas* bottomCanvas = new GLCanvas(_infoBackgroundWindow);
		bottomCanvas->setBackgroundColor({ 40, 40, 43, 255 });
		bottomCanvas->setFixedSize({ 236, 369 });
		#pragma endregion
		#pragma region Info Top Context
		_infoContextTopWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 15, 3));
		_infoContextTopWindow->setFixedSize({ 236, 234 });
		applyCustomTheme(_infoContextTopWindow);

		//Image View

		//Item Title
		Label* itemTitle = new Label(_infoContextTopWindow, "Lorem ipsum", "sans-bold");
		itemTitle->setFontSize(16);
		itemTitle->setColor(Color(255, 255, 255, 255));
		#pragma endregion
		#pragma region Info Bottom Context
		_infoContextBottomWindow->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 15, 3));
		_infoContextBottomWindow->setFixedSize({ 236, 369 });
		applyCustomTheme(_infoContextBottomWindow);

		//Item stats
		iconText(_infoContextBottomWindow, "Lv. Min: 5", ENTYPO_ICON_LAB_FLASK);
		line(_infoContextBottomWindow);
		iconText(_infoContextBottomWindow, "Req. Class: Mage", ENTYPO_ICON_NEWSLETTER);
		line(_infoContextBottomWindow);
		iconText(_infoContextBottomWindow, "Req. Quest: Rat Poison", ENTYPO_ICON_CLIPBOARD);

		//Item Desc
		emptySpace(_infoContextBottomWindow, 20, { 40, 40, 43, 255 });
		title(_infoContextBottomWindow, "DESCRIPTION");
		line(_infoContextBottomWindow);

		vector<Label*> desc = setDesc("Everything you place on the bearing will revolve. Bearings can also be connected to an engine or the driver's seat to be controlled in different ways.");
		
		#pragma endregion

		//Create Backpack

		_screen->setVisible(true);
		_screen->performLayout();
	}

	void render() {
		_screen->drawContents();
		_screen->drawWidgets();
	}

	void ShowInfo(bool show = true) {
		_infoBackgroundWindow->setVisible(show);
		_infoContextBottomWindow->setVisible(show);
		_infoContextTopWindow->setVisible(show);
	}
	
	Screen* getScreen() { return this->_screen; }

	void setPosition(int x, int y) {
		_position = Vector2i(x, y);

		_infoBackgroundWindow->setPosition(Vector2i(x, y));
		_infoContextTopWindow->setPosition(Vector2i(x, y));
		_infoContextBottomWindow->setPosition({ x, y + 234 });
	}

private:
	Screen* _screen;
	FormHelper* _gui;

	Window* _infoBackgroundWindow;
	Window* _infoContextTopWindow;
	Window* _infoContextBottomWindow;

	Window* _backpackWindow;

	Vector2i _position;

	std::vector<std::string> Split(const std::string& str, int splitLength)
	{
		int NumSubstrings = str.length() / splitLength;
		std::vector<std::string> ret;

		for (auto i = 0; i < NumSubstrings; i++)
		{
			ret.push_back(str.substr(i * splitLength, splitLength));
		}
		// If there are leftover characters, create a shorter item at the end.
		if (str.length() % splitLength != 0)
		{
			ret.push_back(str.substr(splitLength * NumSubstrings));
		}
		return ret;
	}

	GLCanvas* line(Window* window, Color color = Color(46, 46, 50, 255), Vector2i size = Vector2i(206, 1)) {
		GLCanvas* lineCanvas = new GLCanvas(window);
		lineCanvas->setBackgroundColor(color);
		lineCanvas->setFixedSize(size);
		return lineCanvas;
	}
	Label* title(Window* window, string title, string font = "sans-bold", int size = 16, Color color = Color(255, 255, 255, 255)) {
		Label* titleLabel = new Label(_infoContextBottomWindow, title, font, size);
		titleLabel->setColor(color);
		return titleLabel;
	}
	vector<Label*> setDesc(string text, string font = "sans-bold", int size = 16, Color color = Color(92, 92, 92, 255), int CharsPerRow = 36) {
		vector<Label*> labels;
		for (string line : Split(text, CharsPerRow)) {
			Label* l = new Label(_infoContextBottomWindow, line, font, size);
			l->setColor(color);
			labels.push_back(l);
		}

		return labels;
	}
	Button* iconText(Window* window, string title, int ICON, string font = "sans-bold", int size = 16, Color color = Color(92, 92, 92, 255)) {
		Button* iconText = new Button(window);
		iconText->setIcon(ICON);
		iconText->setTextColor(color);
		iconText->setCaption(title);
		iconText->setBackgroundColor(Color(0, 0, 0, 0));
		return iconText;
	}
	GLCanvas* emptySpace(Window* window, int height = 5, Color color = Color(255, 255, 255, 255)) {
		GLCanvas* lineCanvas = new GLCanvas(window);
		lineCanvas->setBackgroundColor(color);
		lineCanvas->setFixedSize({1, height});
		return lineCanvas;
	}
	Label* textAtPos(Window* window, string title, Vector2i pos, string font = "sans-bold", int size = 16, Color color = Color(255, 255, 255, 255)) {
		Label* titleLabel = new Label(_infoContextBottomWindow, title, font, size);
		titleLabel->setColor(color);
		return titleLabel;
	}

	void applyCustomTheme(Window* window) {
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
	}
};