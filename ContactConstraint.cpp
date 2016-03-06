
#include "ContactConstraint.h"
#include "Manifold.h"
#include <glm/glm.hpp>

namespace Nitro
{
	ContactConstraint::ContactConstraint(const Manifold& m)
		: Constraint(m.GetBodyA(), m.GetBodyB())
	{
		this->m = m;
	}

	Jacobian ContactConstraint::CalculateJacobian()
	{
		glm::vec3 n = m.GetNormal();
		glm::vec3 r1 = m.GetContact() - m.GetBodyA()->GetPosition();
		glm::vec3 r2 = m.GetContact() - m.GetBodyB()->GetPosition();

		Jacobian J(-n, -glm::cross(r1, n), n, glm::cross(r2, n));

		return J;
	}

	float ContactConstraint::CalculateBias()
	{
		float b;

		// Ref: http://allenchou.net/2013/12/game-physics-resolution-contact-constraints/
		// Baumgarte Stabilization to solve penetration
		// Constraint Solver just stops the bodies from penetrating
		// It doesn't actually push them apart
		// Restitution is added to bias, to bounce the bodies apart
		// 0<=B<1(close to 0). B is simulation dependent. Must be tweaked by experimenting. 
		//Start with values close to 0
		float B = 0.179f;
		float d = m.GetPenetration();
		b = -B*(60.0f)*std::fmaxf(d - 0.002f, 0.0f); // position slop

		// add restitution term to the bias
		float e = 0.25f;
		glm::vec3 vRel = m.CalculateRelativeVelocity();
		float vApproach = glm::dot(vRel, m.GetNormal());
		b += e*std::fminf(0.0f, vApproach + 1.0f); // velocity slop

		return b;
	}

	void ContactConstraint::WarmStart()
	{
		Jacobian J = CalculateJacobian();

		ApplyImpulse(J, m.GetImpulseSumN()*0.05f);

		glm::vec3  t1, t2;
		glm::vec3 wY(0, 1, 0);
		glm::vec3 wX(1, 0, 0);
		glm::vec3 n = m.GetNormal();

		if (glm::dot(n, wX) > 0.01f)
			t1 = glm::cross(n, wY);
		else
			t1 = glm::cross(n, wX);

		if (glm::length2(t1) == 0) return;

		t1 = glm::normalize(t1);
		m.SetTangent1(t1);
		glm::vec3 r1 = m.GetContact() - m.GetBodyA()->GetPosition();
		glm::vec3 r2 = m.GetContact() - m.GetBodyB()->GetPosition();
		Jacobian Jt1(-t1, -glm::cross(r1, t1), t1, glm::cross(r2, t1));
		ApplyImpulse(Jt1, m.GetImpulseSumT1()*0.05f);

		t2 = glm::cross(t1, n);
		t2 = glm::normalize(t2);
		m.SetTangent2(t2);
		Jacobian Jt2(-t2, -glm::cross(r1, t2), t2, glm::cross(r2, t2));
		ApplyImpulse(Jt2, m.GetImpulseSumT2()*0.05f);
	}

	void ContactConstraint::Solve()
	{
		glm::vec3 vRel = m.CalculateRelativeVelocity();
		float vApproach = glm::dot(vRel, m.GetNormal());

		if (vApproach > 0)
			return;

		Jacobian J = CalculateJacobian();

		float b = CalculateBias();

		// calculate effective "mass" of the "constraint"
		float em = CalculateEffectiveMass(J);

		// calculate lambda
		float lambda = CalculateLagrangian(J, b, em);

		// clamp lambda such that total impulse over several iterations
		// in a single time step is never negative
		float old = m.GetImpulseSumN();
		//float sum = glm::clamp(lambda + old, 0.0f, FLT_MAX);
		float sum = std::fmaxf(0.0f, lambda + old);
		m.SetImpulseSumN(sum);
		lambda = sum - old;

		ApplyImpulse(J, lambda);

		// friction
		glm::vec3  t1, t2;
		glm::vec3 wY(0, 1, 0);
		glm::vec3 wX(1, 0, 0);
		glm::vec3 n = m.GetNormal();

		if (glm::dot(n, wX) > 0.01f)
			t1 = glm::cross(n, wY);
		else
			t1 = glm::cross(n, wX);

		if (glm::length2(t1) == 0) return;

		t1 = glm::normalize(t1);
		m.SetTangent1(t1);
		glm::vec3 r1 = m.GetContact() - m.GetBodyA()->GetPosition();
		glm::vec3 r2 = m.GetContact() - m.GetBodyB()->GetPosition();
		Jacobian Jt1(-t1, -glm::cross(r1, t1), t1, glm::cross(r2, t1));
		em = CalculateEffectiveMass(Jt1);
		lambda = CalculateLagrangian(Jt1, 0, em);

		// clamp the friction
		float cf = 0.79f;
		float g = 9.7f;
		float sumN = m.GetImpulseSumN();
		old = m.GetImpulseSumT1();
		//sum = glm::clamp(lambda + old, -cf*(g)*(em), cf*(g)*(em));
		sum = glm::clamp(lambda + old, -cf*sumN, cf*sumN);
		m.SetImpulseSumT1(sum);
		lambda = sum - old;
		ApplyImpulse(Jt1, lambda);

		t2 = glm::cross(t1, n);
		t2 = glm::normalize(t2);
		m.SetTangent2(t2);
		Jacobian Jt2(-t2, -glm::cross(r1, t2), t2, glm::cross(r2, t2));
		em = CalculateEffectiveMass(Jt2);
		lambda = CalculateLagrangian(Jt2, 0, em);
		old = m.GetImpulseSumT2();
		//sum = glm::clamp(lambda + old, -cf*(g)*(em), cf*(g)*(em));
		sum = glm::clamp(lambda + old, -cf*sumN, cf*sumN);
		m.SetImpulseSumT2(sum);
		lambda = sum - old;
		ApplyImpulse(Jt2, lambda);
	}
}