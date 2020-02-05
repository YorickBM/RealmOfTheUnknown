#include "AnimationMesh.h"

void AnimationMesh::AddVertex(MeshVertex & vertex)
{
	vertices.push_back(vertex);
}

void AnimationMesh::AddIndex(GLuint index)
{
	indices.push_back(index);
}

void AnimationMesh::AddWeight(unsigned int vertex_index, unsigned int bone_index, GLuint bone_id, GLfloat weight)
{
	vertices[vertex_index].bone_ids[bone_index] = bone_id;
	vertices[vertex_index].bone_weights[bone_index] = weight;
}

size_t AnimationMesh::NumVertices()
{
	return vertices.size();
}

size_t AnimationMesh::NumBones()
{
	return bones.size();
}

size_t AnimationMesh::NumIndices()
{
	return indices.size();
}
