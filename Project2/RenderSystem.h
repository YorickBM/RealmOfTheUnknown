#pragma once
#include "System.h"
#include "Shader.h"
#include "Components.h"
#include "ComponentSystemManager.h"

class RenderSystem : public System
{
public:
	void Init();
	void Update(Shader shader);
	void Update(Shader shader, bool RenderBoundingBoxes);
};