
#include "DistanceJoint.h"
#include "Body.h"

namespace Nitro
{
	DistanceJoint::DistanceJoint()
	{}

	DistanceJoint::DistanceJoint(Body* A, Body* B)
		: Joint(A, B)
	{}

	float DistanceJoint::CalculateBias()
	{
		currL = glm::distance(p2W, p1W);

		float d = currL - L;
		float b = (0.179)*(60.0f)*(d);

		return b;
	}

	void DistanceJoint::Solve()
	{
		// calculate Jacobian
		p1W = A->LocalToWorld(p1);
		p2W = B->LocalToWorld(p2);

		glm::vec3 d = p2W - p1W;
		glm::vec3 r1 = p1W - A->GetPosition();
		glm::vec3 r2 = p2W - B->GetPosition();

		Jacobian J(-d, -glm::cross(r1, d), d, glm::cross(r2, d));

		// bias
		float b = CalculateBias();

		float em = CalculateEffectiveMass(J);

		float lambda = CalculateLagrangian(J, b, em);

		ApplyImpulse(J, lambda);
	}
}