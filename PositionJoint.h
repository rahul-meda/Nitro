
#pragma once

#include <glm/glm.hpp>
#include "Joint.h"

namespace Nitro
{
	class Body;

	// Position Joint prevents a pair of points from separating
	// The 2 points are constrained to move together, with no restriction on rotation
	// Simulates Ball-and-Socket Joint
	// Removes 3 Degrees of Freedom - 3 Constraints required
	class PositionJoint : public Joint
	{
	public:
		PositionJoint();

		PositionJoint(Body* A, Body* B);

		void Solve();
	};
}