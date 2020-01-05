#pragma once
#include "System.h"
#include "Components.h"
#include "ComponentSystemManager.h"

#include "Camera.h"
#include <GLFW/glfw3.h>

class MovementSystem : public System
{
public:
	void Init();
	void Update(GLfloat deltaTime, bool keys[1024], std::set<Entity> SceneEntities, 
		Camera &camera);

private:
	bool MeshMode = false;
	GLfloat delay = 10;
};
