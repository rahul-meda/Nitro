
#include "DominoPyramid.h"
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
	void DominoPyramid::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		Model* model;
		Body* body;
		glm::vec3 position(0,150,0);
		glm::quat orientation(1, 0, 0, 0);
		float sx = 7;
		float sy = 2;
		glm::vec3 scale(sx,sy,5);
		float r, g, b;
		std::vector<glm::vec3> positions;

		model = new Cube(glm::vec3(0.9,0.6,0.3)); // Sandy Brown :)
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		objects.push_back(new Object(model, body));

		/*position = glm::vec3(-5,-2,0);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		objects.push_back(new Object(model, body));

		position = glm::vec3(5,-2,0);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		objects.push_back(new Object(model, body));*/

		for (int r = 1; r < 25; r++)
		{
			for (int n = 1; n <= r+1; n++)
			{
				glm::vec3 pos;
				pos.x = -(1 + r)*(sx)+(r*sx / 3);
				pos.y = -(r)*(2*sy) + 150;
				pos.z = 0;
				pos.x += (n - 1)*((4*sx)-(2*sx/3));

				positions.push_back(pos);
			}
		}

		for (int i = 0; i < positions.size(); i++)
		{
			body = new Body(positions[i], orientation, scale);
			body->SetCollider(new BoxCollider(body, scale));
			objects.push_back(new Object(model, body));
		}

		/*model = new Sphere(glm::vec3(1, 0, 0));
		scale = glm::vec3(5, 5, 5);
		position = glm::vec3(-25, -20, 0);
		glm::quat orientation = glm::angleAxis(0.79f*j, glm::vec3(0, 1, 0));
		orientation = glm::quat(1, 0, 0, 0);
		body = new Body(position, orientation, scale);
		body->SetCollider(new SphereCollider(body, scale.x));
		body->SetVelocity(glm::vec3(40,0,0));
		objects.push_back(new Object(model, body));*/
	}

	void DominoPyramid::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
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