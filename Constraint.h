
#pragma once

#include <glm/glm.hpp>

namespace Nitro
{
	// Constrains are easy to solve in constraint space
	// The Jacobian is used for transformation
	// from world space(generally cartesian) to constraint space
	// The transpose of the Jacobian is used for the inverse transformation
	struct Jacobian
	{
		Jacobian() {};

		Jacobian(const glm::vec3 L1, const glm::vec3 A1, const glm::vec3 L2, const glm::vec3 A2)
			: L1(L1), A1(A1), L2(L2), A2(A2)
		{}

		// The linear and angular Jacobian components of the pair of bodies
		// invloved in this constraint
		// They are the coefficients of the respective linear and angular
		// velocity components in the velocity constraint equation
		// The velocity constraint equation is usually obtained by taking
		// the derivative of the position constraint equation
		glm::vec3 L1;
		glm::vec3 A1;
		glm::vec3 L2;
		glm::vec3 A2;
	};

	class Body;

	class Constraint
	{
	public:
		Constraint();

		Constraint(Body* A, Body* B);

		// Each type of constraint has its own implementation
		virtual Jacobian CalculateJacobian() = 0;

		// Bias introduces energy ito the system
		// Zero for constraints that don't do any work
		virtual float CalculateBias();

		// Calculate Effective Mass of the Constraint
		float CalculateEffectiveMass(const Jacobian& J);

		// Impulse in constraint space
		float CalculateLagrangian(const Jacobian& J, const float b, const float em);

		// Convert impulse into world space and 
		// apply it to both the bodies
		void ApplyImpulse(const Jacobian& J, const float lambda);

		// The constraint solver
		virtual void Solve() = 0;

	protected:
		// The body pair involved in this constraint
		Body* A;

		Body* B;
	};
}