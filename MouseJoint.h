
#pragma once

#include <glm/glm.hpp>
#include "Joint.h"

namespace Nitro
{
	class Body;

	// For picking objects using mouse
	// Drives a point on the body to the mouse position
	// Removes 3 degrees of freedom - 3 constraints required
	// The 2 points for this joint are the anchor point on the body
	// and the mouse point
	// The 2 points are constrained to move together, with no restriction on rotation
	// Similar to revolute joint, but mouse is considered static (zero velocity, zero mass)
	class MouseJoint : public Joint
	{
	public:
		MouseJoint();

		MouseJoint(Body* A, Body* B);

		void Solve();
	};
}