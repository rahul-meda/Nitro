
#include "ClothMesh.h"
#include <glm/gtc/type_ptr.hpp>

namespace Nitro
{
	ClothMesh::ClothMesh(const glm::vec3& col)
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
	}

	void ClothMesh::SetCustomUniforms()
	{
		glUniform3fv(shader.GetUniformLoc("vColor"), 1, glm::value_ptr(color));
	}

	ClothMesh::~ClothMesh(void)
	{

	}

	int ClothMesh::GetTotalVertices()
	{
		return points.size();
	}

	int ClothMesh::GetTotalIndices()
	{
		return points.size();
	}

	GLenum ClothMesh::GetPrimitiveType()
	{
		return GL_LINES;
	}

	void ClothMesh::SetPoints(const std::vector<glm::vec3>& p)
	{
		this->points.clear();
		this->points.reserve(p.size());
		for (int i = 0; i < p.size(); i++)
			points.push_back(p[i]);

		Init();
	}

	void ClothMesh::FillVertexBuffer(GLfloat* pBuffer)
	{
		glm::vec3* vertices = (glm::vec3*)(pBuffer);

		for (int i = 0; i < points.size(); i++)
		{
			vertices[i] = points[i];
		}
	}

	void ClothMesh::FillIndexBuffer(GLuint* pBuffer)
	{
		//fill indices array
		GLuint* id = pBuffer;

		for (int i = 0; i < points.size(); i++)
		{
			*id++ = i;
		}
	}
}