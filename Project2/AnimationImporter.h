#pragma once
#include <string>
#include "AnimatedModelIncludes.h"

class AnimationImporter
{
public:
	static AnimatedModel* LoadAnimatedFile(std::string filename);
};


