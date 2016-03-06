
#include "Joint.h"
#include "Body.h"

namespace Nitro
{
	Joint::Joint()
	{}

	Joint::Joint(Body* A, Body* B)
		: Constraint(A, B)
	{}

	Jacobian Joint::CalculateJacobian()
	{
		Jacobian J;
		return J;
	}

	void Joint::SetJoint1(const glm::vec3& p)
	{
		p1 = p;
		p1W = A->LocalToWorld(p1);
	}

	glm::vec3 Joint::GetJoint1() const
	{
		return p1W;
	}

	void Joint::SetJoint2(const glm::vec3& p)
	{
		p2 = p;
		if (B->IsStatic())
			p2W = p2;
		else
			p2W = B->LocalToWorld(p2);

		L = glm::distance(p2W, p1W);
	}

	glm::vec3 Joint::GetJoint2() const
	{
		return p2W;
	}

	void Joint::Solve()
	{

	}
}