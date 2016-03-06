
#pragma once

#include "Model.h"

namespace Nitro
{
	class Plane : public Model
	{
	public:
		Plane(const int l = 10, const int w = 10);

		~Plane();

		int GetTotalVertices();

		int GetTotalIndices();

		GLenum GetPrimitiveType();

		void FillVertexBuffer(GLfloat* pBuffer);

		void FillIndexBuffer(GLuint* pBuffer);

	private:
		// dimensions of the plane
		int l, w;
	};
}