
#pragma once

#include "Scene.h"

namespace Nitro
{
	class StackingDemo : public Scene
	{
	public:
		void OnInit(GLFWwindow* window);

		void OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods);
	};
}