
#pragma once

#define K 0.1f
#define MASS 0.001f

namespace Nitro
{
	class Body;

	class Spring
	{
	private:
		Body* A;

		Body* B;

		// original length of the spring (rest length)
		float L;

	public:
		Spring();

		Spring(Body* A, Body* B);

		Body* GetBodyA();

		Body* GetBodyB();

		void Render();

		void Solve();
	};
}