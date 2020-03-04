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

#include <math.h>

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

class Animation
{
public:
	string name; //the name of the animation

	float startTime; //start time of the animation
	float endTime; //end time of the animation
	float speed; //speed of the animation
	int priority; //priority of the animation
	bool loop; //is this animation looped

	Animation();
	Animation(string name, vec2 times, float speed = 0.25, int priority = 10, bool loop = false);

	void setName(string name);
	void setTime(vec2 frames);
	void setSpeed(float speed);
	void setPriority(int priority);
	void setLoop(bool loop);

	~Animation();
};