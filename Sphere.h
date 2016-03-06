#pragma once

#include "Model.h"
#include <glm/glm.hpp>
#include <vector>

namespace Nitro
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
	};

	class Sphere : public Model
	{
	public:
		Sphere(const glm::vec3& color = glm::vec3(1, 1, 1));

		virtual ~Sphere(void);

		int GetTotalVertices();

		int GetTotalIndices();

		GLenum GetPrimitiveType();

		void SetCustomUniforms();

		void FillVertexBuffer(GLfloat* pBuffer);

		void FillIndexBuffer(GLuint* pBuffer);

		void PushIndices(int sectors, int r, int s);

		void CreateSphere(float radius, unsigned int slices, unsigned int stacks);

	private:
		std::vector<Vertex> vertices;

		std::vector<GLushort> indices;
	};
}
