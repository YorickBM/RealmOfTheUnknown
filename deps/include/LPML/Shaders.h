#ifndef SHADERS_H
#define SHADERS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace LowPolyMeshLibrary {
	class shader
	{
	private:
		int ID;
	public:
		static const int VERTEX_ATTRIB = 0;
		static const int TEXTURE_ATTRIB = 1;

		shader();
		shader(const char* vertex, const char* fragment);
		void enable();
		void disable();
		void setUniformVec3(GLchar* name, glm::vec3 vector);
		void setUniformMat4(GLchar* name, glm::mat4 matrix);

		GLuint getID();
	};
}
#endif