
#pragma once

#include "Model.h"

namespace Nitro
{
	class Grid : public Model
	{
	public:
		Grid(int l = 10, int w = 10);

		virtual ~Grid();

		int GetTotalVertices();

		int GetTotalIndices();

		GLenum GetPrimitiveType();

		void FillVertexBuffer(GLfloat* pBuffer);

		void FillIndexBuffer(GLuint* pBuffer);

	private:
		// grid dimensions
		int l, w;

		// total vertices and indices
		int numVertices, numIndices;
	};
}