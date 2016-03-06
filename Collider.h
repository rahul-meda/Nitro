
#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Nitro
{
	class Body;
	class SphereCollider;
	class PlaneCollider;
	class BoxCollider;

	// Interface to all the primitive collision geometries
	// Provides methods to : 
	// 1. Test collisions between primirive shapes
	// 2. Detect contacts between colliding bodies
	// 3. Resolve inter-penetration at collision points
	// 4. Apply impulse at collision points
	class Collider
	{
	public:
		// Collision geometry
		enum PrimitiveType
		{
			Sphere,
			Plane,
			Box,
			Triangle,
			Cylinder,
			Cone,
			Capsule
		};

		Collider(Body* body);

		~Collider();

		virtual PrimitiveType GetPrimitiveType() const = 0;

		Body* GetBody() const;


		/////////////////////////////////////////////////
		/////////      Broad Phase Collision 	 ////////
		///////// Check For Potential Collisions ////////
		/////////////////////////////////////////////////
		
		// test if the given point is inside the collider
		virtual bool TestPoint(const glm::vec3& point);

		// test collision with given collider
		virtual bool TestCollision(Collider* other);

		// test collision with given sphere
		virtual bool TestSphere(SphereCollider* sphere) = 0;

		// test collision with given plane
		virtual bool TestPlane(PlaneCollider* plane) = 0;

		// test collision with given box
		virtual bool TestBox(BoxCollider* box) = 0;

		/////////////////////////////////////////////////
		/////////      Broad Phase Collision 	 ////////
		///////// Check For Potential Collisions ////////
		/////////////////////////////////////////////////


		/////////////////////////////////////////////
		/////////  Narrow Phase Collision  //////////
		/////////    Contact Generation	   //////////
		/////////////////////////////////////////////
		
		// detect contacts with given collider
		virtual void DetectContacts(Collider* other);

		// detect contacts with given sphere
		virtual void DetectSphere(SphereCollider* sphere) = 0;

		// detect contacts with given plane
		virtual void DetectPlane(PlaneCollider* plane) = 0;
		
		// detect contacts with given box
		virtual void DetectBox(BoxCollider* box) = 0;
		
		/////////////////////////////////////////////
		/////////  Narrow Phase Collision  //////////
		/////////    Contact Generation	   //////////
		/////////////////////////////////////////////

	protected:
		// The primitive geometry of this collider
		PrimitiveType primitiveType;

		// The body associated with this collider
		Body* body;
	};

	class SphereCollider : public Collider
	{
	private:
		float radius;

	public:
		SphereCollider(Body* b, const float radius);

		PrimitiveType GetPrimitiveType() const;

		float GetRadius() const;

		bool TestPoint(const glm::vec3& point);

		bool TestSphere(SphereCollider* sphere);

		bool TestPlane(PlaneCollider* plane);

		bool TestBox(BoxCollider* box);

		void DetectSphere(SphereCollider* sphere);

		void DetectPlane(PlaneCollider* plane);

		void DetectBox(BoxCollider* box);
	};

	class PlaneCollider : public Collider
	{
	private:
		// normal to the plane
		glm::vec3 normal;

	public:
		PlaneCollider(Body* b, const glm::vec3& normal);

		PrimitiveType GetPrimitiveType() const;

		glm::vec3 GetNormal() const;

		bool TestPoint(const glm::vec3& point);

		bool TestSphere(SphereCollider* sphere);

		bool TestPlane(PlaneCollider* plane);

		bool TestBox(BoxCollider* box);

		void DetectSphere(SphereCollider* sphere);

		void DetectPlane(PlaneCollider* plane);

		void DetectBox(BoxCollider* box);
	};

	struct Segment
	{
		glm::vec3 p; // start point of segment
		glm::vec3 q; // end point of segment
		glm::vec3 dir; // direction of the segment
		float l; // length of the segment

		Segment(glm::vec3 p = glm::vec3(0), glm::vec3 q = glm::vec3(0), glm::vec3 dir = glm::vec3(0), float l = 0.0f)
			: p(p), q(q), dir(dir), l(l) {}
	};

	class BoxCollider : public Collider
	{
	private:
		// half-sizes of the box
		glm::vec3 extents;

		// vertices of body in world-space 
		glm::vec3 vertices[8];

		// edges of body in world-space
		Segment edges[12];

	public:
		BoxCollider(Body* b, const glm::vec3& extents);

		PrimitiveType GetPrimitiveType() const;

		glm::vec3 GetExtents() const;

		// vertices in body-space
		void CalculateVertices();

		// edges in world-space
		void CalculateEdges();

		// test point given in world space
		bool TestPoint(const glm::vec3& point);

		// test point given in body-space
		bool TestLocalPoint(const glm::vec3& point);

		// check if this box overlaps with given box on the given axis
		bool TestOverlap(BoxCollider* box, const glm::vec3& axis);

		// calculates projection of half-extents of this box on the given axis
		float CalculateProjection(const glm::vec3& axis);

		bool TestSphere(SphereCollider* sphere);

		bool TestPlane(PlaneCollider* plane);

		bool TestBox(BoxCollider* box);

		void DetectSphere(SphereCollider* sphere);

		void DetectPlane(PlaneCollider* plane);

		// Detect points of B inside A
		void DetectPointFace(BoxCollider* boxB);

		void DetectBox(BoxCollider* box);
	};
}