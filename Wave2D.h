
// Simulate wave surface using Fourier series

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Scene.h"
#include "ClothMesh.h"

namespace Nitro
{
	class Wave2D : public Scene
	{
	private:
		// boundaries of the surface
		int a;
		int b;

		// step-sizes along x and z directions for integration
		float dx;
		float dz;

		// number of terms in the fourier series
		int M;
		int N;

		// vertices of the surface
		glm::vec3 surface[256][256];

		// vertices to draw the surface
		std::vector<glm::vec3> vertices;

		// mesh to draw the surface
		ClothMesh* mesh;

		// fourier coeffiecients calculated once
		float a_mn[256][256];

		// current time
		float t;

		// amplitude of the wave
		float amplitude;

		// frequency of the wave
		float frequency;

		// to switch b/w diff functions to calculate initial height
		int funcType;

	public:
		Wave2D();

		void OnInit(GLFWwindow* window);

		void OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods);

		// initial positions at (x,z)(at t = 0) are calculated using this fucntion
		float Height_t0(float x, float z);

		// calculate Fourier coefficients 
		void CalculateFourierCoeffs();

		// integrate the surface to calculate the Riemann sum
		float RiemannSum(int m, int n);

		// calculate height at (x,z) at given time t
		float Height_t(float x, float z, float t);

		// update the surface each frame
		void Update();
	};
}