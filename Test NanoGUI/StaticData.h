#pragma once
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class StaticData {
	static ClassTypeE ClassType;
	static glm::vec3 CamPosition;
};

enum ClassTypeE {
	Warrior,
	Hunter,
	Archer,
	Wizard
};

enum E {

};