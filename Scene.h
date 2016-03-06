
#pragma once

#define GLEW_STATIC

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "MouseJoint.h"

namespace Nitro
{
	class Body;
	class Object;
	class Sphere;
	class Ray;

	class Scene
	{
	protected:
		// window dimensions
		int width;
		int height;

		// camera transformation
		float yaw;
		float pitch;

		// mouse co-ordinates
		float mouseX;
		float mouseY;

		// for panning the scene on corner scroll
		bool panLeft, panRight, panTop, panBot;

		// to check for first mouse callback
		bool firstMouseCB;

		// currently picked object
		Body* pickedBody;

		// mouse body - mouseBody and pickedBody make the mouse joint pair
		Body* mouseBody;

		// true if mouse is close enough to be a joint to a body
		bool mouseJoint;

		MouseJoint mJoint;

		// store the state of pressed keys
		bool keys[1024];

		// list of objects in the scene
		std::vector<Object*> objects;

		bool debugDraw;

		// for drawing the mouse joint
		Sphere* s;
		Ray* r;

		bool editMode;

	public:
		Scene();

		virtual void OnInit(GLFWwindow* window);

		void OnWindowResize(GLFWwindow* window, int width, int height);

		void OnMouseMove(GLFWwindow* window, double x, double y);

		void OnMouseScroll(GLFWwindow* window, double dx, double dy);

		void OnMouseButton(GLFWwindow* window, int button, int action, int mods);

		virtual void OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods);

		virtual void Update();
	};
}