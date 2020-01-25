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
#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Collision.h"

using namespace std;

///GLint TextureFromFile(const char *path, string directory);

class Model
{
public:
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model() {

	}

	Model(const char *path)
	{
		this->loadModel(path);
		this->GenerateMinAndMaxVertice();
	}
	Model(const char *path, glm::vec3 position, float scale = 1.0f, bool DoCubeFaces = false)
	{
		this->_position = position;
		this->_scale = scale;
		this->_origin = position;
		this->model = glm::translate(this->model, position);
		this->model = glm::scale(this->model, glm::vec3(scale, scale, scale));
		this->_DoCubeFaces = DoCubeFaces;

		this->loadModel(path);
		this->GenerateMinAndMaxVertice();
	}
	Model(Model BaseModel, BoundingBox boundingBox, bool DoCubeFaces = false) {
		this->_position = BaseModel.getPosition();
		this->_scale = BaseModel.getScale();
		this->model = glm::translate(this->model, this->_position);
		this->model = glm::scale(this->model, glm::vec3(this->_scale, this->_scale, this->_scale));
		this->_DoCubeFaces = DoCubeFaces;

		this->_MinVertice = BaseModel.GetMinVertice();
		this->_MaxVertice = BaseModel.GetMaxVertice();

		this->CreateBoundingBoxMesh(BaseModel.GetMaxVertice(), BaseModel.GetMinVertice());
	}

	// Draws the model, and thus all its meshes
	void Draw(Shader shader, glm::mat4 model)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		for (GLuint i = 0; i < this->meshes.size(); i++)
		{
			this->meshes[i].Draw(shader);
		}
	}
	void Draw(Shader shader)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(this->model));
		for (GLuint i = 0; i < this->meshes.size(); i++)
		{
			this->meshes[i].Draw(shader);
		}
	}

	void setPosition(glm::vec3 position) {
		glm::mat4 newModel;
		newModel = glm::translate(newModel, position);
		newModel = glm::scale(newModel, glm::vec3(this->_scale, this->_scale, this->_scale));
		this->model = newModel;
		this->_position = position;

		this->_MinVerticeWithPos = this->_MinVertice + position;
		this->_MaxVerticeWithPos = this->_MaxVertice + this->_MinVerticeWithPos;
	}
	void setScale(float scale) {
		glm::mat4 newModel;
		newModel = glm::translate(newModel, this->_position);
		newModel = glm::scale(newModel, glm::vec3(scale, scale, scale));
		this->model = newModel;
		this->_scale = scale;
	}
	void AddPosition(glm::vec3 position) {
		this->_position += position;
		setPosition(this->_position);
	}

	std::vector<Face> getModelFaces() { return this->_faces; }
	std::vector<glm::vec3> getModelVertices() { return this->_vertices; }

	glm::vec3 getPosition() { return this->_position; }
	glm::vec3 getOrigin() { return this->_origin; }
	float getScale() { return this->_scale; }

	GLint TextureFromFile(const char *path, string directory)
	{
		//Generate texture ID and load texture data
		string filename = string(path);
		filename = directory + '/' + filename;
		GLuint textureID;
		glGenTextures(1, &textureID);

		int width, height;

		///std::cout << filename << std::endl;

		unsigned char *image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);

		return textureID;
	}

	void GenerateMinAndMaxVertice() {
		std::vector<float> X, Y, Z;
		for (glm::vec3 i : this->_vertices) {
			X.push_back(i.x);
			Y.push_back(i.y);
			Z.push_back(i.z);
		}

		this->_MinVertice.x = *std::min_element(X.begin(), X.end());
		this->_MinVertice.y = *std::min_element(Y.begin(), Y.end());
		this->_MinVertice.z = *std::min_element(Z.begin(), Z.end());

		this->_MaxVertice.x = *std::max_element(X.begin(), X.end());
		this->_MaxVertice.y = *std::max_element(Y.begin(), Y.end());
		this->_MaxVertice.z = *std::max_element(Z.begin(), Z.end());

		this->_MinVerticeWithPos = this->_MinVertice + this->_position;
		this->_MaxVerticeWithPos = this->_MaxVertice + this->_position;
	}
	void CreateBoundingBoxMesh() {
		GenerateMinAndMaxVertice();
		CreateBoundingBoxMesh(this->GetMaxVertice(), this->GetMinVertice());

		std::cout << "Vertices not yet created" << std::endl;
	}
	void CreateBoundingBoxMesh(glm::vec3 MaxVertice, glm::vec3 MinVertice) {
		//Data To Fill
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures; //Boundingbox has Alpha Texture (So No Texture :?)

		GLuint cube_elements[] = {
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3
		};
		for (GLuint i : cube_elements) indices.push_back(i);

		//Create all points
		Vertex vertex;
		vertex.Position = glm::vec3(MinVertice.x, MaxVertice.y, MaxVertice.z); vertices.push_back(vertex);
		vertex.Position = glm::vec3(MaxVertice.x, MaxVertice.y, MaxVertice.z); vertices.push_back(vertex);
		vertex.Position = glm::vec3(MaxVertice.x, MinVertice.y, MaxVertice.z); vertices.push_back(vertex);
		vertex.Position = glm::vec3(MinVertice.x, MinVertice.y, MaxVertice.z); vertices.push_back(vertex);

		vertex.Position = glm::vec3(MinVertice.x, MaxVertice.y, MinVertice.z); vertices.push_back(vertex);
		vertex.Position = glm::vec3(MaxVertice.x, MaxVertice.y, MinVertice.z); vertices.push_back(vertex);
		vertex.Position = glm::vec3(MaxVertice.x, MinVertice.y, MinVertice.z); vertices.push_back(vertex);
		vertex.Position = glm::vec3(MinVertice.x, MinVertice.y, MinVertice.z); vertices.push_back(vertex);

		Mesh boundingbox(vertices, indices, textures);
		this->meshes.push_back(boundingbox);
	}

	glm::vec3 GetMinVertice() { return this->_MinVertice; }
	glm::vec3 GetMaxVertice() { return this->_MaxVertice; }
	glm::vec3 GetMinVerticeWithPos() { return this->_MinVerticeWithPos; }
	glm::vec3 GetMaxVerticeWithPos() { return this->_MaxVerticeWithPos; }

	std::vector<glm::vec3> getCubeCorners(glm::vec2 position) { 
		std::vector<glm::vec3> list; 
		for (std::map<string, std::vector<glm::vec3>>::iterator it = this->_CubeSpecificCorners.begin(); it != this->_CubeSpecificCorners.end(); ++it) {
			string x = it->first;
			if(x == (glm::to_string(position)))
				list = it->second;
		}
		return list; 
	}

	glm::vec3 _MinVertice, _MaxVertice, _MinVerticeWithPos, _MaxVerticeWithPos;
