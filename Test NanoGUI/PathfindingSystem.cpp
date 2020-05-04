#pragma once
#include "PathfindingSystem.h"
#include "CollisionUtility.h"
#include "NanoUtility.h"

void PathfindingSystem::Init()
{
}

extern ComponentSystemManager csm;

void PathfindingSystem::Update(Camera& camera) {
	for (auto const& entity : mEntities) {

		auto transformC = csm.GetComponent<TransformC>(entity);
		auto& entityC = csm.GetComponent<EntityC>(entity);

		int randomX = NanoUtility::randomInt(1, 8);
		int randomZ = NanoUtility::randomInt(1, 8);
		int randomNegX = NanoUtility::randomInt(1, 12);
		int randomNegZ = NanoUtility::randomInt(1, 12);

		if (entityC.firstTime) {
			entityC.goal = vec2(transformC.position.x, transformC.position.z);
		}
		
		if(entityC.isHostile) {
			int randmInt = NanoUtility::randomInt(1, 200);
			float distance = CollisionUtility::distanceBetweenTwoPoints(transformC.position.x, transformC.position.z, camera.GetPosition().x, camera.GetPosition().z);

			if (distance < 8.f) {
				entityC.goal = vec2(camera.GetPosition().x, camera.GetPosition().z);
			} else if (randmInt == 2) {
				entityC.firstTime = false;
				entityC.goal = vec2(transformC.position.x + (randomX - randomNegX), transformC.position.z + (randomZ - randomNegZ));
			}
		}
		else {
			int randmInt = NanoUtility::randomInt(1, 500);
			if (randmInt == 2) {
				entityC.firstTime = false;
				entityC.goal = vec2(transformC.position.x + (randomX - randomNegX), transformC.position.z + (randomZ - randomNegZ));
			}
		}
	}
}

void PathfindingSystem::MoveToGoal(float deltatime) {
	for (auto const& entity : mEntities) {
		auto& transformC = csm.GetComponent<TransformC>(entity);
		auto entityC = csm.GetComponent<EntityC>(entity);
		glm::vec2 entityPosition = glm::vec2(transformC.position.x, transformC.position.z);

		if (entityC.goal != entityPosition) {
			glm::vec2 diffVec = entityC.goal - entityPosition;
			float speed = entityC.movementSpeed * deltatime;

			float xMovement = 0;
			float yMovement = 0;
			if (diffVec.x < 0) xMovement -= speed;
			else if (diffVec.x > 0) xMovement += speed;
			if (diffVec.y < 0) yMovement -= speed;
			else if (diffVec.y > 0) yMovement += speed;

			if (xMovement > diffVec.x) xMovement = diffVec.x;
			if (yMovement > diffVec.y) yMovement = diffVec.y;


			transformC.position += glm::vec3(xMovement, 0, yMovement);
		}
	}
}