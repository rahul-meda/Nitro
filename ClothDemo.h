
#pragma once

#include "Spring.h"
#include "Scene.h"
#include <vector>
#include "ClothMesh.h"

namespace Nitro
{
	class ClothDemo : public Scene
	{
	public:
		static ClothDemo& GetInstance();

		void OnInit(GLFWwindow* window);

		void OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods);
		
		std::vector<Spring> GetSprings();

		void Update();

	private:
		ClothDemo();

		std::vector<Spring> springs;

		ClothMesh* mesh;
	};
}