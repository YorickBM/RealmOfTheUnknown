#include "AnimationNode.h"
#include <GL\glew.h>

AnimationNode::AnimationNode(std::string name)
{
	this->name = name;
}

void AnimationNode::AddMesh(AnimationMesh * mesh)
{
	meshes.emplace_back(std::move(mesh));
}

void AnimationNode::ForEachMesh(const std::function<void(AnimationMesh&)>& f)
{
	for (auto&& mesh : meshes) {
		f(*mesh);
	}
}

void AnimationNode::AddChild(AnimationNode * child)
{
	children.push_back(child);
}

void AnimationNode::ForEachChild(const std::function<void(AnimationNode&)>& f)
{
	for (auto&& child : children) {
		f(*child);
	}
}