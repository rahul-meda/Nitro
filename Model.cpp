
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Nitro
{
	Model::Model()
	{

	}

	Model::~Model()
	{
		Destroy();
	}

	void Model::Init()
	{
		// setup vao and vbo stuff
		// generate vao and vbo
		if (!vaoID)
		glGenVertexArrays(1, &vaoID);
		if (!vboVerticesID)
		glGenBuffers(1, &vboVerticesID);
		if (!vboIndicesID)
		glGenBuffers(1, &vboIndicesID);

		// get vertices, indices, and geometry data
		totalVertices = GetTotalVertices();
		totalIndices = GetTotalIndices();
		primType = GetPrimitiveType();

		// allocate buffer data
		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData(GL_ARRAY_BUFFER, totalVertices * sizeof(glm::vec3), 0, GL_STATIC_DRAW);

		GLfloat* pBuffer = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		FillVertexBuffer(pBuffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glEnableVertexAttribArray(shader.GetAttributeLoc("vVertex"));
		glVertexAttribPointer(shader.GetAttributeLoc("vVertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndices * sizeof(GLuint), 0, GL_STATIC_DRAW);

		GLuint* pIBuffer = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		FillIndexBuffer(pIBuffer);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		glBindVertexArray(0);
	}

	void Model::Destroy() 
	{
		//Destroy shader
		shader.DeleteProgram();

		//Destroy vao and vbo
		glDeleteBuffers(1, &vboVerticesID);
		glDeleteBuffers(1, &vboIndicesID);
		glDeleteVertexArrays(1, &vaoID);
	}

	void Model ::Render()
	{
		shader.Use();
		if (glm::value_ptr(MVP) != 0)
			glUniformMatrix4fv(shader.GetUniformLoc("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		SetCustomUniforms();
		glBindVertexArray(vaoID);
		glLineWidth(3.0f);
		glDrawElements(primType, totalIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		shader.UnUse();
	}

	void Model::SetColor(const glm::vec3& color)
	{
		this->color = color;
	}

	void Model::SetCustomUniforms() 
	{

	}

	void Model::SetMVP(const glm::mat4& MVP)
	{
		this->MVP = MVP;
	}

	glm::mat4 Model::GetMVP() const
	{
		return MVP;
	}
}