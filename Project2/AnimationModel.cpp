#include "AnimationModel.h"

void AnimationModel::RegisterNode(AnimationNode * node)
{
	node_mapping[node->name] = node;
}

void AnimationModel::SetBuffered(bool buffered)
{
	this->buffered = buffered;
}

bool AnimationModel::IsBuffered()
{
	return buffered;
}
