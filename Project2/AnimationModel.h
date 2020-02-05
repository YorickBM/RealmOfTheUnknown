#pragma once
#include "AnimationShader.h"
#include "AnimationNode.h"
#include "Shader.h";

#include <unordered_map>
#include <memory>
#include <glm\glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Key {
	size_t position;
	size_t rotation;
	size_t scaling;
};

class AnimationModel
{
public:
	AnimationNode* root;
	void RegisterNode(AnimationNode* node);

	AnimationShader* shader = nullptr;
	Shader* shdr = nullptr;

	glm::mat4 inverse_root_transform;
	void SetBuffered(bool buffered);
	bool IsBuffered();
	void SetShader(Shader shdr) {
		this->shdr = &(shdr);
	}
	Shader GetShader() {
		return *(shdr);
	}

protected:
	std::unordered_map<std::string, AnimationNode*> node_mapping;
	bool buffered = false;
};

