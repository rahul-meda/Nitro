#pragma once

#include "Model.h"
#include <glm/glm.hpp>
#include <vector>

namespace Nitro
{
	class ClothMesh : public Model
	{
	public:
		ClothMesh(const glm::vec3& color = glm::vec3(1, 1, 1));

		virtual ~ClothMesh(void);

		int GetTotalVertices();

		int GetTotalIndices();

		GLenum GetPrimitiveType();

		void SetPoints(const std::vector<glm::vec3>& points);

		void SetCustomUniforms();

		void FillVertexBuffer(GLfloat* pBuffer);

		void FillIndexBuffer(GLuint* pBuffer);

	private:
		std::vector<glm::vec3> points;
	};
}
