
#pragma once

#include "Constraint.h"

namespace Nitro
{
	class Body;

	class Joint : public Constraint
	{
	protected:
		// The respective joints in local and world space
		glm::vec3 p1;

		glm::vec3 p2;

		glm::vec3 p1W;

		glm::vec3 p2W;

		// original length of the joint
		float L;

		// current length of the joint;
		float currL;
	public:
		Joint();

		Joint(Body* A, Body* B);

		virtual Jacobian CalculateJacobian();

		// set the joint on body - store local and global points
		void SetJoint1(const glm::vec3& p1);

		// returns the world point on body
		glm::vec3 GetJoint1() const;

		void SetJoint2(const glm::vec3& p2);

		glm::vec3 GetJoint2() const;

		virtual void Solve();
	};
}