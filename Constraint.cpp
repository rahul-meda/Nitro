
#include "Constraint.h"
#include "Body.h"

namespace Nitro
{
	Constraint::Constraint()
	{}

	Constraint::Constraint(Body* A, Body* B)
		: A(A), B(B)
	{

	}

	float Constraint::CalculateBias()
	{
		return 0;
	}

	float Constraint::CalculateEffectiveMass(const Jacobian& J)
	{
		float em;

		float im1 = A->GetInvMass();
		float im2 = B->GetInvMass();
		glm::mat3 iI1 = A->GetInvInertia();
		glm::mat3 iI2 = B->GetInvInertia();

		em = glm::dot(J.L1, J.L1)*im1;
		em += glm::dot(J.L2, J.L2)*im2;
		em += glm::dot(J.A1, iI1*J.A1);
		em += glm::dot(J.A2, iI2*J.A2);

		return em;
	}

	float Constraint::CalculateLagrangian(const Jacobian& J, const float b, const float em)
	{
		float lambda;

		glm::vec3 v1 = A->GetVelocity();
		glm::vec3 w1 = A->GetAngularVelocity();
		glm::vec3 v2 = B->GetVelocity();
		glm::vec3 w2 = B->GetAngularVelocity();

		float JVi;

		JVi = glm::dot(J.L1, v1);
		JVi += glm::dot(J.A1, w1);
		JVi += glm::dot(J.L2, v2);
		JVi += glm::dot(J.A2, w2);

		lambda = -(JVi + b) / em;

		return lambda;
	}

	void Constraint::ApplyImpulse(const Jacobian& J, const float lambda)
	{
		float im1 = A->GetInvMass();
		float im2 = B->GetInvMass();
		glm::mat3 iI1 = A->GetInvInertia();
		glm::mat3 iI2 = B->GetInvInertia();

		glm::mat3 iM1(1);
		iM1[0][0] = im1;
		iM1[1][1] = im1;
		iM1[2][2] = im1;
		glm::mat3 iM2(1);
		iM2[0][0] = im2;
		iM2[1][1] = im2;
		iM2[2][2] = im2;

		glm::vec3 v1 = A->GetVelocity();
		glm::vec3 w1 = A->GetAngularVelocity();
		glm::vec3 v2 = B->GetVelocity();
		glm::vec3 w2 = B->GetAngularVelocity();
		v1 += lambda*J.L1 * iM1;
		w1 += lambda*J.A1 * iI1;
		v2 += lambda*J.L2 * iM2;
		w2 += lambda*J.A2 * iI2;
		A->SetVelocity(v1);
		A->SetAngularVelocity(w1);
		B->SetVelocity(v2);
		B->SetAngularVelocity(w2);
	}
}