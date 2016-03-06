
#include "BilliardsDemo.h"
#include "Cube.h"
#include "Sphere.h"
#include "Body.h"
#include "Object.h"
#include "Collider.h"
#include "DistanceJoint.h"
#include <glm/gtc/random.hpp>
#include "Physics.h"
#include "PositionJoint.h"

namespace Nitro
{
	void BilliardsDemo::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		Model* model;
		Body* body;

		// Table
		model = new Cube(glm::vec3(0, 0.7, 0));
		glm::vec3 position = glm::vec3(0,1,0);
		glm::vec3 scale = glm::vec3(10, 1, 25);
		glm::quat orientation = glm::quat(1,0,0,0);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		body->SetStatic();
		objects.push_back(new Object(model, body));
		model = new Cube(glm::vec3(0, 1, 0));
		position = glm::vec3(-10, 1, 0);
		scale = glm::vec3(1, 4, 25);
		orientation = glm::quat(1, 0, 0, 0);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		body->SetStatic();
		objects.push_back(new Object(model, body));
		model = new Cube(glm::vec3(0, 1, 0));
		position = glm::vec3(10, 1, 0);
		scale = glm::vec3(1, 4, 25);
		orientation = glm::quat(1, 0, 0, 0);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		body->SetStatic();
		objects.push_back(new Object(model, body));
		model = new Cube(glm::vec3(0, 1, 0));
		position = glm::vec3(0, 1, -25);
		scale = glm::vec3(10, 4, 1);
		orientation = glm::quat(1, 0, 0, 0);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		body->SetStatic();
		objects.push_back(new Object(model, body));
		model = new Cube(glm::vec3(0, 1, 0));
		position = glm::vec3(0, 1, 25);
		scale = glm::vec3(10, 4, 1);
		orientation = glm::quat(1, 0, 0, 0);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		body->SetStatic();
		objects.push_back(new Object(model, body));

		Reset();
	}

	void BilliardsDemo::Reset()
	{
		Model* model;
		Body* body;

		// Balls
		float r, g, b;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < i + 1; j++)
			{
				r = glm::linearRand(0.0f, 1.0f);
				g = glm::linearRand(0.0f, 1.0f);
				b = glm::linearRand(0.0f, 1.0f);
				model = new Sphere(glm::vec3(r, g, b));
				glm::vec3 scale = glm::vec3(1);
				glm::vec3 position = glm::vec3((-i + 2 * j), 3, -1.73*i);
			    glm::quat orientation = glm::quat(1, 0, 0, 0);
				body = new Body(position, orientation, scale);
				body->SetCollider(new SphereCollider(body, scale.x));
				objects.push_back(new Object(model, body));
			}
		}
		model = new Sphere(glm::vec3(1));
	    glm::vec3 scale = glm::vec3(1);
		glm::vec3 position = glm::vec3(0, 3, 16);
		glm::quat orientation = glm::quat(1, 0, 0, 0);
		body = new Body(position, orientation, scale);
		body->SetMass(125);
		body->SetCollider(new SphereCollider(body, scale.x));
		body->SetVelocity(glm::vec3(0, 0, -50));
		objects.push_back(new Object(model, body));
	}

	void BilliardsDemo::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
	{
		Scene::OnKeyInput(window, key, code, action, mods);

		if (keys[GLFW_KEY_R])
			Reset();
	}
}