
#include "TestBox.h"
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
	void TestBox::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		Model* model;
		Body* body;
		glm::vec3 position, scale;
		glm::quat orientation(1, 0, 0, 0);
		float r, g, b;
		Body* A;
		Body* B;

		// Box A
		model = new Cube(glm::vec3(0.6, 0.1, 0.1));
		position = glm::vec3(0, 10, 0);
		scale = glm::vec3(4, 4, 4);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		body->SetStatic();
		objects.push_back(new Object(model, body));

		// Box B
		model = new Cube(glm::vec3(0,1,0));
		position = glm::vec3(7.09, 10, 0);
		scale = glm::vec3(1, 1, 1);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		float d = glm::length(position);
		glm::vec3 n = position/d;
		glm::vec3 dir = glm::cross(n, glm::vec3(0,0,1));
		body->SetVelocity(4.0f*dir);
		objects.push_back(new Object(model, body));
	}

	void TestBox::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
	{
		Scene::OnKeyInput(window, key, code, action, mods);
	}

	void TestBox::Update()
	{
		Body* b = objects[1]->GetBody();
		glm::vec3 position = b->GetPosition();
		float d = glm::length(position);
		glm::vec3 n = position / d;
		glm::vec3 dir = glm::cross(n, glm::vec3(0, 0, 1));
		b->SetVelocity(4.0f*dir);
		
		Scene::Update();
	}
}