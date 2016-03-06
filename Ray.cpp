
#include "Ray.h"
#include <glm/gtc/type_ptr.hpp>

namespace Nitro
{
	Ray::Ray(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& col)
	{
		color = col;
		//generate the cube object
		shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/shader.vert");
		shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/shader.frag");
		shader.LinkProgram();
		shader.Use();
		shader.AddAttribute("vVertex");
		shader.AddUniform("MVP");
		shader.AddUniform("vColor");
		glUniform3fv(shader.GetUniformLoc("vColor"), 1, glm::value_ptr(color));
		shader.UnUse();

		Init();
	}

	void Ray::SetCustomUniforms()
	{
		glUniform3fv(shader.GetUniformLoc("vColor"), 1, glm::value_ptr(color));
	}

	Ray::~Ray(void)
	{

	}

	int Ray::GetTotalVertices()
	{
		return 2;
	}

	int Ray::GetTotalIndices()
	{
		return 2;
	}

	GLenum Ray::GetPrimitiveType()
	{
		return GL_LINES;
	}

	void Ray::SetPoints(const glm::vec3& p1, const glm::vec3& p2)
	{
		this->p1 = p1;
		this->p2 = p2;
		Init();
	}

	void Ray::FillVertexBuffer(GLfloat* pBuffer)
	{
		glm::vec3* vertices = (glm::vec3*)(pBuffer);

		vertices[0] = p1;
		vertices[1] = p2;
	}

	void Ray::FillIndexBuffer(GLuint* pBuffer) {

		//fill indices array
		GLuint* id = pBuffer;

		*id++ = 0;
		*id++ = 1;
	}
}