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
		this->localTransform = mat4(1.0);

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
		this->localTransform = mat4(1.0);
		applyLocalPosition(position);
		applyLocalScale(scale);

		this->modelLoader->loadModel(path); //load the model from the file
		this->modelLoader->getModelData(this->skeleton, this->meshes); //get the loaded data and store it in this class

		this->_scale = scale;
		this->_position = position;
		this->_path = path;
		this->_origin = position;
	}
	AnimModel(Mesh* mesh, glm::vec3 position, float scale) {
		this->localTransform = mat4(1.0);
		applyLocalPosition(position);
		applyLocalScale(scale);

		this->meshes.push_back(mesh);

		this->_scale = scale;
		this->_position = position;
		this->_origin = position;
	}

	vec3 GetPosition() { return this->_position; }
	float GetScale() { return this->_scale; }
	string GetPath() { return this->_path; }
	vector<Mesh*> GetMeshes() { return this->meshes; }
	vector<vec3> GetMinAndMaxVertice() { 
		vector<vec3> minAndMax;
		minAndMax.push_back(this->_MinVertice);
		minAndMax.push_back(this->_MaxVertice);
	
		return minAndMax;
	}
	
	AnimModel GetBoundingBoxModel() { 
		return AnimModel(CreateBoundingBoxMesh(), this->GetPosition(), this->GetScale()); //Upload mesh vector in here
	}
	void GenerateMinAndMaxVertice() {
		std::vector<float> X, Y, Z;
		for (glm::vec3 i : this->meshes.at(meshes.size() - 1)->GetNonTranslatedVertices()) {
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
		GenerateMinAndMaxVertice();

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

	void applyLocalRotation(float angle, vec3 axis)
	{
		vec3 sc;
		quat rot;
		vec3 tran;
		vec3 skew;
		vec4 perspective;

		decompose(localTransform, sc, rot, tran, skew, perspective);

		localTransform = mat4(1.0);
		localTransform *= translate(localTransform, tran);
		localTransform *= scale(localTransform, sc);
		localTransform *= rotate(localTransform, radians(angle), axis) * mat4_cast(conjugate(rot));
	}
	void applyLocalPosition(vec3 translation)
	{
		vec3 sc = vec3(this->GetScale(), this->GetScale(), this->GetScale());
		quat rot;
		vec3 tran;
		vec3 skew;
		vec4 perspective;

		decompose(localTransform, sc, rot, tran, skew, perspective);

		localTransform = mat4(1.0);
		localTransform *= glm::translate(localTransform, translation) * glm::translate(localTransform, tran);
		localTransform *= glm::scale(localTransform, sc);
		localTransform *= mat4_cast(conjugate(rot));
	}
	void applyLocalScale(float scale)
	{
		vec3 sc;
		quat rot;
		vec3 tran;
		vec3 skew;
		vec4 perspective;

		decompose(localTransform, sc, rot, tran, skew, perspective);

		localTransform = mat4(1.0);
		localTransform *= glm::translate(localTransform, tran);
		localTransform *= glm::scale(localTransform, sc) * glm::scale(localTransform, vec3(scale, scale, scale));
		localTransform *= mat4_cast(conjugate(rot));
	}

	void SetPosition(vec3 position) {
		vec3 sc = vec3(this->GetScale(), this->GetScale(), this->GetScale());
		quat rot;
		vec3 tran;
		vec3 skew;
		vec4 perspective;

		decompose(localTransform, sc, rot, tran, skew, perspective);

		localTransform = mat4(1.0);
		localTransform *= glm::translate(localTransform, position);
		localTransform *= glm::scale(localTransform, sc);
		localTransform *= mat4_cast(conjugate(rot));
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

private:
	ModelLoader* modelLoader; //class to import models from files
	vector < Mesh* > meshes; //here we keep meshes data
	Skeleton* skeleton; //and a skeleton
	mat4 localTransform;

	vec3 _position;
	vec3 _origin;
	float _scale;
	string _path;

	vec3 _MinVertice, _MaxVertice;
};