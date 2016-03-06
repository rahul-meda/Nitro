
#include "Physics.h"
#include "Collider.h"
#include "ContactConstraint.h"
#include "DistanceJoint.h"
#include "PositionJoint.h"
#include "Sphere.h"
#include "Ray.h"
#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Camera.h"
#include "Object.h"
#include "Manifold.h"
#include "ClothDemo.h"
#include "BroadPhase.h"

namespace Nitro
{
	Physics::Physics()
		: pauseStep(true), advanceStep(false)
	{
		contact = new Sphere();
		normal = new Ray(glm::vec3(0), glm::vec3(1));
		box = new Cube();
	}

	Physics::~Physics()
	{

	}

	Physics& Physics::GetInstance()
	{
		static Physics instance;
		return instance;
	}

	void Physics::Init()
	{

	}

	std::vector<Manifold>& Physics::GetContactList()
	{
		return contacts;
	}

	std::vector<Joint*>& Physics::GetJoints()
	{
		return joints;
	}

	Ray* Physics::GetRay()
	{
		return normal;
	}

	Cube* Physics::GetCubeModel()
	{
		return box;
	}

	void Physics::Update(const std::vector<Object*> objects)
	{
		static const float dt = (float) 1.0f / 60.0f;
		//static const float dt = 0.001f;

		if (!pauseStep)
			Step(objects, dt);
		else
		{
			if (advanceStep)
			{
				Step(objects, dt);
				advanceStep = false;
			}
		}
	}

