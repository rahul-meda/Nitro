
#include "Body.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Nitro
{
	Body::Body(const glm::vec3& p, const glm::quat& o, const glm::vec3& s, bool isStatic)
		: position(p), 
		prevPos(p),
		orientation(o), 
		scale(s),
		invMass(0), 
		isStatic(isStatic),
		invInertia(1), 
		velocity(0),
		nfa(0),
		angularVelocity(0),
		energy(0),
		isResting(false),
		linearDamping(1), 
		angularDamping(1),
		force(0), 
		torque(0),
		group(0)
	{
		orientation = glm::normalize(orientation);
		R = glm::toMat3(orientation);
		SetMass();
		SetInertia();
	}

	void Body::SetMass(const float d)
	{
		if (isStatic)
		{
			invMass = 0;
			invInertia = glm::mat3(0);
			iitW = glm::mat3(0);
			return;
		}
		float m = (2 * scale.x)*(2 * scale.y)*(2 * scale.z)*d;
		invMass = (float) 1.0f / m;
	}

	float Body::GetInvMass() const
	{
		return invMass;
	}

	void Body::SetStatic(bool s)
	{
		isStatic = s;
		if (isStatic)
		{
			invMass = 0;
			invInertia = glm::mat3(0);
			iitW = glm::mat3(0);
		}
	}

	bool Body::IsStatic() const
	{
		return isStatic;
	}

	void Body::SetInertia(const glm::mat3& i)
	{
		if (isStatic)
		{
			invInertia = glm::mat3(0);
			iitW = glm::mat3(0);
			return;
		}

		float x2 = 2*scale.x*2*scale.x;
		float y2 = 2*scale.y*2*scale.y;
		float z2 = 2*scale.z*2*scale.z;

		float m = 1.0f / invMass;
		m /= 12.0f;

		float ixx = m*(y2+z2);
		float iyy = m*(z2+x2);
		float izz = m*(x2+y2);

		glm::mat3 inertia(1);
		inertia[0][0] = ixx;
		inertia[1][1] = iyy;
		inertia[2][2] = izz;

		invInertia = glm::mat3(1);
		invInertia[0][0] = 1.0f / inertia[0][0];
		invInertia[1][1] = 1.0f / inertia[1][1];
		invInertia[2][2] = 1.0f / inertia[2][2];
	}

	glm::mat3 Body::GetInvInertia() const
	{
		return iitW;
	}

	void Body::SetPosition(const glm::vec3& pos)
	{
		position = pos;

		if (position != position)
		{
			int x = 1;
		}
	}

	glm::vec3 Body::GetPosition() const
	{
		return position;
	}

	void Body::SetOrientation(const glm::quat& orient)
	{
		orientation = glm::normalize(orient);

		if (orientation != orientation)
		{
			int x = 1;
		}
	}

	glm::quat Body::GetOrientation() const
	{
		return orientation;
	}

	glm::mat3 Body::GetRotationMatrix() const
	{
		return R;
	}

	glm::vec3 Body::LocalToWorld(const glm::vec3& lPoint)
	{
		return ((R)*(lPoint) + position);
	}

	glm::vec3 Body::WorldToLocal(const glm::vec3& wPoint)
	{
		return ((glm::transpose(R))*(wPoint - position));
		//return (glm::transpose(R)*(wPoint) - position);
	}

	glm::vec3 Body::LocalToWorldDir(const glm::vec3& lDir)
	{
		return (R * lDir);
	}

	glm::vec3 Body::WorldToLocalDir(const glm::vec3& wDir)
	{
		return (glm::transpose(R) * wDir);
	}

	void Body::SetScale(const glm::vec3& s)
	{
		scale = s;
	}

	glm::vec3 Body::GetScale() const
	{
		return scale;
	}

	void Body::SetVelocity(const glm::vec3& vel)
	{
		isResting = false;
		velocity = vel;
	}

	glm::vec3 Body::GetVelocity() const
	{
		return velocity;
	}

	glm::vec3 Body::GetLFA() const
	{
		return lfa;
	}

	void Body::SetAngularVelocity(const glm::vec3& r)
	{
		isResting = false;
		angularVelocity = r;
	}

	glm::vec3 Body::GetAngularVelocity() const
	{
		return angularVelocity;
	}

	void Body::SetLinearDamping(const float d)
	{
		linearDamping = d;
	}

	void Body::SetAngularDamping(const float d)
	{
		angularDamping = d;
	}

	void Body::SetGroup(const int g)
	{
		group = g;
	}

	int Body::GetGroup() const
	{
		return group;
	}

	void Body::AddForce(const glm::vec3& f)
	{
		isResting = false;
		force += f;
	}

	void Body::AddForce(const glm::vec3& f, const glm::vec3& p)
	{
		isResting = false;
		force += f;

		glm::vec3 r = LocalToWorld(p) - position;
		glm::vec3 T = glm::cross(r, f);

		torque += T;
	}

	void Body::AddTorque(const glm::vec3& T)
	{
		isResting = false;
		torque += T;
	}

	void Body::ApplyImpulse(const glm::vec3& impulse, const glm::vec3& p)
	{
		isResting = false;
		velocity += invMass * impulse;
		glm::vec3 T = glm::cross(p-position, impulse);
		angularVelocity += iitW * T;
	}

	void Body::SetCollider(Collider* c)
	{
		collider = c;
	}

	Collider* Body::GetCollider() const
	{
		return collider;
	}

	void Body::AddOrientation(const float angle, const glm::vec3& axis)
	{
		glm::quat dq = glm::quat(0, axis*angle);
		orientation += dq*orientation*0.5f;
		orientation = glm::normalize(orientation);
	}

	void Body::IntegrateVelocities(float dt)
	{
		if (isStatic) return;

		//if (isResting) return;

		glm::vec3 linAcc = force * invMass + glm::vec3(0,-9.7f,0)*2.0f;
		lfa = linAcc;
		//linAcc = glm::vec3(0);
		// rotation matrix
		orientation = glm::normalize(orientation);
		R = glm::toMat3(orientation);

		// inverse inertia in world space *(To Do: Check the order)*
		iitW = glm::transpose(R) * invInertia * (R);
		//iitW = R * invInertia * glm::transpose(R);

		glm::vec3 angAcc = iitW * torque;

		velocity += linAcc * dt;
		angularVelocity += angAcc * dt;

		velocity *= linearDamping;
		angularVelocity *= angularDamping;

		/*float currEnergy = glm::length2(velocity) + glm::length2(angularVelocity);
		float bias = 0.79f;
		energy = bias*energy + (1 - bias)*currEnergy;
		if (energy < 0.003f)
		{
			isResting = true;
			velocity = glm::vec3(0);
			angularVelocity = glm::vec3(0);
		}
		else if(energy > 1.0f)
		{
			energy = 1.0f;
		}*/

		/*force = glm::vec3(0);
		torque = glm::vec3(0);*/
	}

	void Body::IntegratePositions(float dt)
	{
		if (isStatic) return;

		// Euler
		position += velocity * dt;

		// Position Verlet
		/*static bool verletStep1 = true;

		if (verletStep1)
		{
			glm::vec3 a = force * invMass + glm::vec3(0, -9.7f, 0);
			position += velocity*dt + 0.5f*a*dt*dt;
			verletStep1 = false;
			return;
		}

		glm::vec3 temp = 2.0f*position - prevPos;
		prevPos = position;
		glm::vec3 a = force * invMass + glm::vec3(0, -9.7f, 0);
		position = temp + a*dt*dt;*/

		glm::quat dq = glm::quat(0, angularVelocity) * dt;
		orientation += dq * orientation * 0.5f;

		force = glm::vec3(0);
		torque = glm::vec3(0);
	}

	void Body::IntegrateVeloityVerlet(float dt)
	{
		nfa = force*invMass + glm::vec3(0, -9.7f, 0);
		velocity += 0.5f*(cfa + nfa)*dt;

		force = glm::vec3(0);
		torque = glm::vec3(0);
	}
}