
#include "Explosion.h"
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
	void Explosion::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		Model* model;
		Body* body;
		glm::vec3 position(0);
		glm::quat orientation(1, 0, 0, 0);
		float sx = 9;
		float sy = 3;
		float sz = 3;
		glm::vec3 scale(sx, sy, sz);
		float r, g, b;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> scales;

		model = new Cube(glm::vec3(0.5, 0.8, 0.9));

		for (int i = 0; i < 1; i++)
		{
			for (int j = 0; j < 25; j++)
			{
				positions.push_back(glm::vec3(20 * i, 9+18*j, 0));
				scales.push_back(glm::vec3(10.0f-0.1f*j, 9, 10.0f-0.1f*j));
			}
		}

		for (int i = 0; i < positions.size(); i++)
		{
			body = new Body(positions[i], orientation, scales[i]);
			body->SetCollider(new BoxCollider(body, scales[i]));
			body->SetMass(100.0f - 4*i);
			objects.push_back(new Object(model, body));
		}
	}

	void Explosion::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
	{
		Scene::OnKeyInput(window, key, code, action, mods);

		if (keys[GLFW_KEY_P])
		{
			static Model* model = new Sphere(glm::vec3(1, 0, 0));
			static glm::vec3 scale = glm::vec3(3);
			glm::vec3 position = Camera::GetInstance().GetPosition();
			position.y -= 50;
			static glm::quat orientation = glm::quat(1, 0, 0, 0);
			Body* body = new Body(position, orientation, scale);
			body->SetCollider(new SphereCollider(body, scale.x));
			glm::vec3 dir = Camera::GetInstance().GetCamZ();
			body->SetVelocity(200.0f*dir);
			objects.push_back(new Object(model, body));
		}
	}
}