
#include "Scene.h"
#include <iostream>
#include <algorithm>
#include "Object.h"
#include "Grid.h"
#include "Collider.h"
#include "Physics.h"
#include "DistanceJoint.h"
#include "MouseJoint.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Sphere.h"
#include "Ray.h"
#include "Cube.h"

namespace Nitro
{
	Scene::Scene()
		: mouseJoint(false), firstMouseCB(false), 
		debugDraw(false), editMode(false)
	{
		panLeft = panRight = panBot = panTop = false;
	}

	void Scene::OnInit(GLFWwindow* window)
	{
		glfwGetWindowSize(window, &width, &height);

		glClearColor(0.5, 0.5, 0.5, 0);

		// draw the pixel only if the object is closer to the viewer
		glEnable(GL_DEPTH_TEST); // enables depth-testing
		glDepthFunc(GL_LESS);    // interpret smaller values as closer

		Model* model;
		Body* body;

		s = new Sphere();
		r = new Ray(glm::vec3(0), glm::vec3(1));

		// Grid
		/*model = new Grid(100, 100);
		glm::vec3 position(0);
		glm::quat orientation(1, glm::vec3(0));
		glm::vec3 scale(10);
		body = new Body(position, orientation, scale);
		body->SetCollider(new PlaneCollider(body, glm::vec3(0, 1, 0)));
		body->SetStatic();
		objects.push_back(new Object(model, body));*/

		// Ground
		model = new Cube(glm::vec3(0.9, 0.6, 0.3));
		glm::vec3 scale = glm::vec3(500, 5, 500);
		glm::vec3 position = glm::vec3(0,-5,0);
		glm::quat orientation = glm::quat(1,0,0,0);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		body->SetStatic();
		objects.push_back(new Object(model, body));

		Camera::GetInstance().SetProjection(45.0f, (float)width/height);

		glm::vec3 eye = glm::vec3(7);
		glm::vec3 o = glm::vec3(0);
		Camera::GetInstance().SetPosition(eye);
		glm::vec3 look = eye - o;
		look = glm::normalize(look);

		yaw = glm::degrees((float)(atan2(look.z, look.x) + 3.14));
		pitch = glm::degrees(asin(look.y));

		Camera::GetInstance().Rotate(yaw, pitch, 0);
	}

	void Scene::OnWindowResize(GLFWwindow* window, int width, int height)
	{
		this->width = width;
		this->height = height;
		glViewport(0, 0, width, height);
		Camera::GetInstance().SetProjection(45.0f, (float)width / height);
	}

	void Scene::OnMouseMove(GLFWwindow* window, double x, double y)
	{
		if (firstMouseCB)
		{
			mouseX = x;
			mouseY = y;
			firstMouseCB = false;
		}

		float dx = mouseX - x;
		float dy = y - mouseY;
		mouseX = x;
		mouseY = y;

		float sensitivity = 0.1;
		dx *= sensitivity;
		dy *= sensitivity;

		yaw += dx;
		pitch += dy;

		if (mouseX < 20) panLeft = true;
		else panLeft = false;
		if (mouseX > width - 20) panRight = true;
		else panRight = false;
		if (mouseY < 20) panBot = true;
		else panBot = false;
		if (mouseY > height - 20) panTop = true;
		else panTop = false;

		if (pitch > 90) pitch = 90;
		if (pitch < -90) pitch = -90;

		Camera::GetInstance().Rotate(yaw, pitch, 0);

		if (mouseJoint)
		{
			//// read pixel depth at mouse click position - gives screen space 3D point
			//float mouseZ = 0;
			//glReadPixels(mouseX, height - mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouseZ);

			//// convert the point from screen space to world space
			//glm::vec3 sMouse = glm::vec3(mouseX, height - mouseY, mouseZ);
			//glm::mat4 MV = Camera::GetInstance().GetViewMatrix();
			//glm::mat4 P = Camera::GetInstance().GetProjectionMatrix();
			//glm::vec3 wMouse = glm::unProject(sMouse, MV, P, glm::vec4(0, 0, width, height));
			//glm::vec3 anchorW = mJoint.GetJoint1();

			//float jointLength = glm::distance2(wMouse, anchorW);
			//if (jointLength > 20)
			//	mouseJoint = false;
			//else
			//	mJoint.SetJoint2(wMouse);

			glm::vec3 mouse = mJoint.GetJoint2();
			/*if (dx*dx + dy*dy > 9)
			{
				mouseJoint = false;
				return;
			}
			else*/
			{
				mouse.x -= dx;
				mouse.y -= dy;
				mJoint.SetJoint2(mouse);
			}
		}
	}

