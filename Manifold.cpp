
#include "Manifold.h"

namespace Nitro
{
	Manifold::Manifold()
	{}

	Manifold::Manifold(Body* A, Body* B)
		: A(A), 
		  B(B), 
		  impulseSumN(0),
		  impulseSumT1(0),
		  impulseSumT2(0)
	{}

	void Manifold::SetBodyA(Body* b)
	{
		A = b;
	}

	Body* Manifold::GetBodyA() const
	{
		return A;
	}

	void Manifold::SetBodyB(Body* b)
	{
		B = b;
	}

	Body* Manifold::GetBodyB() const
	{
		return B;
	}

	void Manifold::SetContact(const glm::vec3& c)
	{
		contact = c;
	}

	glm::vec3 Manifold::GetContact() const
	{
		return contact;
	}

	void Manifold::SetNormal(const glm::vec3& n)
	{
		normal = n;

		if (normal != normal)
		{
			int x = 1;
		}
	}

	glm::vec3 Manifold::GetNormal() const
	{
		return normal;
	}

	void Manifold::SetPenetration(const float p)
	{
		penetration = p;
	}

	float Manifold::GetPenetration() const
	{
		return penetration;
	}

	void Manifold::SetImpulseSumN(const float nis)
	{
		impulseSumN = nis;
	}

	float Manifold::GetImpulseSumN() const
	{
		return impulseSumN;
	}

	void Manifold::SetImpulseSumT1(const float tis)
	{
		impulseSumT1 = tis;
	}

	float Manifold::GetImpulseSumT1() const
	{
		return impulseSumT1;
	}

	void Manifold::SetImpulseSumT2(const float tis)
	{
		impulseSumT2 = tis;
	}

	float Manifold::GetImpulseSumT2() const
	{
		return impulseSumT2;
	}

	void Manifold::SetTangent1(const glm::vec3& t1)
	{
		tangent1 = t1;
	}

	glm::vec3 Manifold::GetTangent1() const
	{
		return tangent1;
	}

	void Manifold::SetTangent2(const glm::vec3& t2)
	{
		tangent2 = t2;
	}

	glm::vec3 Manifold::GetTangent2() const
	{
		return tangent2;
	}

	void Manifold::SetContactData(const glm::vec3& c, const glm::vec3& n, const float p)
	{
		contact = c;
		normal = n;
		penetration = p;

		// calculate the basis matrix for this contact
		glm::vec3 cX, cY, cZ;
		glm::vec3 wY(0,1,0);
		glm::vec3 wX(1,0,0);
		glm::mat3 contactBasis;
		cX = normal;
		if (glm::dot(normal, wY) > 0.01f)
		{
			cZ = glm::cross(cX, wY);
		}
		else
		{
			cZ = glm::cross(cX, wX);
		}
		cY = glm::cross(cZ, cX);

		contactBasis[0] = cX;
		contactBasis[1] = cY;
		contactBasis[2] = cZ;
	}

	glm::vec3 Manifold::CalculateRelativeVelocity() const
	{
		glm::vec3 vA = A->GetVelocity();
		glm::vec3 rA = contact - A->GetPosition();
		vA += glm::cross(A->GetAngularVelocity(), rA);

		glm::vec3 vB = B->GetVelocity();
		glm::vec3 rB = contact - B->GetPosition();
		vB += glm::cross(B->GetAngularVelocity(), rB);

		return (vB - vA);
	}

	// Ref : Digipen Physics Club Slides "Collision Resolution Impulses" by Joshua Davis 
	void Manifold::ResolveCollision()
	{
		glm::vec3 vRel = CalculateRelativeVelocity();
		float vApproach = glm::dot(vRel, normal);

		if (vApproach > 0)
			return;

		// linear inverse mass sum (linear Inertia)
		float invMassSum = A->GetInvMass() + B->GetInvMass();

		if (invMassSum == 0)
			return;

		// angular inverse mass sum (angular inertia)
		glm::vec3 rA = contact - A->GetPosition();
		glm::vec3 rAxN = glm::cross(rA, normal);
		glm::vec3 rB = contact - B->GetPosition();
		glm::vec3 rBxN = glm::cross(rB, normal);
		invMassSum += glm::dot(rAxN, A->GetInvInertia()*rAxN);
		invMassSum += glm::dot(rBxN, B->GetInvInertia()*rBxN);

		float e = 0.1f;

		// resting contact
		static float dt = 1.0f / 100.0f;
		// approach velocity due to acceleration
		/*float bias = glm::dot(B->GetLFA()-A->GetLFA(), normal)*(dt);

		if (std::abs(vApproach) < 0.1f)
			e = 0;*/

		float jn = -(1 + e) * (vApproach);
		jn /= invMassSum;
		glm::vec3 jNormal = jn*normal;

		//A->ApplyImpulse(-jNormal, contact);
		//B->ApplyImpulse(jNormal, contact);

		// friction
		glm::vec3  vRelTangent = vRel - (glm::dot(vRel, normal))*normal + glm::vec3(0.0001f);
		glm::vec3 tangent = glm::normalize(vRelTangent);
		tangent *= -1.0f; // To Do: Check later
		float vRelT = glm::dot(vRel, tangent);
		invMassSum = A->GetInvMass() + B->GetInvMass();
		glm::vec3 rAxT = glm::cross(rA, tangent);
		glm::vec3 rBxT = glm::cross(rB, tangent);
		invMassSum += glm::dot(rAxT, A->GetInvInertia()*rAxT);
		invMassSum += glm::dot(rBxT, B->GetInvInertia()*rBxT);
		float jt = -vRelT;
		jt /= invMassSum;

		glm::vec3 jTangent;
		if (glm::abs(jt) < glm::abs(jn)*0.97f)
		{
			jTangent = jt*tangent;
		}
		else
		{
			jTangent = 0.95f*jn*tangent;
		}
		
		A->ApplyImpulse(-jTangent, contact);
		B->ApplyImpulse(jTangent, contact);
	}

	void Manifold::ResolvePenetration()
	{
		float invMassSum = (A->GetInvMass() + B->GetInvMass());
		if (invMassSum == 0)
			return;
		float pen = glm::max(0.0f, penetration-0.03f) / invMassSum;
		//float pen = penetration / (A->GetInvMass() + B->GetInvMass());
		glm::vec3 proj = pen*normal;
		proj *= 0.33f;
		A->SetPosition(A->GetPosition() - proj*A->GetInvMass());
		B->SetPosition(B->GetPosition() + proj*B->GetInvMass());
	}
}