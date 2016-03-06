
#include "MouseJoint.h"
#include "Body.h"

namespace Nitro
{
	MouseJoint::MouseJoint()
	{}

	MouseJoint::MouseJoint(Body* A, Body* B)
		: Joint(A, B)
	{}

	void MouseJoint::Solve()
	{
		// Calculate Jacobian
		p1W = A->LocalToWorld(p1);
		p2W = p2;

		glm::vec3 r1 = p1W - A->GetPosition();

		glm::vec3 x = glm::vec3(1, 0, 0);
		Jacobian J1(-x, -glm::cross(r1, x), glm::vec3(0), glm::vec3(0));
		glm::vec3 y = glm::vec3(0, 1, 0);
		Jacobian J2(-y, -glm::cross(r1, y), glm::vec3(0), glm::vec3(0));
		glm::vec3 z = glm::vec3(0, 0, 1);
		Jacobian J3(-z, -glm::cross(r1, z), glm::vec3(0), glm::vec3(0));

		// Calculate Bias
		float b1 = (0.179f)*(60.0f)*(p2W.x - p1W.x);
		float b2 = (0.179f)*(60.0f)*(p2W.y - p1W.y);
		float b3 = (0.179f)*(60.0f)*(p2W.z - p1W.z);

		// Calculate Effective Mass
		float em1 = CalculateEffectiveMass(J1);
		float em2 = CalculateEffectiveMass(J2);
		float em3 = CalculateEffectiveMass(J3);

		// Calculate Impulse (Constraint space)
		float lambda1 = CalculateLagrangian(J1, b1, em1);
		float lambda2 = CalculateLagrangian(J2, b2, em2);
		float lambda3 = CalculateLagrangian(J3, b3, em3);

		// Apply Impulse
		ApplyImpulse(J1, lambda1);
		ApplyImpulse(J2, lambda2);
		ApplyImpulse(J3, lambda3);
	}
}