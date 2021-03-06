
#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Nitro
{
	class Camera
	{
	public:
		static Camera& GetInstance();

		void SetProjection(const float fov, const float aspectRatio, const float zNear = 0.1f, const float zFar = 10000.0f);

		const glm::mat4 GetViewMatrix() const;

		const glm::mat4 GetProjectionMatrix() const;

		const glm::mat4 GetVPMatrix() const;

		void SetFOV(const float fov);

		const float GetFOV() const;

		const float GetAspectRatio() const;

		void SetPosition(const glm::vec3& pos);

		const glm::vec3 GetPosition() const;

		void Move(const glm::vec3& dir);

		const glm::vec3 GetVelocity() const;

		// getters for local camera axes
		const glm::vec3 GetCamX() const;

		const glm::vec3 GetCamY() const;

		const glm::vec3 GetCamZ() const;

		void Rotate(const float yaw, const float pitch, const float roll);

		void Update();

	private:
		Camera();

		~Camera();

		// camera frustrum
		float fov, aspectRatio, zNear, zFar;

		// world up direction
		static glm::vec3 UP;

		// camera axes
		glm::vec3 look, up, right;

		// camera position
		glm::vec3 position;

		// camera velocity
		glm::vec3 velocity;

		// camrea rotation matrix
		glm::mat4 R;

		// view matrix
		glm::mat4 V;

		// projection matrix
		glm::mat4 P;
	};
}