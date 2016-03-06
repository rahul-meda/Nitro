
#include "Spring.h"
#include <glm/glm.hpp>
#include "Body.h"
#include "Physics.h"
#include "Ray.h"
#include "Camera.h"

namespace Nitro
{
	Spring::Spring()
	{}

	Spring::Spring(Body* A, Body* B)
		: A(A), B(B)
	{
		L = glm::distance(A->GetPosition(), B->GetPosition());
	}

	Body* Spring::GetBodyA()
	{
		return A;
	}

	Body* Spring::GetBodyB()
	{
		return B;
	}

	void Spring::Render()
	{
		glm::mat4 VP(1);
		VP = Camera::GetInstance().GetVPMatrix();

		glm::vec3 p1 = A->GetPosition();
		glm::vec3 p2 = B->GetPosition();
		Ray* r = Physics::GetInstance().GetRay();
		r->SetPoints(p1, p2);
		r->SetColor(glm::vec3(0.5f));
		r->SetMVP(VP);
		r->Render();
	}

	void Spring::Solve()
	{
		if (A->IsStatic() && B->IsStatic())
			return;

		glm::vec3 p1 = A->GetPosition();
		glm::vec3 p2 = B->GetPosition();

		// current length od spring
		float l = glm::distance(p1, p2);
		// stretch of the spring
		float s = l - L;
		// unit direction vector of the spring 
		glm::vec3 d = (p2 - p1) / l;

		// damping ratio
		float dr = 0.1f;
		// damping coefficient
		float c = dr * 2.0f * std::sqrtf(MASS*K);
		glm::vec3 vRel = B->GetVelocity() - A->GetVelocity();
		// spring force
		glm::vec3 f = (K*s + c*(glm::dot(vRel, d)))*d;

		A->AddForce(f);
		B->AddForce(-f);
	}
}