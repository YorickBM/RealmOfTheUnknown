#pragma once
#define _USE_MATH_DEFINES

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include <math.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Collision.h"

using namespace std;

GLint TextureFromFile(const char *path, string directory);

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
		this->col = new que::Collision();
	}
	Model(const char *path, glm::vec3 position, float scale = 1.0f)
	{
		this->_position = position;
		this->_scale = scale;
		this->_origin = position;
		this->model = glm::translate(this->model, position);
		this->model = glm::scale(this->model, glm::vec3(scale, scale, scale));
		this->loadModel(path);
		this->col = new que::Collision();
		this->ProcessBoundingBox(this->_position);
	}

	void ProcessBoundingBox(glm::vec3 position) {
		glm::vec3 Max, Min;
		glm::vec3 pos = this->_position + position;

		Max.x = (this->getModelVertices()[this->_MaxVertice.x].x + pos.x);
		Max.y = (this->getModelVertices()[this->_MaxVertice.y].y + pos.y);
		Max.z = (this->getModelVertices()[this->_MaxVertice.z].z + pos.z);

		Min.x = (this->getModelVertices()[this->_MinVertice.x].x + pos.x);
		Min.y = (this->getModelVertices()[this->_MinVertice.y].y + pos.y);
		Min.z = (this->getModelVertices()[this->_MinVertice.z].z + pos.z);

		this->_boundingBox.min = Min;
		this->_boundingBox.max = Max;
	}
	void DetectCollision(Model SceneModel) {

		std::cout << "--- Starting Collision Detection ---" << std::endl;
		/*for (int current = 0; current < SceneModel.getModelVertices().size(); current++) {
			double x = this->col->CalcAngleSum(SceneModel.getModelVertices()[current],this->getModelVertices(), this->getModelVertices().size());
			if (abs(x) < M_PI)
				exit(1);
			
			std::cout << x << std::endl;
			std::cout << "> " << SceneModel.getModelVertices()[current].x << std::endl;
		}*/
		this->col->detectCollision(SceneModel.getModelFaces(), this->getModelVertices());


		std::cout << "--- Finished Collision Detection ---" << std::endl;
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

	std::vector<glm::vec3> testList;
	void getATriangleFace() {
		for (GLuint i = 0; i < this->meshes.size(); i++)
		{
			for (int h = 0; h < this->meshes[i].indices.size(); h++) {
				float x = this->meshes[i].vertices[h].Position.x;
				float y = this->meshes[i].vertices[h].Position.y;
				float z = this->meshes[i].vertices[h].Position.z;

				glm::vec3 prod = glm::vec3(x, y, z);

				if(std::find(testList.begin(), testList.end(), prod) == testList.end())
					testList.push_back(prod);

				///std::cout << x << ";" << y << ";" << z << std::endl;
			}

			std::cout << "Faces: " << _faces.size() << std::endl;
			std::cout << "Vertices: " << testList.size() << std::endl;
		}
	}

	void setPosition(glm::vec3 position) {
		glm::mat4 newModel;
		newModel = glm::translate(newModel, position);
		newModel = glm::scale(newModel, glm::vec3(this->_scale, this->_scale, this->_scale));
		this->model = newModel;
		this->_position = position;
		this->ProcessBoundingBox(position);
	}
	void setScale(float scale) {
		glm::mat4 newModel;
		newModel = glm::translate(newModel, this->_position);
		newModel = glm::scale(newModel, glm::vec3(scale, scale, scale));
		this->model = newModel;
		this->_scale = scale;
	}

	BoundingBox getBoundingBox() { return this->_boundingBox; }

	std::vector<Face> getModelFaces() { return this->_faces; }
	std::vector<glm::vec3> getModelVertices() { return this->_vertices; }

	glm::vec3 getPosition() { return this->_position; }
	glm::vec3 getOrigin() { return this->_origin; }
	float getScale() { return this->_scale; }

private:
	que::Collision* col;

	/*  Model Data  */
	BoundingBox _boundingBox;
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	glm::mat4 model;
	glm::vec3 _position;
	glm::vec3 _origin;
	float _scale;
	std::vector<glm::vec3> _vertices;
	std::vector<Face> _faces;
	glm::vec3 _MinVertice, _MaxVertice;

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
		std::vector<glm::vec3> TempVertices;
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			if (std::find(this->_vertices.begin(), this->_vertices.end(), (vertex.Position + this->_position)) == this->_vertices.end())
				this->_vertices.push_back(vertex.Position + this->_position);
			TempVertices.push_back(vertex.Position + this->_position);

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

			//Get Max & Min Values;
			Max.x = max(vector.x, Max.x);
			Max.y = max(vector.y, Max.y);
			Max.z = max(vector.z, Max.z);

			Min.x = min(vector.x, Min.x);
			Min.y = min(vector.y, Min.y);
			Min.z = min(vector.z, Min.z);

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

		//Store BoundingBox
		this->_boundingBox.min = Min;
		this->_boundingBox.max = Max;
		BoundingBoxPreperation();

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

	// Peer
	void BoundingBoxPreperation() {
		for (int i = 0; i < this->getModelVertices().size(); i++) {
			if (max(this->getModelVertices()[i].x + this->_position.x, this->_boundingBox.max.x)) this->_MaxVertice.x = i;
			if (max(this->getModelVertices()[i].y + this->_position.y, this->_boundingBox.max.y)) this->_MaxVertice.y = i;
			if (max(this->getModelVertices()[i].z + this->_position.z, this->_boundingBox.max.z)) this->_MaxVertice.z = i;

			if (min(this->getModelVertices()[i].x + this->_position.x, this->_boundingBox.min.x)) this->_MinVertice.x = i;
			if (min(this->getModelVertices()[i].y + this->_position.y, this->_boundingBox.min.y)) this->_MinVertice.y = i;
			if (min(this->getModelVertices()[i].z + this->_position.z, this->_boundingBox.min.z)) this->_MinVertice.z = i;
		}
	}
};

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