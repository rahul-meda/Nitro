
#include "Line.h"
#include <glm/gtc/type_ptr.hpp>

namespace Nitro
{
	Line::Line()
	{}

	Line::Line(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& col)
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

	void Line::SetCustomUniforms()
	{
		glUniform3fv(shader.GetUniformLoc("vColor"), 1, glm::value_ptr(color));
	}

	Line::~Line(void)
	{

	}

	int Line::GetTotalVertices()
	{
		return 2;
	}

	int Line::GetTotalIndices()
	{
		return 2;
	}

	GLenum Line::GetPrimitiveType()
	{
		return GL_LINES;
	}

	void Line::SetPoints(const glm::vec3& p1, const glm::vec3& p2)
	{
		this->p1 = p1;
		this->p2 = p2;
		Init();
	}

	void Line::FillVertexBuffer(GLfloat* pBuffer)
	{
		glm::vec3* vertices = (glm::vec3*)(pBuffer);

		vertices[0] = glm::vec3(0);
		vertices[1] = glm::vec3(1,0,0);
	}

	void Line::FillIndexBuffer(GLuint* pBuffer) {

		//fill indices array
		GLuint* id = pBuffer;

		*id++ = 0;
		*id++ = 1;
	}
}