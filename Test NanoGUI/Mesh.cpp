#pragma once
#include "Mesh.h"

#pragma region Mesh
Mesh::Mesh(vector < Vertex >& v, vector < unsigned int >& i, vector < Texture >& t)
{
	vertices = v; //copy vertex data
	indices = i; //copy index data
	textures = t; //copy textures data

	setupMesh(); //call setup
}
Mesh::~Mesh() {}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO); //generate VAO
	glGenBuffers(1, &VBO); //generate VBO
	glGenBuffers(1, &EBO); //generate EBO

	glBindVertexArray(VAO); //bind VAO

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW); //insert vertex data into VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW); //insert index data into EBO

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0); //0 layout for position

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1); //1 layout for normal

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2); //2 layout for UV

	for (int i = 0; i < BONES_AMOUNT; i++)
	{
		glVertexAttribPointer(3 + i, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, boneIDs) + sizeof(float) * i));
		glEnableVertexAttribArray(3 + i); //from 3 to 8 (BONES_AMOUNT) layouts for bones ids. Array in shader uses N layouts, equal to the size, instead of single layout location the vec uses
	}

	for (int i = 0; i < BONES_AMOUNT; i++)
	{
		glVertexAttribPointer(9 + i, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, weights) + sizeof(float) * i));
		glEnableVertexAttribArray(9 + i); //from 9 to 14 for the weights
	}

	glBindVertexArray(0); //unbind 
}

void Mesh::draw(ShaderLoader* shader)
{
	unsigned int diffuseNR = 1; //amount of diffuse textures
	unsigned int specularNR = 1; //amount of specular textures

	for (int i = 0; i < textures.size(); i++) //loop through textures
	{
		glActiveTexture(GL_TEXTURE0 + i); //set the texture active

		string number; //this one is needed if we use more than one texture of the same type

		if (textures[i].type == "texture_diffuse") //if it is a diffuse texture
		{
			number = to_string(diffuseNR); //this is the diffuseNR`s texture
			diffuseNR++; //amount of diffuse + 1
		}
		else if (textures[i].type == "texture_specular")
		{
			number = to_string(specularNR); //this is the specularNR`s texture
			specularNR++; //amount of specular + 1
		}

		glUniform1i(glGetUniformLocation(shader->ID, ("material." + textures[i].type + number).c_str()), i); //send the texture to the shader (example: material.texture_diffuse1)
		glBindTexture(GL_TEXTURE_2D, textures[i].id); //bind this texture
	}

	glBindVertexArray(VAO); //bind VAO
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); //draw mesh from indices

	glBindVertexArray(0); //unbind VAO
	glBindTexture(GL_TEXTURE_2D, 0); //unbind textures
}

vector<vec3> Mesh::translateVertices(float scale, vec3 position, vec3 rot) {
	vector<vec3> returnvalue;
	for (Vertex vertice : vertices) {
		vec3 vertexPos(vertice.position);
		vertexPos *= vec3(scale);
		glm::vec4 vec4Ofvec3 = glm::vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1);
		glm::mat4 transformMatrix = createTransform(position, rot, vec3(scale));
		vertexPos = (vec4Ofvec3 * transformMatrix);
		vertexPos += vec3(position.x, position.y, position.z);

		returnvalue.push_back(vertexPos);
	}
	return returnvalue;
}

glm::mat4 Mesh::createTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
	glm::mat4 trans = glm::mat4(1.0);
	trans = glm::rotate(trans, glm::radians(rot.x), glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, glm::radians(rot.y), glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, glm::radians(rot.z), glm::vec3(0, 0, 1));
	return trans;
}

#pragma endregion
