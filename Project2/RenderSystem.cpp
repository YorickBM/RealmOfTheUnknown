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
		Model.Draw(Shader);
	}
}