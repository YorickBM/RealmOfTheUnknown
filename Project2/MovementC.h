#pragma once
struct MovementC {
	int movementType = 1;

	glm::vec3 Front = glm::vec3(0, 0, 0);
	glm::vec3 Right = glm::vec3(0, 0, 0);
	GLfloat MovementSpeed = 0.f;
};

enum MovementType {
	None = 0, Keyboard = 1, Auto = 2
};