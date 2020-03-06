#pragma once
#include "System.h"
#include "ShaderLoader.h"
#include "Components.h"
#include "ComponentSystemManager.h"

class RenderSystem : public System
{
public:
	void Init();
	void Update(ShaderLoader* shader);
	void Update(ShaderLoader* shader, bool RenderBoundingBoxes);
};