
#include "Grid.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Nitro
{
	Grid::Grid(int l, int w)
		:l(l), w(w)
	{
		//setup shader
		shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/shader.vert");
		shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/shader.frag");
		shader.LinkProgram();
		shader.Use();
		shader.AddAttribute("vVertex");
		shader.AddUniform("MVP");
		shader.AddUniform("vColor");
		glm::vec3 color = glm::vec3(0.9,0.6,0.3); // Sandy Brown :)
		glUniform3fv(shader.GetUniformLoc("vColor"), 1, glm::value_ptr(color));
		shader.UnUse();

		Init();
	}

	Grid::~Grid()
	{

	}

	int Grid::GetTotalVertices()
	{
		return ((l+1)+(w+1))*2;
	}

	int Grid::GetTotalIndices()
	{
		return (l*w);
	}

	GLenum Grid::GetPrimitiveType()
	{
		return GL_LINES;
	}

	void Grid::FillVertexBuffer(GLfloat* pBuffer)
	{
		glm::vec3* vertices = (glm::vec3*)(pBuffer);

		int hl = l / 2;
		int hw = w / 2;

		int n = 0;

		for (int i = -hl; i <= hl; i++)
		{
			vertices[n++] = glm::vec3(i,0,-hw);
			vertices[n++] = glm::vec3(i,0,hw);

			vertices[n++] = glm::vec3(-hl,0,i);
			vertices[n++] = glm::vec3(hl,0,i);
		}
	}

	void Grid::FillIndexBuffer(GLuint* pBuffer)
	{
		GLuint* id = pBuffer;

		for (int i = 0; i < l*w; i += 4)
		{
			*id++ = i;
			*id++ = i + 1;
			*id++ = i + 2;
			*id++ = i + 3;
		}
	}
}