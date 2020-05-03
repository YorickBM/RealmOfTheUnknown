#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "ModelLoader.h"

using namespace std;
using namespace glm;

class AnimModel {
public:
	AnimModel() {}
	AnimModel(string path) {
		ModelLoader* loader = new ModelLoader();
		this->modelLoader = loader;
		SetPosition(vec3(0));
		SetScale(1.f);

		this->modelLoader->loadModel(path); //load the model from the file
		this->modelLoader->getModelData(this->skeleton, this->meshes); //get the loaded data and store it in this class

		this->_scale = 1.0f;
		this->_position = glm::vec3(0, 0, 0);
		this->_path = path;
		this->_origin = glm::vec3(0, 0, 0);
	}
	AnimModel(string path, glm::vec3 position, float scale) {
		ModelLoader* loader = new ModelLoader();
		this->modelLoader = loader;
		SetPosition(position);
		SetScale(scale);

		///applyLocalPosition(position);
		///applyLocalScale(scale);

		this->modelLoader->loadModel(path); //load the model from the file
		this->modelLoader->getModelData(this->skeleton, this->meshes); //get the loaded data and store it in this class

		this->_scale = scale;
		this->_position = position;
		this->_path = path;
		this->_origin = position;
	}
	AnimModel(string path, glm::vec3 position, float scale, vec3 rotation) {
		ModelLoader* loader = new ModelLoader();
		this->modelLoader = loader;
		SetPosition(position);
		SetScale(scale);
		SetRotation(rotation);

		this->modelLoader->loadModel(path); //load the model from the file
		this->modelLoader->getModelData(this->skeleton, this->meshes); //get the loaded data and store it in this class

		this->_path = path;
		this->_origin = position;
	}
	AnimModel(Mesh* mesh, glm::vec3 position, float scale, vec3 rotation) {
		SetPosition(position);
		SetScale(scale);
		SetRotation(rotation);

		this->meshes.push_back(mesh);

		this->_scale = scale;
		this->_position = position;
		this->_origin = position;
	}

	vec3 GetPosition() { return this->_position; }
	float GetScale() { return this->_scale; }
	vec3 GetRotation() { return vec3(this->rotX, this->rotY, this->rotZ); }
	string GetPath() { return this->_path; }
	vector<Mesh*> GetMeshes() { return this->meshes; }
	void GetMinAndMaxVertice(vec3& min, vec3& max) {
		GenerateMinAndMaxVerticesTranslated(min, max, this->_position, vec3(this->rotX, this->rotY, this->rotZ), this->_scale);
	}
	
	AnimModel GetBoundingBoxModel() { 
		return AnimModel(CreateBoundingBoxMesh(), this->GetPosition(), this->GetScale(), this->GetRotation()); //Upload mesh vector in here
	}
	void MinAndMaxVertices(vec3& min, vec3& max, vector<float> X, vector<float> Y, vector<float> Z) {
		min.x = *std::min_element(X.begin(), X.end());
		min.y = *std::min_element(Y.begin(), Y.end());
		min.z = *std::min_element(Z.begin(), Z.end());

		max.x = *std::max_element(X.begin(), X.end());
		max.y = *std::max_element(Y.begin(), Y.end());
		max.z = *std::max_element(Z.begin(), Z.end());
	}
	void GenerateMinAndMaxVertices() {
		std::vector<float> X, Y, Z;
		for (glm::vec3 i : this->meshes.at(meshes.size() - 1)->GetNonTranslatedVertices()) {
			X.push_back(i.x);
			Y.push_back(i.y);
			Z.push_back(i.z);
		}

		MinAndMaxVertices(this->_MinVertice, this->_MaxVertice, X, Y, Z);
	}
	void GenerateMinAndMaxVerticesTranslated(vec3& min, vec3& max, vec3 position, vec3 rotation, float scale = 1) {
		std::vector<float> Xt, Yt, Zt;
		std::vector<vec3> verticest = this->meshes.at(meshes.size() - 1)->translateVertices(scale, position, rotation);
		for (glm::vec3 i : verticest) {
			Xt.push_back(i.x);
			Yt.push_back(i.y);
			Zt.push_back(i.z);
		}
		
		MinAndMaxVertices(min, max, Xt, Yt, Zt);
	}
	Mesh* CreateBoundingBoxMesh() {
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
		GenerateMinAndMaxVertices();

		//Create all points
		Vertex vertex;
		vertex.position = glm::vec3(this->_MinVertice.x, this->_MaxVertice.y, this->_MaxVertice.z); vertices.push_back(vertex);
		vertex.position = glm::vec3(this->_MaxVertice.x, this->_MaxVertice.y, this->_MaxVertice.z); vertices.push_back(vertex);
		vertex.position = glm::vec3(this->_MaxVertice.x, this->_MinVertice.y, this->_MaxVertice.z); vertices.push_back(vertex);
		vertex.position = glm::vec3(this->_MinVertice.x, this->_MinVertice.y, this->_MaxVertice.z); vertices.push_back(vertex);

		vertex.position = glm::vec3(this->_MinVertice.x, this->_MaxVertice.y, this->_MinVertice.z); vertices.push_back(vertex);
		vertex.position = glm::vec3(this->_MaxVertice.x, this->_MaxVertice.y, this->_MinVertice.z); vertices.push_back(vertex);
		vertex.position = glm::vec3(this->_MaxVertice.x, this->_MinVertice.y, this->_MinVertice.z); vertices.push_back(vertex);
		vertex.position = glm::vec3(this->_MinVertice.x, this->_MinVertice.y, this->_MinVertice.z); vertices.push_back(vertex);

		Mesh* boundingbox = new Mesh(vertices, indices, textures);
		return boundingbox;
	}

	void playAnimation(Animation* anim, bool reset = false)
	{
		this->skeleton->playAnimation(anim, reset); //play animation
	}
	void stopAnimation()
	{
		this->skeleton->stopAnimation(); //stop animation
	}

	void SetPosition(vec3 argument) {
		this->_position = argument;
	}
	void SetScale(float argument) {
		this->_scale = argument;
	}
	void SetRotation(vec3 argument) {
		this->rotX = argument.x;
		this->rotY = argument.y;
		this->rotZ = argument.z;
	}

	void Draw(ShaderLoader* shader)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "localTransform"), 1, GL_FALSE, value_ptr(this->localTransform));

		this->skeleton->update(shader); //rendering the skeleton part

		for (int i = 0; i < this->meshes.size(); i++) //loop through the meshes
		{
			this->meshes[i]->draw(shader); //rendering the mesh part
		}
	}
	void Draw(ShaderLoader* shader, bool renderSkeleton)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "localTransform"), 1, GL_FALSE, value_ptr(this->localTransform));

		if(renderSkeleton)
			this->skeleton->update(shader); //rendering the skeleton part

		for (int i = 0; i < this->meshes.size(); i++) //loop through the meshes
		{
			this->meshes[i]->draw(shader); //rendering the mesh part
		}
	}

private:
	ModelLoader* modelLoader; //class to import models from files
	vector < Mesh* > meshes; //here we keep meshes data
	Skeleton* skeleton; //and a skeleton
	mat4 localTransform;

	vec3 _position;
	vec3 _origin;
	float _scale;
	string _path;

	float rotX, rotY, rotZ;

	vec3 _MinVertice, _MaxVertice;
};