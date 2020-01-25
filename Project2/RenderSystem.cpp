#include "RenderSystem.h"

extern ComponentSystemManager csm;

void RenderSystem::Init()
{
}
void RenderSystem::Update(Shader Shader)
{
	for (auto const& entity : mEntities)
	{
		auto& Model = csm.GetComponent<RenderObjectC>(entity).model;
		auto MeshMode = csm.GetComponent<RenderObjectC>(entity).MeshMode;

		if (MeshMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Model.Draw(Shader);
		if (MeshMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}