	void Scene::OnMouseScroll(GLFWwindow* window, double dx, double dy)
	{
		float fov = Camera::GetInstance().GetFOV();
		fov -= dy*0.079;

		if (fov > 90) fov = 90;
		if (fov < 30) fov = 30;
		
		Camera::GetInstance().SetFOV(fov);
	}

	void Scene::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			// read pixel depth at mouse click position - gives screen space 3D point
			float mouseZ = 0;
			glReadPixels(mouseX, height - mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouseZ);

			// convert the point from screen space to world space
			glm::vec3 sMouse = glm::vec3(mouseX, height - mouseY, mouseZ);
			glm::mat4 MV = Camera::GetInstance().GetViewMatrix();
			glm::mat4 P = Camera::GetInstance().GetProjectionMatrix();
			glm::vec3 wMouse = glm::unProject(sMouse, MV, P, glm::vec4(0, 0, width, height));

			glm::vec3 p = objects[1]->GetBody()->GetPosition();
			std::printf("Clicked pos: %f, %f, %f \n", wMouse.x, wMouse.y, wMouse.z);
			std::printf("Cube1 pos: %f, %f, %f \n", p.x, p.y, p.z);

			// check if mouse point is in body
			for (int i = 0; i < objects.size(); i++)
			{
				Collider* c = objects[i]->GetBody()->GetCollider();
				if (c == nullptr)
					continue;
				if (c->GetBody()->IsStatic())
					continue;

				if (c->TestPoint(wMouse))
				{
					pickedBody = objects[i]->GetBody();
					glm::vec3 anchor = pickedBody->WorldToLocal(wMouse);
					mouseBody = new Body(wMouse, glm::quat(1,0,0,0), glm::vec3(0), true);

					mJoint = MouseJoint(pickedBody, mouseBody);
					mJoint.SetJoint1(anchor);
					mJoint.SetJoint2(wMouse);

					mouseJoint = true;

					std::cout << "selected index: " << i << std::endl;
				}
			}
		}
		else if (action == GLFW_RELEASE)
		{
			mouseJoint = false;
		}
	}

	void Scene::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		if (key == GLFW_KEY_RIGHT_ALT && action == GLFW_PRESS)
			debugDraw = !debugDraw;

		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				keys[key] = true;
			else if (action == GLFW_RELEASE)
				keys[key] = false;
		}

		if (keys[GLFW_KEY_W])
			Camera::GetInstance().Move(Camera::GetInstance().GetCamZ());
		if (keys[GLFW_KEY_S])
			Camera::GetInstance().Move(-Camera::GetInstance().GetCamZ());
		if (keys[GLFW_KEY_A])
			Camera::GetInstance().Move(-Camera::GetInstance().GetCamX());
		if (keys[GLFW_KEY_D])
			Camera::GetInstance().Move(Camera::GetInstance().GetCamX());
		if (keys[GLFW_KEY_Q])
			Camera::GetInstance().Move(Camera::GetInstance().GetCamY());
		if (keys[GLFW_KEY_Z])
			Camera::GetInstance().Move(-Camera::GetInstance().GetCamY());

		if (keys[GLFW_KEY_E])
			editMode = !editMode;

		static const float s = 0.07f;
		if (editMode && pickedBody != nullptr)
		{
			Physics::GetInstance().pauseStep = true;
			if (keys[GLFW_KEY_UP])
				pickedBody->SetPosition(pickedBody->GetPosition() + s*glm::vec3(0, 1, 0));
			if (keys[GLFW_KEY_DOWN])
				pickedBody->SetPosition(pickedBody->GetPosition() + s*glm::vec3(0, -1, 0));
			if (keys[GLFW_KEY_LEFT])
				pickedBody->SetPosition(pickedBody->GetPosition() + s*glm::vec3(-1, 0, 0));
			if (keys[GLFW_KEY_RIGHT])
				pickedBody->SetPosition(pickedBody->GetPosition() + s*glm::vec3(1, 0, 0));
			if (keys[GLFW_KEY_KP_ADD])
				pickedBody->SetPosition(pickedBody->GetPosition() + s*glm::vec3(0, 0, 1));
			if (keys[GLFW_KEY_KP_SUBTRACT])
				pickedBody->SetPosition(pickedBody->GetPosition() + s*glm::vec3(0, 0, -1));
			if (keys[GLFW_KEY_KP_6])
				pickedBody->AddOrientation(s, glm::vec3(1, 0, 0));
			if (keys[GLFW_KEY_KP_8])
				pickedBody->AddOrientation(s, glm::vec3(0, 1, 0));
			if (keys[GLFW_KEY_KP_5])
				pickedBody->AddOrientation(s, glm::vec3(0, 0, 1));
		}

		if (keys[GLFW_KEY_SPACE])
		{
			Physics::GetInstance().pauseStep = !(Physics::GetInstance().pauseStep);
		}
		if (keys[GLFW_KEY_N])
		{
			Physics::GetInstance().advanceStep = !(Physics::GetInstance().advanceStep);
		}
	}

	void Scene::Update()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 T(1), R(1), S(1), M(1), MVP(1);
		glm::mat4 V = Camera::GetInstance().GetViewMatrix();
		glm::mat4 P = Camera::GetInstance().GetProjectionMatrix();
		glm::mat4 VP = P*V;
		Model* m;

		// Physics Update
		Physics::GetInstance().Update(objects);

		if (mouseJoint)
		{
			// draw mouse joint
			M = glm::translate(mJoint.GetJoint1())*glm::scale(glm::vec3(0.2));
			MVP = VP * M;
			s->SetColor(glm::vec3(0, 1, 1));
			s->SetMVP(MVP);
			s->Render();
			M = glm::translate(mJoint.GetJoint2()) * glm::scale(glm::vec3(0.2));
			MVP = VP * M;
			s->SetColor(glm::vec3(1, 1, 0));
			s->SetMVP(MVP);
			s->Render();

			glm::vec3 p1 = mJoint.GetJoint1();
			glm::vec3 p2 = mJoint.GetJoint2();
			r->SetPoints(p1, p2);
			r->SetColor(glm::vec3(1, 1, 0));
			r->SetMVP(VP);
			r->Render();

			mJoint.Solve();
		}

		Physics::GetInstance().DrawJoints();

		// Graphics update
		for (int i = 0; i < objects.size(); i++)
		{
			Object* o = objects[i];

			m = o->GetModel();
			Body* b = o->GetBody();
			T = glm::translate(b->GetPosition());
			R = glm::toMat4(glm::normalize(b->GetOrientation()));
			S = glm::scale(b->GetScale());
			M = T * R * S;
			MVP = VP * M;
			m->SetMVP(MVP);
			m->Render();
		}

		// Draw collision points and normals
		if (debugDraw)
		{
			// wire frame mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			// debug draw collision data and joints
			Physics::GetInstance().DebugDraw();
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		if (panLeft)
		{
			yaw += 1;
			Camera::GetInstance().Rotate(yaw, pitch, 0);
		}
		if (panRight)
		{
			yaw -= 1;
			Camera::GetInstance().Rotate(yaw, pitch, 0);
		}
		if (panTop)
		{
			pitch += 1;
			Camera::GetInstance().Rotate(yaw, pitch, 0);
		}
		if (panBot)
		{
			pitch -= 1;
			Camera::GetInstance().Rotate(yaw, pitch, 0);
		}
	}
}