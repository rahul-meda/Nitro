#pragma once

#include "Model.h"
#include <glm/glm.hpp>

namespace Nitro
{
	class CubeFrame;

	class Cube : public Model
	{
	private:
		CubeFrame* frame;

		// if this is false, only the frame is drawn
		bool fillBox;

	public:
		Cube(const glm::vec3& color = glm::vec3(1, 1, 1));

		virtual ~Cube(void);

		int GetTotalVertices();

		int GetTotalIndices();

		GLenum GetPrimitiveType();

		virtual void SetCustomUniforms();

		void FillVertexBuffer(GLfloat* pBuffer);

		void FillIndexBuffer(GLuint* pBuffer);

		void SetMVP(const glm::mat4& MVP);

		void FillBox(bool fill = true);

		void Render();
	};

	class CubeFrame : public Model
	{
	public:
		CubeFrame(const glm::vec3& color = glm::vec3(1, 1, 1));

		virtual ~CubeFrame(void);

		int GetTotalVertices();

		int GetTotalIndices();

		GLenum GetPrimitiveType();

		void SetCustomUniforms();

		void FillVertexBuffer(GLfloat* pBuffer);

		void FillIndexBuffer(GLuint* pBuffer);
	};
}
