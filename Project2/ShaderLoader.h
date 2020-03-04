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

//#include <math.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace std;
using namespace glm;

class ShaderLoader
{
public:
	unsigned int ID;

	ShaderLoader();

	GLuint loadShaders(const char* vertex_file_path, const char* fragment_file_path);

	void use();
	void unuse();

	~ShaderLoader();
};

