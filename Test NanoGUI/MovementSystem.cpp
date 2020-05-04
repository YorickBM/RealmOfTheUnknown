#pragma once
#include "MovementSystem.h"
#include "CollisionUtility.h"

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
		
		if (motionC.isCamera) {
			camera.SetPrevPosition(camera.GetPosition());
			camera.SetPosition(camera.ProcessKeyboard(motionC.direction, motionC.movementSpeed, deltatime, camera.GetPosition()));
		}
	}
}
