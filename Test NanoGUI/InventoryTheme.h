#pragma once
#include <nanogui/theme.h>
#include <nanogui/entypo.h> // the default icons font

class CustomTheme : public nanogui::Theme {
public:
    CustomTheme(NVGcontext* ctx) : nanogui::Theme(ctx) {
        using nanogui::Color;
        // ... change any colors you want ...
        mBorderDark = Color(111, 255);
        // ... change any default icons you want ...
        // default: ``ENTYPO_ICON_CHECK``
        mCheckBoxIcon = ENTYPO_ICON_CROSS;
    }
};