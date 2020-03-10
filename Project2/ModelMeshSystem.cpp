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
		glUniformMatrix4fv(glGetUniformLocation(shaderLoader->ID, "model"), 1, GL_FALSE, value_ptr(objectModel)); //send the empty model matrix to the shader
		modelMeshC.model.Draw(shaderLoader);
	}
}