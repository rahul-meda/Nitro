
#include "DominoDemo.h"
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
	void DominoDemo::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		Model* model;
		Body* body;
		float s = 4;
		float s1 = 2;
		glm::vec3 scale(s);
		glm::vec3 scale1(0.7,s1,0.2);
		glm::vec3 position(0);
		glm::quat orientation(1, 0, 0, 0);
		float r, g, b;

		// each layer outputs the positions of the next layer
		std::vector<glm::vec3> positions;

		// domino positions
		std::vector<glm::vec3> positions1;

		// position of the top-most cube of the pyramid
		positions.push_back(glm::vec3(0));

		positions1.push_back(glm::vec3(0,s+s1,0));

		// each layer of the pyramid is recursively calculated using the data of current layer

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < (i + 1)*(i + 1); j++)
			{
				model = new Cube(glm::vec3(0.9, 0.6, 0.3));
				position = positions[j];
				r = (-std::abs(position.x) + 9.0f * s) / (9.0f * s);
				g = (-std::abs(position.y) + 18.0f * s) / (18.0f * s);
				b = (-std::abs(position.z) + 9.0f * s) / (9.0f * s);
				model->SetColor(glm::vec3(r, g, b));
				body = new Body(position, orientation, scale);
				body->SetCollider(new BoxCollider(body, scale));
				objects.push_back(new Object(model, body));

				model = new Cube(glm::vec3(0));
				position = positions1[j];
				body = new Body(position, orientation, scale1);
				body->SetCollider(new BoxCollider(body, scale1));
				//objects.push_back(new Object(model, body));
			}

			positions.clear();
			positions1.clear();

			float y = 2*s * (i+1);
			float w = 0.5f*y;
			float y1 = y - s;
			float w1 = w + s;
			if ((i % 2) == 0)
			{
				for (int r = 0; r < (i + 2); r++)
				{
					for (int c = 0; c < (i + 2); c++)
					{
						positions.push_back(glm::vec3(-w+2*s*r, -y, -w+2*s*c));
						positions1.push_back(glm::vec3(-w1 + 2 * s*r+0.5*s, -y1+s1, -w1 + 2 * s*c+s));
					}
				}
			}
			else
			{
				for (int r = 0; r < (i + 2); r++)
				{
					for (int c = 0; c < (i + 2); c++)
					{
						positions.push_back(glm::vec3(-w+2*s*r, -y, -w+2*s*c));
						positions1.push_back(glm::vec3(-w1 + 2 * s*r+0.5*s, -y1+s1, -w1 + 2 * s*c+s));
					}
				}
			}
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

	void DominoDemo::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
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