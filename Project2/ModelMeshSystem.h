#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"

class ModelMeshSystem : public System
{
public:
	void Init();
	void Update(ShaderLoader* shaderLoader);
};