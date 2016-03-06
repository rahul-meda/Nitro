
#pragma once

#include "Body.h"
#include <glm/glm.hpp>

namespace Nitro
{
	class Manifold
	{
	protected:
		Body* A;

		Body* B;

		glm::vec3 contact;

		glm::vec3 normal;

		float penetration;
		
		// for clamping the lagrangian
		float impulseSumN;

		float impulseSumT1;

		float impulseSumT2;

		// friction directions
		glm::vec3 tangent1;
		glm::vec3 tangent2;

	public:
		Manifold();

		Manifold(Body* A, Body* B);

		void SetBodyA(Body* b);

		Body* GetBodyA() const;
	
		void SetBodyB(Body* b);

		Body* GetBodyB() const;

		void SetContact(const glm::vec3& c);

		glm::vec3 GetContact() const;

		void SetNormal(const glm::vec3& n);

		glm::vec3 GetNormal() const;

		void SetPenetration(const float p);

		float GetPenetration() const;

		void SetImpulseSumN(const float nis);

		float GetImpulseSumN() const;

		void SetImpulseSumT1(const float nis);

		float GetImpulseSumT1() const;

		void SetImpulseSumT2(const float nis);

		float GetImpulseSumT2() const;

		void SetTangent1(const glm::vec3& t1);

		glm::vec3 GetTangent1() const;

		void SetTangent2(const glm::vec3& t2);

		glm::vec3 GetTangent2() const;

		void SetContactData(const glm::vec3& c, const glm::vec3& n, const float p);

		////////////////////////////////////////////
		////////    Collision Resolution ///////////
		////////       Impulse Based     ///////////
		////////////////////////////////////////////

		// calculate relative velocity at contact w.r.t A
		glm::vec3 CalculateRelativeVelocity() const;

		// resolve collision for this contact
		// by applying impulse at contact point
		void ResolveCollision();

		void ResolvePenetration();
	};
}