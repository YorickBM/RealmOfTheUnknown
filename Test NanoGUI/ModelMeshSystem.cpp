#include "ModelMeshSystem.h"

extern ComponentSystemManager csm;
void ModelMeshSystem::Init()
{
}

void ModelMeshSystem::Update(ShaderLoader* shaderLoader)
{
	for (auto const& entity : mEntities)
	{
		auto& modelMeshC = csm.GetComponent<ModelMeshC>(entity);
		auto& transformC = csm.GetComponent<TransformC>(entity);

		modelMeshC.model.SetPosition(transformC.position);

		mat4 objectModel; //model matrix
		objectModel = mat4(1.0);
		objectModel = glm::translate(objectModel, modelMeshC.model.GetPosition());
		objectModel = glm::scale(objectModel, glm::vec3(modelMeshC.model.GetScale()));
		objectModel = glm::rotate(objectModel, glm::radians(modelMeshC.model.GetRotation().x), glm::vec3(1.f, 0.f, 0.f));
		objectModel = glm::rotate(objectModel, glm::radians(modelMeshC.model.GetRotation().y), glm::vec3(0.f, 1.f, 0.f));
		objectModel = glm::rotate(objectModel, glm::radians(modelMeshC.model.GetRotation().z), glm::vec3(0.f, 0.f, 1.f));
		glUniformMatrix4fv(glGetUniformLocation(shaderLoader->ID, "model"), 1, GL_FALSE, value_ptr(objectModel)); //send the model matrix to the shader
		modelMeshC.model.Draw(shaderLoader);

		//glUniformMatrix4fv(glGetUniformLocation(shaderLoader->ID, "model"), 1, GL_FALSE, value_ptr(objectModel)); //send the model matrix to the shader
		//modelMeshC.BoundingBox.Draw(shaderLoader, false);
	}
}