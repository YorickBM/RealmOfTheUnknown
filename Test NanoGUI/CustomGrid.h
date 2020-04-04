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

class CustomGrid : public FormHelper {
public:
	CustomGrid(Screen *screen) {
		mScreen = screen;
	}

	Window* addGrid(Window* window, const std::vector<int>& cols = {}, const std::vector<int>& rows = {}, int margin = 0) {
		assert(mScreen);
		mWindow = window;
		mLayout = new AdvancedGridLayout(cols, rows, margin);
		mLayout->setMargin(margin);
		mLayout->setColStretch(2, 1);
		mWindow->setLayout(mLayout);
		mWindow->setVisible(true);
		return mWindow;
	}

	Button* addButton(const std::string& label, const std::function<void()>& cb) {
		Button* button = new Button(mWindow, label);
		button->setCallback(cb);
		button->setFixedHeight(25);
		if (mLayout->rowCount() > 0)
			mLayout->appendRow(mVariableSpacing);
		mLayout->appendRow(0);
		mLayout->setAnchor(button, AdvancedGridLayout::Anchor(1, mLayout->rowCount() - 1, 3, 1));
		return button;
	}
	Button* addButtonT(const std::string& label, const std::function<void()>& cb) {
		Button* button = new Button(mWindow, label);
		button->setCallback(cb);
		button->setFixedHeight(25);
		if (mLayout->rowCount() > 0)
			mLayout->appendRow(mVariableSpacing);
		mLayout->appendRow(0);
		mLayout->setAnchor(button, AdvancedGridLayout::Anchor(1, mLayout->rowCount() - 1, 3, 1));
		return button;
	}
};