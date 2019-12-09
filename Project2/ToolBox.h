#pragma once
#include <iostream>
#include <map>

#include "Model.h"
#include <glm/glm.hpp>

class HashMap {
	std::map<glm::vec3 *, Model *> htmap;

public:
	void put(glm::vec3 *key, Model *value) {
		htmap[key] = value;
	}

	const void *get(glm::vec3 *key) {
		return htmap[key];
	}
};
