#pragma once
#include "AnimationModel.h"
#include "AnimationNode.h"
#include "AnimationEvaluator.h"
#include "Model.h"

class AnimatedModel : public AnimationModel
{
public:
	void AddAnimation(Animation* animation);
	void SetAnimation(std::string name);
	void Update(double delta);
	void AddBone(std::shared_ptr<Bone> bone);

private:
	double time = 0.0;
	size_t current_animation = -1;
	std::unordered_map<std::string, size_t> animation_mapping;
	std::vector<std::unique_ptr<Animation>> animations;

	void UpdateTransformsHierarchy(AnimationNode & node, Pose& pose, glm::mat4 parent_transform);
};