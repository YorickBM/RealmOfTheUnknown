#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Components.h"
#include "ComponentSystemManager.h"

class InputSystem : public System
{
public:
	void Init();
	void Update(bool keys[1024], std::unordered_map<string, string> settings);
};