#pragma once
#include "InputSystem.h"

void InputSystem::Init()
{
}

extern ComponentSystemManager csm;
void InputSystem::Update(bool keys[1024], std::unordered_map<string, string> settings)
{
	for (auto const& entity : mEntities)
	{
		auto& motionC = csm.GetComponent<MotionC>(entity);

		if (keys[std::stoi(settings.at("Forward"))]) motionC.direction = FORWARD;
		if (keys[std::stoi(settings.at("Backwards"))]) motionC.direction = BACKWARD;
		if (keys[std::stoi(settings.at("Left"))]) motionC.direction = LEFT;
		if (keys[std::stoi(settings.at("Right"))]) motionC.direction = RIGHT;
		
		//Set Direction to NONE of no keys are pressed
		if(!keys[std::stoi(settings.at("Forward"))] && !keys[std::stoi(settings.at("Left"))] && !keys[std::stoi(settings.at("Right"))] && !keys[std::stoi(settings.at("Backwards"))]) motionC.direction = NONE;
	}
}