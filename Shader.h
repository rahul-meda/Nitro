
#pragma once

#include <GL/glew.h>
#include <string>
#include <map>

namespace Nitro
{
	class Shader
	{
	public:
		Shader();

		~Shader();

		// Reads the shader from file 
		void LoadFromFile(GLenum type, const std::string& fileName);

		// Compiles the shader object
		void CompileShader(GLenum type, const std::string& source);

		// Links the shader program
		void LinkProgram();

		// Binds the shader object
		void Use();

		// Unbinds the shader object
		void UnUse();

		// Store the attribute location once at init
		void AddAttribute(const std::string& attribute);

		// Store the uniform location once at init
		void AddUniform(const std::string& unifrom);

		// Retrieve the attribute location
		GLuint GetAttributeLoc(const std::string& attribute);

		// Retrieve the uniform location
		GLuint GetUniformLoc(const std::string& uniform);

		// Delete the shader program
		void DeleteProgram();

	private:
		enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER };

		// Number of shader objects
		int numShaders;

		// The list of shader objects
		GLuint shaders[3];

		// The shader program
		GLuint shaderProg;

		// List of attributes, and their respective locations
		std::map<std::string, GLuint> attributes;

		// List of uniforms, and their respective locations
		std::map<std::string, GLuint> uniforms;
	};
}