
#define CAM_SPEED 200.0f
#define DAMPING 0.97f
#define SPEED_EPSILON 0.79f

#include "Camera.h"
#include <glm/gtx/euler_angles.hpp>

namespace Nitro
{
	Camera::Camera()
	{
		zNear = 0.1f;
		zFar = 10000.0f;
	}

	Camera::~Camera()
	{
	}

	Camera& Camera::GetInstance()
	{
		static Camera instance;
		return instance;
	}

	void Camera::SetProjection(const float fov, const float aspectRatio, const float n, const float f)
	{
		this->fov = fov;
		this->aspectRatio = aspectRatio;
		zNear = n;
		zFar = f;

		P = glm::perspective(fov, aspectRatio, n, f);
	}

	const glm::mat4 Camera::GetViewMatrix() const
	{
		return V;
	}

	const glm::mat4 Camera::GetProjectionMatrix() const
	{
		return P;
	}

	const glm::mat4 Camera::GetVPMatrix() const
	{
		return P*V;
	}

	void Camera::SetPosition(const glm::vec3& pos)
	{
		position = pos;
	}

	const glm::vec3 Camera::GetPosition() const
	{
		return position;
	}

	void Camera::Move(const glm::vec3& dir)
	{
		const static float dt = 1.0f / 100.0f;

		position += 2*CAM_SPEED*dt*dir;
		Update();
	}

	const glm::vec3 Camera::GetVelocity() const
	{
		return velocity;
	}

	const glm::vec3 Camera::GetCamX() const
	{
		return right;
	}

	const glm::vec3 Camera::GetCamY() const
	{
		return up;
	}

	const glm::vec3 Camera::GetCamZ() const
	{
		return look;
	}

	void Camera::SetFOV(const float fov)
	{
		this->fov = fov;
		P = glm::perspective(fov, aspectRatio, zNear, zFar);
	}

	const float Camera::GetFOV() const
	{
		return fov;
	}

	const float Camera::GetAspectRatio() const
	{
		return aspectRatio;
	}

	void Camera::Rotate(const float yaw, const float pitch, const float roll)
	{
		float y = glm::radians(yaw);
		float p = glm::radians(pitch);
		float r = glm::radians(roll);

		R = glm::yawPitchRoll(y, p, r);

		Update();
	}

	void Camera::Update()
	{
		//static float dt = 1.0f / 100.0f;
		//if (glm::dot(velocity, velocity) > SPEED_EPSILON*SPEED_EPSILON)
		//{
		//	//velocity *= 0.9;
		//	position += velocity*dt;
		//}

		// camera z-axis
		look = glm::vec3(R*glm::vec4(0,0,1,0));

		// camera y-axis
		up = glm::vec3(R*glm::vec4(0,1,0,0));

		// camera x-axis
		//right = glm::vec3(R*glm::vec4(1,0,0,0)); // verify this later
		right = glm::cross(look, up);

		// target direction
		glm::vec3 target = position + look;
		//target = glm::normalize(target);

		V = glm::lookAt(position, target, up);
	}
}