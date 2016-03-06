
#pragma once

#include "Joint.h"
#include <glm/glm.hpp>

namespace Nitro
{
	class Body;

	class DistanceJoint : public Joint
	{
	public:
		DistanceJoint();

		DistanceJoint(Body* A, Body* B);

		float CalculateBias();

		void Solve();
	};
}