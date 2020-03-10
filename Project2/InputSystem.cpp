#include "InputSystem.h"

void InputSystem::Init()
{
}

extern ComponentSystemManager csm;
void InputSystem::Update(bool keys[1024])
{
	for (auto const& entity : mEntities)
	{
		auto& motionC = csm.GetComponent<MotionC>(entity);

		if (keys[GLFW_KEY_W]) motionC.direction = FORWARD;
		if (keys[GLFW_KEY_S]) motionC.direction = BACKWARD;
		if (keys[GLFW_KEY_A]) motionC.direction = LEFT;
		if (keys[GLFW_KEY_D]) motionC.direction = RIGHT;
		
		//Set Direction to NONE of no keys are pressed
		if(!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S] && !keys[GLFW_KEY_A] && !keys[GLFW_KEY_D]) motionC.direction = NONE;
	}
}