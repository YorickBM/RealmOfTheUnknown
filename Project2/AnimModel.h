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
	}

	vec3 GetPosition() { return this->_position; }
	float GetScale() { return this->_scale; }
	string GetPath() { return this->_path; }
	vector<Mesh*> GetMeshes() { return this->meshes; }

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
	float _scale;
	string _path;
};