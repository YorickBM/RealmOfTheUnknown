#pragma once
#define MAX_BONES_AMOUNT 100 
#define _USE_MATH_DEFINES
#define GLEW_STATIC

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <unordered_map>

//#include <cmath>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Converter.h";
using namespace std;
using namespace glm;

#include "ShaderLoader.h"
#include "Bone.h"
#include "Animation.h"

class Skeleton
{
private:
	vector < Bone* > bones; //here we keep our bones
	vector < mat4 > bonesMatrices; //final transformation matrices that go to the shader

	float time; //little timer

	Animation* activeAnimation; //little Animation class to keep the custom animation data

	void renderBonesMatrices(ShaderLoader* shader); //update and send bones matrices to the shader

public:
	Skeleton(vector < Bone* >& bones);

	void playAnimation(Animation* anim, bool reset = true); //play desired animation
	void stopAnimation(); //stop playing

	void update(ShaderLoader* shader); //here we update the animation and call renderBonesMatrices()

	~Skeleton();
};

