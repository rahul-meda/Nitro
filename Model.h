
#pragma once

#include "Shader.h"
#include <glm/glm.hpp>

namespace Nitro
{
	class Model
	{
	public:
		Model();

		virtual ~Model();

		virtual void Render();

		virtual int GetTotalVertices() = 0;

		virtual int GetTotalIndices() = 0;

		virtual GLenum GetPrimitiveType() = 0;

		virtual void FillVertexBuffer(GLfloat* pBuffer) = 0;

		virtual void FillIndexBuffer(GLuint* pBuffer) = 0;

		virtual void SetColor(const glm::vec3& color);

		virtual void SetCustomUniforms();

		virtual void SetMVP(const glm::mat4& MVP);

		glm::mat4 GetMVP() const;

		void Init();

		void Destroy();

	protected:
		GLuint vaoID;

		GLuint vboVerticesID;

		GLuint vboIndicesID;

		Shader shader;

		GLenum primType;

		int totalVertices;

		int totalIndices;

		glm::vec3 color;

		glm::mat4 MVP;
	};
}