#include "MovementSystem.h"

extern ComponentSystemManager csm;

void MovementSystem::Init()
{
}

void MovementSystem::Update(GLfloat deltaTime, bool keys[1024], std::set<Entity> SceneEntities, Camera &camera)
{
	if (delay > 0) delay--;
	for (auto const& entity : mEntities)
	{
		auto movementType = csm.GetComponent<MovementC>(entity).movementType;
		auto isColliding = csm.GetComponent<CollisionC>(entity).isColliding;

		switch (movementType) {
		case MovementType::Keyboard:
			if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
			{
				camera.ProcessKeyboard(FORWARD, deltaTime, isColliding);
			}
			if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
			{
				camera.ProcessKeyboard(BACKWARD, deltaTime, isColliding);
			}
			if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
			{
				camera.ProcessKeyboard(LEFT, deltaTime, isColliding);
			}
			if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
			{
				camera.ProcessKeyboard(RIGHT, deltaTime, isColliding);
			}
			#pragma region Polygon Mode Toggle
			if (keys[GLFW_KEY_TAB]) {
				if (!MeshMode && delay == 0) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					MeshMode = true; delay = 10;
					return;
				}
				else if (delay == 0) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					MeshMode = false; delay = 10;
					return;
				}
			}
			#pragma endregion
			break;

		case MovementType::Auto:
			break;

		default:
			break;
		}
	}
}
