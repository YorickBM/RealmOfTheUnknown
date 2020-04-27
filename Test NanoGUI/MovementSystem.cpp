#include "MovementSystem.h"

void MovementSystem::Init()
{
}

extern ComponentSystemManager csm;
void MovementSystem::Update(float deltatime, Camera& camera)
{
	for (auto const& entity : mEntities)
	{
		auto& motionC = csm.GetComponent<MotionC>(entity);
		auto& transformC = csm.GetComponent<TransformC>(entity);

		transformC.position = camera.ProcessKeyboard(motionC.direction, motionC.movementSpeed, deltatime, transformC.position);

		//Update Camera to Position cuz its the only thing we be moving atm (Seperate System Maybe?)
		///camera.SetPosition(transformC.position);
	}
}
