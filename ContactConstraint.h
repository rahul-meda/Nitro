
#pragma once

#include "Constraint.h"
#include "Manifold.h"

namespace Nitro
{
	// Contact constraint stops the bodies from inter-penetrating
	// Bias is used to resolve penetration and bounce
	// Restricts one Degree of Freedom - 1 Constraint required
	// prevents relative motion b/w the bodies along the contact normal
	// Friction introduces more constraints preventing relative motion
	// b/w the bodies along contact tangents
	class ContactConstraint : public Constraint
	{
	public:
		ContactConstraint(const Manifold& m);

		Jacobian CalculateJacobian();

		float CalculateBias();

		// warm start the contact using previous impulse sum as an initial guess
		void WarmStart();

		void Solve();

	private:
		Manifold m;
	};
}