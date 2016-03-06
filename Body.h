
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Nitro
{
	class Collider;

	class Body
	{
	protected:
		//////////////////////////
		//// State Properties ////
		//////////////////////////
		// inverse mass
		float invMass;

		// inverse inertia tensor in body-space
		glm::mat3 invInertia;

		// inverse inertia tensor in world-space
		glm::mat3 iitW;

		// static bodies are immovable
		bool isStatic;

		// position of C.O.M
		glm::vec3 position;

		// last frame position
		glm::vec3 prevPos;

		// orientation
		glm::quat orientation;

		// half-extents of the body
		glm::vec3 scale;

		// rotation matrix of the body
		glm::mat3 R;
		//////////////////////////
		//// State Properties ////
		//////////////////////////

		//////////////////////////////
		//// Kinematic Properties ////
		//////////////////////////////
		// linear velocity
		glm::vec3 velocity;

		// angular velocity
		glm::vec3 angularVelocity;

		// current frame acceleration
		glm::vec3 cfa;

		// last frame acceleration
		glm::vec3 lfa;

		// acceleration in next frame
		glm::vec3 nfa;

		float energy;

		bool isResting;

		// to remove energy added through numerical errors
		float linearDamping;

		float angularDamping;

		glm::vec3 force;

		glm::vec3 torque;
		//////////////////////////////
		//// Kinematic Properties ////
		//////////////////////////////

		//////////////////////////////
		//// Dynamic Properties //////
		//////////////////////////////
		float staticFriction;

		float dynamicFriction;

		// The coolider attached to this body
		Collider* collider;

		float restitution;
		//////////////////////////////
		//// Dynamic Properties //////
		//////////////////////////////

		// For collision filtering
		// To Do: Do this using bit masks
		int group;

	public:
		Body(const glm::vec3& p, const glm::quat& o, const glm::vec3& s, bool isStatic = false);

		void SetMass(const float d = 100.0f);

		float GetInvMass() const;

		void SetStatic(bool s = true);

		bool IsStatic() const;

		virtual void SetInertia(const glm::mat3& inertia = glm::mat3(1));

		glm::mat3 GetInvInertia() const;

		void SetPosition(const glm::vec3& pos);

		glm::vec3 GetPosition() const;

		void SetOrientation(const glm::quat& orient);

		glm::quat GetOrientation() const;

		void AddOrientation(const float angle, const glm::vec3& axis);

		glm::mat3 GetRotationMatrix() const;

		// for converting point from body-space to world-space
		glm::vec3 LocalToWorld(const glm::vec3& bodyPoint);

		// for converting point from world-space to body-space
		glm::vec3 WorldToLocal(const glm::vec3& wPoint);

		// for converting direction from body to world space
		glm::vec3 LocalToWorldDir(const glm::vec3& dir);

		// for converting direction from world to body space
		glm::vec3 WorldToLocalDir(const glm::vec3& dir);

		void SetScale(const glm::vec3& s);

		glm::vec3 GetScale() const;

		void SetVelocity(const glm::vec3& vel);

		glm::vec3 GetVelocity() const;

		glm::vec3 GetLFA() const;

		void SetAngularVelocity(const glm::vec3& r);

		glm::vec3 GetAngularVelocity() const;

		void SetLinearDamping(const float d);

		float GetLinearDamping() const;

		void SetAngularDamping(const float d);

		float GetAngularDamping() const;

		void SetGroup(const int g);

		int GetGroup() const;

		// add force to C.O.M
		void AddForce(const glm::vec3& f);

		// add force at point in body space
		void AddForce(const glm::vec3& f, const glm::vec3& point);

		void AddTorque(const glm::vec3& T);

		// apply impulse at the point (world-space)
		void ApplyImpulse(const glm::vec3& impulse, const glm::vec3& p);

		void SetCollider(Collider* c);

		Collider* GetCollider() const;

		void IntegrateVelocities(float dt);

		void IntegratePositions(float dt);

		void IntegrateVeloityVerlet(float dt);
	};
}