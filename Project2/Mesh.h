#pragma once
#define _USE_MATH_DEFINES
#define BONES_AMOUNT 6

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
#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Converter.h";
#include "ShaderLoader.h";

using namespace std;
using namespace glm;
using namespace Assimp;


struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texCoords; //UV coordinates

	float boneIDs[BONES_AMOUNT] = { 0.0f }; //bones that are followed by this vertex 
	float weights[BONES_AMOUNT] = { 0.0f }; //strength/weight of the above bones 
};

struct Texture
{
	unsigned int id; //opengl id
	string type; //type diffuse/specular
	string path; //path to the texture
};

class Mesh
{
private:
	GLuint VAO;
	GLuint VBO, EBO;

	vector < Vertex > vertices; //vector of vertices this mesh has
	vector < GLuint > indices; //vector of indices for EBO
	vector < Texture > textures; //textures this mesh has
	vector < vec3 > translated; //vertices modified by model pos & scale

	void setupMesh(); //here we generate VAO, VBO, EBO

public:
	Mesh(vector < Vertex >& v, vector < unsigned int >& i, vector < Texture >& t);

	void draw(ShaderLoader* shader); //here we render the mesh to the given shader

	//Vertice's effected by model scale
	//Vertice's effect by model position
	vector<vec3> translateVertices(float scale, vec3 position);
	vector<vec3> GetTranslatedVertices() { return translated; }
	vector<vec3> GetNonTranslatedVertices() { 
		vector<vec3> verticePositions;

		for(Vertex vertice : vertices) {
			verticePositions.push_back(vertice.position);
		}

		return verticePositions; 
	}

	~Mesh();
};