private:
	/*  Model Data  */
	vector<Mesh> BoundingBoxMeshes;
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	glm::mat4 model;
	glm::vec3 _position;
	glm::vec3 _origin;
	float _scale;
	std::vector<glm::vec3> _vertices;
	std::vector<Face> _faces;
	std::map<string, std::vector<glm::vec3>> _CubeSpecificCorners;
	std::map<int, std::vector<glm::vec3>> _CubeSpecificCorners2;
	std::map<string, int> _CubeSpecificCorners3;

	glm::vec3 front;
	glm::vec3 right;
	GLfloat movementSpeed;
	bool _DoCubeFaces = false;

	/*  Functions   */
	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string path)
	{
		std::cout << "Path: " << path << std::endl;
		// Read file via ASSIMP
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// Retrieve the directory path of the filepath
		this->directory = path.substr(0, path.find_last_of('/'));

		// Process ASSIMP's root node recursively
		this->processNode(scene->mRootNode, scene);

		std::cout << " -------------------- " << std::endl;
	}

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene)
	{
		// Process each mesh located at the current node
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene.
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}

		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// Data to fill
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;
		glm::vec3 Max, Min;

		// Walk through each of the mesh's vertices
		int q = 0;
		int u = 0;
		std::vector<glm::vec3> TempVertices;
		std::vector<glm::vec3> SecondTempVertices;

		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			#pragma region Temporary Storage
			if (std::find(this->_vertices.begin(), this->_vertices.end(), (vertex.Position)) == this->_vertices.end())
				this->_vertices.push_back(vertex.Position);
			TempVertices.push_back(vertex.Position);
			if (std::find(SecondTempVertices.begin(), SecondTempVertices.end(), (vertex.Position)) == SecondTempVertices.end())
				SecondTempVertices.push_back(vertex.Position);
			#pragma endregion

			//Face Creation
			if (++q == 3) {
				Face TempFace;
				TempFace.Pos0 = TempVertices.at(0);
				TempFace.Pos1 = TempVertices.at(1);
				TempFace.Pos2 = TempVertices.at(2);

				_faces.push_back(TempFace);
				q = 0;
				TempVertices.clear();
			}

			//Square Face Creation
			if (this->_DoCubeFaces && ++u == 6) {
				glm::vec2 CenterPos(
					(int)SecondTempVertices.at(0).x + .5f,
					(int)SecondTempVertices.at(0).z + .5f
				);

				this->_CubeSpecificCorners[glm::to_string(CenterPos)] = SecondTempVertices;
				u = 0;
				SecondTempVertices.clear();
			}

			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			std::cout << "\n - We got a material index - " << std::endl;
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			// 2. Specular maps
			vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		std::cout << typeName << ":" << std::endl;

		aiString str;
		mat->GetTexture(type, 0, &str);
		std::cout << str.C_Str() << std::endl;

		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;

			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)

					break;
				}
			}
			if (!skip)
			{   // If texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);

				this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}

		return textures;
	}
};