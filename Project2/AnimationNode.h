#pragma once
#include "AnimationMesh.h"

#include <memory>
#include <functional>
#include <string>

class AnimationNode
{
public:
	AnimationNode(std::string name);
	glm::mat4 transform;
	std::string name;
	std::shared_ptr<Bone> bone = nullptr;

	void AddChild(AnimationNode* child);
	void ForEachChild(const std::function<void(AnimationNode&)>& f);
	void AddMesh(AnimationMesh* mesh);
	void ForEachMesh(const std::function<void(AnimationMesh&)>& f);
private:
	std::vector<std::unique_ptr<AnimationMesh>> meshes;
	std::vector<AnimationNode*> children;
};