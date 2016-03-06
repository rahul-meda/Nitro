
#include "ClothDemo.h"
#include "Cube.h"
#include "Sphere.h"
#include "Body.h"
#include "Object.h"
#include "Collider.h"
#include "DistanceJoint.h"
#include <glm/gtc/random.hpp>
#include "Physics.h"
#include "PositionJoint.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace Nitro
{
	ClothDemo::ClothDemo()
	{}

	ClothDemo& ClothDemo::GetInstance()
	{
		static ClothDemo instance;
		return instance;
	}

	void ClothDemo::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		mesh = new ClothMesh(glm::vec3(0.5));

		Model* model;
		Body* body;
		glm::vec3 position, scale(1);
		glm::quat orientation(1, 0, 0, 0);
		float r, g, b;
		Spring s;

		const int N = 21;

		Body* bodies[N][N];

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				// Static bodies
				if (i == 0 || j == 0 || i == N-1 || j == N-1)
				{
					model = new Sphere(glm::vec3(1));
					position = glm::vec3(3*i, 25, 3*j);
					body = new Body(position, orientation, scale);
					body->SetCollider(new SphereCollider(body, scale.x));
					body->SetStatic();
					bodies[i][j] = body;
					objects.push_back(new Object(model, body));
				}
				// Masses
				else
				{
					model = new Sphere();
					position = glm::vec3(3*i, 25, 3*j);
					r = position.x / 60.0f;
					g = (position.y + 30.0f) / 60.0f;
					b = position.z / 60.0f;
					model->SetColor(glm::vec3(r,g,b));
					body = new Body(position, orientation, scale);
					body->SetCollider(new SphereCollider(body, scale.x));
					body->SetGroup(1);
					body->SetMass(0.000125);
					bodies[i][j] = body;
					objects.push_back(new Object(model, body));
				}
			}
		}

		// Spring Connections
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				Body* b = bodies[i][j];
				if (!(b->IsStatic()))
				{
					// connect the non-static body to 8 neighbors
					// edges of the mesh will be less stiff 
					// edges are connected with one spring
					// other masses have 2 springs
					s = Spring(b, bodies[i-1][j]);
					springs.push_back(s);
					s = Spring(b, bodies[i+1][j]);
					springs.push_back(s);
					s = Spring(b, bodies[i][j-1]);
					springs.push_back(s);
					s = Spring(b, bodies[i][j+1]);
					springs.push_back(s);
					s = Spring(b, bodies[i-1][j-1]);
					springs.push_back(s);
					s = Spring(b, bodies[i+1][j-1]);
					springs.push_back(s);
					s = Spring(b, bodies[i+1][j+1]);
					springs.push_back(s);
					s = Spring(b, bodies[i-1][j+1]);
					springs.push_back(s);
				}
			}
		}

		model = new Sphere(glm::vec3(0));
		position = glm::vec3(30, 35, 30);
		scale = glm::vec3(5);
		body = new Body(position, orientation, scale);
		body->SetCollider(new SphereCollider(body, scale.x));
		body->SetMass(0.000125);
		objects.push_back(new Object(model, body));
	}

	void ClothDemo::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
	{
		Scene::OnKeyInput(window, key, code, action, mods);

		if (keys[GLFW_KEY_P])
		{
			Body* b = objects[221]->GetBody();
			b->AddForce(0.01f*glm::vec3(0,-999,0));
		}
	}

	std::vector<Spring> ClothDemo::GetSprings()
	{
		return springs;
	}

	void ClothDemo::Update()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			Object* o = objects[i];

			Model* model = o->GetModel();
			Body* body = o->GetBody();
			glm::vec3 position = body->GetPosition();
			float r, g, b;

			if (!(body->IsStatic()))
			{
				r = std::abs(position.x-30.0f) / 30.0f;
				b = std::abs(position.y - 25.0f) / 30.0f + 0.75f;
				g = std::abs(position.z - 30.0f) / 30.0f;

				model->SetColor(glm::vec3(r, g, b));
			}
		}

		std::vector<glm::vec3> points;
		glm::mat4 V = Camera::GetInstance().GetViewMatrix();
		glm::mat4 P = Camera::GetInstance().GetProjectionMatrix();
		glm::mat4 VP = P*V;

		for (int i = 0; i < springs.size(); i++)
		{
			springs[i].Solve();
			//springs[i].Render();

			glm::vec3 p1 = springs[i].GetBodyA()->GetPosition();
			glm::vec3 p2 = springs[i].GetBodyB()->GetPosition();
			points.push_back(p1);
			points.push_back(p2);
		}

		Scene::Update();

		mesh->SetPoints(points);
		mesh->SetMVP(VP);
		mesh->Render();
	}
}