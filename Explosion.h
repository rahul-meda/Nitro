
#pragma once

#include "Scene.h"

namespace Nitro
{
	class Explosion : public Scene
	{
	public:
		void OnInit(GLFWwindow* window);

		void OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods);
	};
}