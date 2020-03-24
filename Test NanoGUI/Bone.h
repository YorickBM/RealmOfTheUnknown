#pragma once
#define _USE_MATH_DEFINES

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <unordered_map>
#include <cmath>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Converter.h"
using namespace std;
using namespace glm;

#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace Assimp;

class Bone
{
public:
	string name; //the name of the bone
	int id; //bone’s id in the model

	aiNode* node; //the node this bone is attached to, we keep transformation matrix here
	aiNodeAnim* nodeAnim; //this bone`s animation node, keyframe data is here

	Bone* parentBone; //parent bone
	mat4 offset; //offset matrix

	//keyframe data. This data is calculated in a particular period of time
	vec3 pos;
	quat rot;
	vec3 scal;

	Bone(int id, string name, mat4& offset);

	mat4 getParentTransforms(); //calculates the whole transformation matrix starting from the root bone 

	vec3 calcInterpolatedPosition(float time); //calculates interpolated position between two keyframes
	quat calcInterpolatedRotation(float time); //calculates interpolated rotation between two keyframes

	int findPosition(float time); //finds the index of the keyframe with the value less than time
	int findRotation(float time); //same as above

	void updateKeyframeTransform(float time); //updates this bone transformation matrix

	~Bone();
};