	void Physics::Step(const std::vector<Object*> objects, float dt)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->GetBody()->IntegrateVelocities(dt);
		}

		// warm start contacts
		for (int i = 0; i < contacts.size(); i++)
		{
			ContactConstraint c(contacts[i]);
			c.WarmStart();
		}

		contacts.clear();

		// broad phase - AABB BVH using Binay Tree
		// check for potential collisions - can have false positives

		// Construct the AABB Tree
		AABBTree tree;

		for (int i = 0; i < objects.size(); i++)
		{
			Body* body = objects[i]->GetBody();
			glm::mat3 R = body->GetRotationMatrix();

			if (body->GetCollider() == nullptr) continue;

			if (body->GetCollider()->GetPrimitiveType() == Collider::Sphere)
			{
				AABB aabb(body->GetPosition(), body->GetScale());
				tree.InsertNode(aabb, body);
			}
			else
			{
				BoxCollider* collider = static_cast<BoxCollider*>(body->GetCollider());
				float projX = collider->CalculateProjection(glm::vec3(1, 0, 0));
				float projY = collider->CalculateProjection(glm::vec3(0, 1, 0));
				float projZ = collider->CalculateProjection(glm::vec3(0, 0, 1));
				glm::vec3 aabbExt = glm::vec3(projX, projY, projZ)*1.1f; // 10% bigger bounding volume
				AABB aabb(body->GetPosition(), aabbExt);
				tree.InsertNode(aabb, body);
			}
		}

		// Draw the AABB tree
		//tree.DrawTree();

		// Compute Potential Collision Pairs
		tree.ComputePairs();

		for (int i = 0; i < tree.pairs.size(); i++)
		{
			Body* A = tree.pairs[i].A;
			int g1 = A->GetGroup();
			Collider* cA = A->GetCollider();
			if (cA == nullptr) continue;

			Body* B = tree.pairs[i].B;
			int g2 = B->GetGroup();
			Collider* cB = B->GetCollider();
			if (cB == nullptr) continue;

			if (cA->GetBody()->IsStatic() && cB->GetBody()->IsStatic()) continue;

			if (g1 != 0 && g2 != 0)
			{
				if (g1 == g2) continue;
			}

			if (cA->TestCollision(cB))
			{
				cA->DetectContacts(cB);
			}
		}

		//for (int i = 0; i < objects.size(); i++)
		//{
		//	Body* b = objects[i]->GetBody();
		//	int g1 = b->GetGroup();
		//	Collider* A = b->GetCollider();
		//	if (A == nullptr) continue;

		//	for (int j = i + 1; j < objects.size(); j++)
		//	{
		//		b = objects[j]->GetBody();
		//		int g2 = b->GetGroup();
		//		Collider* B = b->GetCollider();
		//		if (B == nullptr) continue;
		//		if (A->GetBody()->IsStatic() && B->GetBody()->IsStatic())
		//			continue;
		//		if (g1 != 0 && g2 != 0)
		//		{
		//			if (g1 == g2) continue;
		//		}
		//		if (A->TestCollision(B))
		//		{
		//			// potential collision detected
		//			// perform narrow phase - false positives are eliminated
		//			// generate contacts at touching parts
		//			A->DetectContacts(B);
		//		}
		//	}
		//}
		
		int iters = 25;
		while (iters-- > 0)
		{
			// solve contact constraints
			for (int i = 0; i < contacts.size(); i++)
			{
				ContactConstraint c(contacts[i]);
				c.Solve();
			}

			// solve joint constraints
			for (int i = 0; i < joints.size(); i++)
			{
				joints[i]->Solve();
			}
		}

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->GetBody()->IntegratePositions(dt);
		}

		// solve all the springs with the new positions to get nfa
		// calculate velocity based on the guessed acceleration
		/*std::vector<Spring> springs = ClothDemo::GetInstance().GetSprings();
		for (int i = 0; i < springs.size(); i++)
		{
			springs[i].Solve();
		}*/

		/*for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->GetBody()->IntegrateVeloityVerlet(dt);
		}*/
	}

	void Physics::DebugDraw()
	{
		glm::mat4 M(1), VP(1), MVP(1);
		VP = Camera::GetInstance().GetVPMatrix();

		for (int i = 0; i < contacts.size(); i++)
		{
			// contact point
			M = glm::translate(contacts[i].GetContact()) * glm::scale(glm::vec3(0.2));
			MVP = VP * M;
			contact->SetColor(glm::vec3(1,0,0));
			contact->SetMVP(MVP);
			contact->Render();

			// contact normal
			glm::vec3 p1 = contacts[i].GetContact();
			glm::vec3 p2 = p1 + contacts[i].GetNormal();
			normal->SetPoints(p1, p2);
			normal->SetColor(glm::vec3(0,1,0));
			normal->SetMVP(VP);
			normal->Render();

			// friction 1
			p1 = contacts[i].GetContact();
			p2 = p1 + contacts[i].GetTangent1();
			normal->SetPoints(p1, p2);
			normal->SetColor(glm::vec3(0, 0, 1));
			normal->SetMVP(VP);
			normal->Render();

			// friction 2
			p1 = contacts[i].GetContact();
			p2 = p1 + contacts[i].GetTangent2();
			normal->SetPoints(p1, p2);
			normal->SetColor(glm::vec3(0, 0, 1));
			normal->SetMVP(VP);
			normal->Render();
		}
	}

	void Physics::DrawJoints()
	{
		glm::mat4 M(1), VP(1), MVP(1);
		VP = Camera::GetInstance().GetVPMatrix();

		for (int i = 0; i < joints.size(); i++)
		{
			M = glm::translate(joints[i]->GetJoint1()) * glm::scale(glm::vec3(0.2));
			MVP = VP * M;
			contact->SetColor(glm::vec3(0, 1, 1));
			contact->SetMVP(MVP);
			contact->Render();
			M = glm::translate(joints[i]->GetJoint2()) * glm::scale(glm::vec3(0.2));
			MVP = VP * M;
			contact->SetColor(glm::vec3(0, 1, 1));
			contact->SetMVP(MVP);
			contact->Render();

			glm::vec3 p1 = joints[i]->GetJoint1();
			glm::vec3 p2 = joints[i]->GetJoint2();
			normal->SetPoints(p1, p2);
			normal->SetColor(glm::vec3(1, 1, 0));
			normal->SetMVP(VP);
			normal->Render();
		}
	}
}