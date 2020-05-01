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

#include <map>
#include <iostream>
#include <cstdint>

using imagesDataType = vector<pair<GLTexture, GLTexture::handleType>>;

class NanoUtility {
public:
	static std::vector<std::string> Split(const std::string& str, int splitLength)
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

	static GLCanvas* line(Window* window, Color color = Color(46, 46, 50, 255), Vector2i size = Vector2i(206, 1)) {
		GLCanvas* lineCanvas = new GLCanvas(window);
		lineCanvas->setBackgroundColor(color);
		lineCanvas->setFixedSize(size);
		return lineCanvas;
	}
	static Label* title(Window* window, string title, string font = "sans-bold", int size = 16, Color color = Color(255, 255, 255, 255)) {
		Label* titleLabel = new Label(window, title, font, size);
		titleLabel->setColor(color);
		return titleLabel;
	}
	static vector<Label*> setDesc(Window* window, string text, string font = "sans-bold", int size = 16, Color color = Color(92, 92, 92, 255), int CharsPerRow = 36) {
		vector<Label*> labels;
		for (string line : Split(text, CharsPerRow)) {
			Label* l = new Label(window, line, font, size);
			l->setColor(color);
			labels.push_back(l);
		}

		return labels;
	}
	static Button* iconText(Window* window, string title, int ICON, string font = "sans-bold", int size = 16, Color color = Color(92, 92, 92, 255), Vector2i btnSize = { 20, 10 }) {
		Button* iconText = new Button(window);
		iconText->setIcon(ICON);
		iconText->setTextColor(color);
		iconText->setCaption(title);
		iconText->setBackgroundColor(Color(0, 0, 0, 0));
		iconText->setFontSize(size);

		return iconText;
	}
	static GLCanvas* emptySpace(Window* window, int height = 5, Color color = Color(255, 255, 255, 255)) {
		GLCanvas* lineCanvas = new GLCanvas(window);
		lineCanvas->setBackgroundColor(color);
		lineCanvas->setFixedSize({ 1, height });
		return lineCanvas;
	}
	static Label* textAtPos(Window* window, string title, Vector2i pos, string font = "sans-bold", int size = 16, Color color = Color(255, 255, 255, 255)) {
		Label* titleLabel = new Label(window, title, font, size);
		titleLabel->setColor(color);
		return titleLabel;
	}
	static Button* button(Window* window, string title, Vector2i btnSize, const std::function<void()> &callback, string font = "sans-bold", int size = 16, Color color = Color(92, 92, 92, 255)) {
		Button* button = new Button(window);
		button->setTextColor(color);
		button->setSize(btnSize);
		button->setCaption(title);
		button->setFontSize(size);
		button->setCallback(callback);
		return button;
	}

	static int LoadImage(string texturePath, imagesDataType& mImagesData) {
		GLTexture texture(texturePath);
		auto data = texture.load("resources/" + texturePath + ".png");
		mImagesData.emplace_back(std::move(texture), std::move(data));

		return mImagesData.size() - 1;
	}

	static void applyCustomTheme(Window* window) {
		//*
		window->theme()->mTransparent = Color(29, 0);
		window->theme()->mWindowFillUnfocused = Color(255, 0);
		window->theme()->mWindowFillFocused = Color(255, 0);
		window->theme()->mBorderMedium = Color(255, 0);
		window->theme()->mBorderDark = Color(255, 0);
		window->theme()->mBorderLight = Color(255, 0);
		window->theme()->mDropShadow = Color(255, 0);

		window->theme()->mButtonCornerRadius = 5;

		window->theme()->mButtonGradientTopUnfocused = Color(99, 54, 11, 225);
		window->theme()->mButtonGradientBotUnfocused = Color(99, 54, 11, 225);

		window->theme()->mButtonGradientTopFocused = Color(82, 74, 102, 255);
		window->theme()->mButtonGradientBotFocused = Color(82, 74, 102, 255);

		window->theme()->mButtonGradientTopPushed = Color(255, 255, 255, 255);
		window->theme()->mButtonGradientBotPushed = Color(255, 255, 255, 255);
		//*/
	}
};