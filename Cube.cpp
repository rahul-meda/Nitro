
#include "Cube.h"
#include <glm/gtc/type_ptr.hpp>

namespace Nitro
{
	Cube::Cube(const glm::vec3& col)
	{
		color = col;
		fillBox = true;
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

		frame = new CubeFrame();
	}

	void Cube::SetCustomUniforms() 
	{
		glUniform3fv(shader.GetUniformLoc("vColor"), 1, glm::value_ptr(color));
	}

	Cube::~Cube(void)
	{

	}

	int Cube::GetTotalVertices()
	{
		return 8;
	}

	int Cube::GetTotalIndices() 
	{
		//6 faces with 2 triangles each with 3 vertices
		return 6 * 2 * 3;
	}

	GLenum Cube::GetPrimitiveType()
	{
		return GL_TRIANGLES;
	}

	void Cube::FillVertexBuffer(GLfloat* pBuffer)
	{
		glm::vec3* vertices = (glm::vec3*)(pBuffer);

		vertices[0] = glm::vec3(-1.0f, -1.0f, -1.0f);
		vertices[1] = glm::vec3(1.0f, -1.0f, -1.0f);
		vertices[2] = glm::vec3(1.0f, 1.0f, -1.0f);
		vertices[3] = glm::vec3(-1.0f, 1.0f, -1.0f);
		vertices[4] = glm::vec3(-1.0f, -1.0f, 1.0f);
		vertices[5] = glm::vec3(1.0f, -1.0f, 1.0f);
		vertices[6] = glm::vec3(1.0f, 1.0f, 1.0f);
		vertices[7] = glm::vec3(-1.0f, 1.0f, 1.0f);
	}

	void Cube::FillIndexBuffer(GLuint* pBuffer)
	{
		//fill indices array
		GLuint* id = pBuffer;

		//bottom face
		*id++ = 0; 	*id++ = 5; 	*id++ = 4;
		*id++ = 5; 	*id++ = 0; 	*id++ = 1;

		//top face
		*id++ = 3; 	*id++ = 7; 	*id++ = 6;
		*id++ = 3; 	*id++ = 6; 	*id++ = 2;

		//front face
		*id++ = 7; 	*id++ = 4; 	*id++ = 6;
		*id++ = 6; 	*id++ = 4; 	*id++ = 5;

		//back face
		*id++ = 2; 	*id++ = 1; 	*id++ = 3;
		*id++ = 3; 	*id++ = 1; 	*id++ = 0;

		//left face 
		*id++ = 3; 	*id++ = 0; 	*id++ = 7;
		*id++ = 7; 	*id++ = 0; 	*id++ = 4;

		//right face 
		*id++ = 6; 	*id++ = 5; 	*id++ = 2;
		*id++ = 2; 	*id++ = 5; 	*id++ = 1;
	}

	void Cube::SetMVP(const glm::mat4& MVP)
	{
		this->MVP = MVP;
		frame->SetMVP(MVP);
	}

	void Cube::FillBox(bool fill)
	{
		fillBox = fill;
	}

	void Cube::Render()
	{
		if (fillBox)
		{
			Model::Render();
		}
		else
		{
			frame->SetColor(glm::vec3(0.1,0.1,0.79));
		}
		frame->Render();
	}

	CubeFrame::CubeFrame(const glm::vec3& col)
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

	void CubeFrame::SetCustomUniforms()
	{
		glUniform3fv(shader.GetUniformLoc("vColor"), 1, glm::value_ptr(color));
	}

	CubeFrame::~CubeFrame(void)
	{

	}

	int CubeFrame::GetTotalVertices()
	{
		return 8;
	}

	int CubeFrame::GetTotalIndices()
	{
		// 12 edges, 2 indices per edge
		return 12*2;
	}

	GLenum CubeFrame::GetPrimitiveType()
	{
		return GL_LINES;
	}

	void CubeFrame::FillVertexBuffer(GLfloat* pBuffer)
	{
		glm::vec3* vertices = (glm::vec3*)(pBuffer);

		// back face
		vertices[0] = glm::vec3(-1.005f, -1.005f, -1.005f); // BL
		vertices[1] = glm::vec3(1.005f, -1.005f, -1.005f);  // BR 
		vertices[2] = glm::vec3(1.005f, 1.005f, -1.005f);	  // TR
		vertices[3] = glm::vec3(-1.005f, 1.005f, -1.005f);  // TL
		// front face
		vertices[4] = glm::vec3(-1.005f, -1.005f, 1.005f); // BL
		vertices[5] = glm::vec3(1.005f, -1.005f, 1.005f);  // BR
		vertices[6] = glm::vec3(1.005f, 1.005f, 1.005f);   // TR
		vertices[7] = glm::vec3(-1.005f, 1.005f, 1.005f);  // TL
	}

	void CubeFrame::FillIndexBuffer(GLuint* pBuffer)
	{
		// fill indices array
		GLuint* id = pBuffer;

		// back face
		*id++ = 0; *id++ = 1; *id++ = 1; *id++ = 2;
		*id++ = 2; *id++ = 3; *id++ = 3; *id++ = 0;

		// front face
		*id++ = 4; *id++ = 5; *id++ = 5; *id++ = 6;
		*id++ = 6; *id++ = 7; *id++ = 7; *id++ = 4;

		*id++ = 0; *id++ = 4; *id++ = 1; *id++ = 5;

		*id++ = 2; *id++ = 6; *id++ = 3; *id++ = 7;
	}
}