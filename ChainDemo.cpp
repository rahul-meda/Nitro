//
//#include "JointsDemo.h"
//#include "Cube.h"
//#include "Sphere.h"
//#include "Body.h"
//#include "Object.h"
//#include "Collider.h"
//#include "DistanceJoint.h"
//#include <glm/gtc/random.hpp>
//#include "Physics.h"
//#include "RevoluteJoint.h"
//
//namespace Nitro
//{
//	void JointsDemo::OnInit(GLFWwindow* window)
//	{
//		Scene::OnInit(window);
//
//		Model* model;
//		Body* body;
//
//		// Inclined Plane
//		model = new Cube(glm::vec3(1, 0, 0));
//		glm::vec3 position = glm::vec3(-30, 20, 0);
//		glm::vec3 scale = glm::vec3(25, 0.25, 10);
//		glm::quat orientation = glm::angleAxis(-0.45f, glm::vec3(0, 0, 1));
//		body = new Body(position, orientation, scale);
//		body->SetCollider(new BoxCollider(body, scale));
//		body->SetStatic();
//		objects.push_back(new Object(model, body));
//
//		// Distance Joints
//		float r = glm::linearRand(0.0f, 1.0f);
//		float g = glm::linearRand(0.0f, 1.0f);
//		float b = glm::linearRand(0.0f, 1.0f);
//		model = new Cube(glm::vec3(r, g, b));
//		scale = glm::vec3(2, 2, 2);
//		position = glm::vec3(-40, 40, 0);
//		orientation = glm::quat(1, 0, 0, 0);
//		Body* A = new Body(position, orientation, scale);
//		A->SetCollider(new BoxCollider(A, scale));
//		objects.push_back(new Object(model, A));
//
//		r = glm::linearRand(0.0f, 1.0f);
//		g = glm::linearRand(0.0f, 1.0f);
//		b = glm::linearRand(0.0f, 1.0f);
//		model = new Cube(glm::vec3(r, g, b));
//		scale = glm::vec3(2, 2, 2);
//		position = glm::vec3(7.9 - 40, 40, 0);
//		orientation = glm::quat(1, 0, 0, 0);
//		Body* B = new Body(position, orientation, scale);
//		B->SetCollider(new BoxCollider(B, scale));
//		objects.push_back(new Object(model, B));
//
//		Joint* joint = new DistanceJoint(A, B);
//		joint->SetJoint1(glm::vec3(2, 0, 0));
//		joint->SetJoint2(glm::vec3(-2, 0, 0));
//
//		Physics::GetInstance().GetJoints().push_back(joint);
//
//		// Revolute Joints
//		r = glm::linearRand(0.0f, 1.0f);
//		g = glm::linearRand(0.0f, 1.0f);
//		b = glm::linearRand(0.0f, 1.0f);
//		model = new Cube(glm::vec3(r, g, b));
//		scale = glm::vec3(2, 2, 2);
//		position = glm::vec3(0, 7, 0);
//		orientation = glm::quat(1, 0, 0, 0);
//		A = new Body(position, orientation, scale);
//		A->SetCollider(new BoxCollider(A, scale));
//		A->SetGroup(1);
//		objects.push_back(new Object(model, A));
//
//		r = glm::linearRand(0.0f, 1.0f);
//		g = glm::linearRand(0.0f, 1.0f);
//		b = glm::linearRand(0.0f, 1.0f);
//		model = new Cube(glm::vec3(r, g, b));
//		scale = glm::vec3(1, 0.1, 1);
//		position = glm::vec3(3, 9, 3);
//		orientation = glm::quat(1, 0, 0, 0);
//		B = new Body(position, orientation, scale);
//		B->SetCollider(new BoxCollider(B, scale));
//		B->SetGroup(1);
//		objects.push_back(new Object(model, B));
//
//		joint = new RevoluteJoint(A, B);
//		joint->SetJoint1(glm::vec3(2, 2, 2));
//		joint->SetJoint2(glm::vec3(-1, 0, -1));
//
//		Physics::GetInstance().GetJoints().push_back(joint);
//	}
//
//	void JointsDemo::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
//	{
//		Scene::OnKeyInput(window, key, code, action, mods);
//	}
//}