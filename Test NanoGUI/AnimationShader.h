#pragma once
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>

class AnimationShader
{
public:
	AnimationShader();
	AnimationShader(std::string vs, std::string fs) {
		this->AddComponent(vs, GL_VERTEX_SHADER);
		this->AddComponent(fs, GL_FRAGMENT_SHADER);
		this->Link();
	}
	void AddComponent(std::string filename, GLenum type);
	void Link();
	void Use();
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	GLuint& ID();
private:
	std::vector<GLuint> components;
	GLuint identifier;
};

