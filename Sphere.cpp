
#include "Sphere.h"
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14285

namespace Nitro
{
	Sphere::Sphere(const glm::vec3& col)
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

		CreateSphere(1, 20, 20);

		Init();
	}

	void Sphere::SetCustomUniforms()
	{
		glUniform3fv(shader.GetUniformLoc("vColor"), 1, glm::value_ptr(color));
	}

	Sphere::~Sphere(void)
	{

	}

	//adds the given sphere indices to the indices vector
	void Sphere::PushIndices(int sectors, int r, int s) 
	{
		int curRow = r * sectors;
		int nextRow = (r + 1) * sectors;

		indices.push_back(curRow + s);
		indices.push_back(nextRow + s);
		indices.push_back(nextRow + (s + 1));

		indices.push_back(curRow + s);
		indices.push_back(nextRow + (s + 1));
		indices.push_back(curRow + (s + 1));
	}

	//generates a sphere primitive with the given radius, slices and stacks
	void Sphere::CreateSphere(float radius, unsigned int slices, unsigned int stacks)
	{
		float const R = 1.0f / (float)(slices - 1);
		float const S = 1.0f / (float)(stacks - 1);

		for (size_t r = 0; r < slices; ++r) 
		{
			for (size_t s = 0; s < stacks; ++s) 
			{
				float const y = (float)(sin(-PI*0.5f + PI * r * R));
				float const x = (float)(cos(2 * PI * s * S) * sin(PI * r * R));
				float const z = (float)(sin(2 * PI * s * S) * sin(PI * r * R));

				Vertex v;
				v.position = glm::vec3(x, y, z)*radius;
				v.normal = glm::normalize(v.position);
				vertices.push_back(v);
				PushIndices(stacks, r, s);
			}
		}
	}

	int Sphere::GetTotalVertices()
	{
		return vertices.size();
	}

	int Sphere::GetTotalIndices()
	{
		//6 faces with 2 triangles each with 3 vertices
		return indices.size();
	}

	GLenum Sphere::GetPrimitiveType()
	{
		return GL_TRIANGLES;
	}

	void Sphere::FillVertexBuffer(GLfloat* pBuffer)
	{
		glm::vec3* v = (glm::vec3*)(pBuffer);

		for (int i = 0; i < vertices.size(); i++)
		{
			v[i] = vertices[i].position;
		}
	}

	void Sphere::FillIndexBuffer(GLuint* pBuffer)
	{
		//fill indices array
		GLuint* id = pBuffer;

		for (int i = 0; i < indices.size(); i++)
		{
			id[i] = indices[i];
		}
	}
}