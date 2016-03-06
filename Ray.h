#pragma once

#include "Model.h"
#include <glm/glm.hpp>

namespace Nitro
{
	class Ray : public Model
	{
	public:
		Ray(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& color = glm::vec3(1, 1, 1));

		virtual ~Ray(void);

		int GetTotalVertices();

		int GetTotalIndices();

		GLenum GetPrimitiveType();

		void SetPoints(const glm::vec3& p1, const glm::vec3& p2);

		void SetCustomUniforms();

		void FillVertexBuffer(GLfloat* pBuffer);

		void FillIndexBuffer(GLuint* pBuffer);

	private:
		glm::vec3 p1;
		glm::vec3 p2;
	};
}
