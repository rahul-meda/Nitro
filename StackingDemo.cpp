
#include "StackingDemo.h"
#include "Cube.h"
#include "Sphere.h"
#include "Body.h"
#include "Object.h"
#include "Collider.h"
#include "DistanceJoint.h"
#include <glm/gtc/random.hpp>
#include "Physics.h"

namespace Nitro
{
	void StackingDemo::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		Model* model;
		Body* body;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				for (int k = 0; k < 9; k++)
				{
					float r = glm::linearRand(0.0f, 1.0f);
					float g = glm::linearRand(0.0f, 1.0f);
					float b = glm::linearRand(0.0f, 1.0f);
					model = new Cube(glm::vec3(0.5, 0.8, 0.9));
					glm::vec3 scale = glm::vec3(3, 3, 3);
					glm::vec3 position = glm::vec3(12.0f*i, 5 + 7 * j, 12.0f*k);
					glm::quat orientation = glm::angleAxis(0.79f*j, glm::vec3(0, 1, 0));
					//glm::quat orientation = glm::quat(1, 0, 0, 0);
					body = new Body(position, orientation, scale);
					body->SetCollider(new BoxCollider(body, scale));
					objects.push_back(new Object(model, body));
				}
			}
		}
	}

	void StackingDemo::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
	{
		Scene::OnKeyInput(window, key, code, action, mods);

		if (keys[GLFW_KEY_P])
		{
			static Model* model = new Sphere(glm::vec3(1,0,0));
			static glm::vec3 scale = glm::vec3(2);
			glm::vec3 position = Camera::GetInstance().GetPosition();
			static glm::quat orientation = glm::quat(1,0,0,0);
			Body* body = new Body(position, orientation, scale);
			body->SetCollider(new SphereCollider(body, scale.x));
			glm::vec3 dir = Camera::GetInstance().GetCamZ();
			body->SetVelocity(200.0f*dir);
			objects.push_back(new Object(model, body));
		}
	}
}