#ifndef FUNC_H
#define FUNC_H

#include <GLFW\glfw3.h>
#include <Windows.h>
#include <string>
#include <fstream>

using namespace std;

namespace LowPolyMeshLibrary {
	string LoadFileToString(string filepath);
	GLuint loadBMP(const char* filepath);
}
#endif