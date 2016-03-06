
#include "PendulumDemo.h"
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
	void PendulumDemo::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		Model* model;
		Body* body;
		glm::vec3 position, scale;
		glm::quat orientation(1,0,0,0);
		float r, g, b;
		Body* A;
		Body* B;

		// Ceiling
		model = new Cube(glm::vec3(0.6,0.1,0.1));
		position = glm::vec3(0,25,0);
		scale = glm::vec3(10,1,1);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		body->SetStatic();
		objects.push_back(new Object(model, body));

		// Sphere Pendulum of length L //
		// Clamp
		model = new Sphere(glm::vec3(0.7, 0.7, 0.7));
		scale = glm::vec3(0.2);
		position = glm::vec3(-10, 24, 0);
		A = new Body(position, orientation, scale);
		//A->SetCollider(new SphereCollider(A, scale.x));
		A->SetStatic();
		objects.push_back(new Object(model, A));
		// Sphere Mass
		r = glm::linearRand(0.0f, 1.0f);
		g = glm::linearRand(0.0f, 1.0f);
		b = glm::linearRand(0.0f, 1.0f);
		model = new Sphere(glm::vec3(1,1,0));
		scale = glm::vec3(2);
		position = glm::vec3(-17.14, 17, 0);
		B = new Body(position, orientation, scale);
		B->SetCollider(new SphereCollider(B, scale.x));
		objects.push_back(new Object(model, B));
		// Joint - attaching clamp and sphere center-to-center
		Joint* joint = new DistanceJoint(A, B);
		joint->SetJoint1(glm::vec3(0));
		joint->SetJoint2(glm::vec3(0));
		Physics::GetInstance().GetJoints().push_back(joint);

		for (int i = 0; i < 3; i++)
		{
			// Sphere Pendulum of length L
			// Clamp
			model = new Sphere(glm::vec3(0.7, 0.7, 0.7));
			scale = glm::vec3(0.2);
			position = glm::vec3(-6+4*i, 24, 0);
			A = new Body(position, orientation, scale);
			A->SetStatic();
			objects.push_back(new Object(model, A));
			// Sphere Mass
			r = glm::linearRand(0.0f, 1.0f);
			g = glm::linearRand(0.0f, 1.0f);
			b = glm::linearRand(0.0f, 1.0f);
			model = new Sphere(glm::vec3(1, 1, 0));
			scale = glm::vec3(2);
			position = glm::vec3(-6+4*i, 14, 0);
			B = new Body(position, orientation, scale);
			B->SetCollider(new SphereCollider(B, scale.x));
			objects.push_back(new Object(model, B));
			// Joint - attaching clamp and sphere center-to-center
			joint = new DistanceJoint(A, B);
			joint->SetJoint1(glm::vec3(0));
			joint->SetJoint2(glm::vec3(0));
			Physics::GetInstance().GetJoints().push_back(joint);
		}

		model = new Cube(glm::vec3(0.6, 0.1, 0.1));
		position = glm::vec3(0, 25, 20);
		scale = glm::vec3(10, 1, 1);
		body = new Body(position, orientation, scale);
		body->SetCollider(new BoxCollider(body, scale));
		body->SetStatic();
		objects.push_back(new Object(model, body));
		// Cube Pendulum of length L //
		// Clamp
		model = new Sphere(glm::vec3(0.7, 0.7, 0.7));
		scale = glm::vec3(0.2);
		position = glm::vec3(-10, 24, 20);
		Body* A1 = new Body(position, orientation, scale);
		A1->SetStatic();
		objects.push_back(new Object(model, A1));
		// Cube Mass
		r = glm::linearRand(0.0f, 1.0f);
		g = glm::linearRand(0.0f, 1.0f);
		b = glm::linearRand(0.0f, 1.0f);
		model = new Cube(glm::vec3(0.5, 0.8, 0.9));
		scale = glm::vec3(2);
		position = glm::vec3(-13, 15, 20);
		Body* B1 = new Body(position, orientation, scale);
		B1->SetCollider(new BoxCollider(B1, scale));
		objects.push_back(new Object(model, B1));
		// Joint - attaching clamp and Cube center-to-center
		joint = new DistanceJoint(A1, B1);
		joint->SetJoint1(glm::vec3(0));
		joint->SetJoint2(glm::vec3(0,2,0));
		Physics::GetInstance().GetJoints().push_back(joint);

		// Cube Pendulum of length L //
		// Clamp
		model = new Sphere(glm::vec3(0.7, 0.7, 0.7));
		scale = glm::vec3(0.2);
		position = glm::vec3(-2, 24, 20);
		Body* A2 = new Body(position, orientation, scale);
		A2->SetStatic();
		objects.push_back(new Object(model, A2));
		// Cube Mass
		r = glm::linearRand(0.0f, 1.0f);
		g = glm::linearRand(0.0f, 1.0f);
		b = glm::linearRand(0.0f, 1.0f);
		model = new Cube(glm::vec3(0.5, 0.8, 0.9));
		scale = glm::vec3(2);
		position = glm::vec3(0, 13, 18);
		Body* B2 = new Body(position, orientation, scale);
		B2->SetCollider(new BoxCollider(B2, scale));
		objects.push_back(new Object(model, B2));
		// Joint - attaching clamp and Cube center-to-center
		joint = new DistanceJoint(A2, B2);
		joint->SetJoint1(glm::vec3(0));
		joint->SetJoint2(glm::vec3(-2, 2, 2));
		Physics::GetInstance().GetJoints().push_back(joint);

		// Cube Pendulum of zero length - only rotation about the pivot
		// Clamp
		model = new Sphere(glm::vec3(0.7, 0.7, 0.7));
		scale = glm::vec3(0.2);
		position = glm::vec3(6, 24, 20);
		Body* A3 = new Body(position, orientation, scale);
		//A->SetCollider(new SphereCollider(A, scale.x));
		A3->SetStatic();
		objects.push_back(new Object(model, A3));
		// Cube
		r = glm::linearRand(0.0f, 1.0f);
		g = glm::linearRand(0.0f, 1.0f);
		b = glm::linearRand(0.0f, 1.0f);
		model = new Cube(glm::vec3(0.5, 0.8, 0.9));
		scale = glm::vec3(2);
		position = glm::vec3(6,21.171,20);
		orientation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1,0,1));
		Body* B3 = new Body(position, orientation, scale);
		B3->SetCollider(new BoxCollider(B3, scale));
		//B->SetGroup(1);
		objects.push_back(new Object(model, B3));
		// Joint - attaching cube's vertex to the clamp
		joint = new PositionJoint(A3, B3);
		joint->SetJoint1(glm::vec3(0));
		joint->SetJoint2(glm::vec3(2,2,2));
		Physics::GetInstance().GetJoints().push_back(joint);

		joint = new DistanceJoint(B1, B2);
		joint->SetJoint1(glm::vec3(2,-2,2));
		joint->SetJoint2(glm::vec3(-2,-2,2));
		//Physics::GetInstance().GetJoints().push_back(joint);

		joint = new DistanceJoint(B2, B3);
		joint->SetJoint1(glm::vec3(2,2,2));
		joint->SetJoint2(glm::vec3(-2,2,2));
		Physics::GetInstance().GetJoints().push_back(joint);
	}

	void PendulumDemo::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
	{
		Scene::OnKeyInput(window, key, code, action, mods);
	}
}