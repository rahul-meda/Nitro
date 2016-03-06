
#include "Domino1.h"
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
	void Domino1::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		Model* model;
		Body* body;
		glm::vec3 position(0);
		glm::quat orientation(1, 0, 0, 0);
		glm::vec3 scale(1, 10, 5);
		float r, g, b;
		model = new Cube(glm::vec3(0.5,0.8,0.9));

		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 50; i++)
			{
				position = glm::vec3(-100 + 5 * i, 10, 12*j);
				body = new Body(position, orientation, scale);
				body->SetCollider(new BoxCollider(body, scale));
				objects.push_back(new Object(model, body));
			}
		}

		for (int i = 0; i < 3; i++)
		{
			model = new Sphere(glm::vec3(0.5, 0.8, 0.9));
			position = glm::vec3(-125, 20, 12 * i);
			body = new Body(position, orientation, glm::vec3(3));
			body->SetCollider(new SphereCollider(body, 3));
			body->SetVelocity(glm::vec3(25,0,0));
			objects.push_back(new Object(model, body));
		}
	}

	void Domino1::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
	{
		Scene::OnKeyInput(window, key, code, action, mods);

		if (keys[GLFW_KEY_P])
		{
			Body* b = objects[3]->GetBody();
			float m = (float)1.0f / b->GetInvMass();
			b->AddForce(m*glm::vec3(100, 0, 0));
		}
	}
}