
#pragma once

#include <vector>

namespace Nitro
{
	class Object;
	class Joint;
	class Sphere;
	class Ray;
	class Cube;
	class Manifold;

	class Physics
	{
	private:
		Physics();

		~Physics();

		std::vector<Manifold> contacts;

		std::vector<Joint*> joints;

		// Debug draw 
		// for drawing points
		Sphere* contact;

		// for drawing rays
		Ray* normal;

		// for drawing cubes
		Cube* box;

	public:
		bool pauseStep;

		bool advanceStep;

	public:
		static Physics& GetInstance();

		void Init();

		std::vector<Manifold>& GetContactList();

		std::vector<Joint*>& GetJoints();

		Ray* GetRay();

		Cube* GetCubeModel();

		void Step(const std::vector<Object*> objects, float dt);

		void Update(const std::vector<Object*> objects);

		void DebugDraw();

		void DrawJoints();
	};
}