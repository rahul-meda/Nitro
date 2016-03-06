
#include "Plane.h"
#include <glm/glm.hpp>

namespace Nitro
{
	Plane::Plane(int l, int w)
		:l(l), w(w)
	{
		//setup shader
		shader.LoadFromFile(GL_VERTEX_SHADER, "Shaders/shader.vert");
		shader.LoadFromFile(GL_FRAGMENT_SHADER, "Shaders/shader.frag");
		shader.LinkProgram();
		shader.Use();
		shader.AddAttribute("vVertex");
		shader.AddUniform("MVP");
		shader.UnUse();

		Init();
	}

	Plane::~Plane()
	{

	}

	int Plane::GetTotalVertices()
	{
		return 4;
	}

	int Plane::GetTotalIndices()
	{
		return 6;
	}

	GLenum Plane::GetPrimitiveType()
	{
		return GL_TRIANGLES;
	}

	void Plane::FillVertexBuffer(GLfloat* pBuffer)
	{
		glm::vec3* vertices = (glm::vec3*)pBuffer;

		int hl = l / 2;
		int hw = w / 2;

		vertices[0] = glm::vec3(-hl,0,-hw);
		vertices[1] = glm::vec3(hl,0,-hw);
		vertices[2] = glm::vec3(hl,0,hw);
		vertices[3] = glm::vec3(-hl,0,hw);
	}

	void Plane::FillIndexBuffer(GLuint* pBuffer)
	{
		GLuint* i = pBuffer;

		*i++ = 0;
		*i++ = 1;
		*i++ = 2;
		*i++ = 0;
		*i++ = 2;
		*i++ = 3;
	}
}