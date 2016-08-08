
#include "Wave2D.h"
#include "math.h"
#include "Camera.h"

#define PI 3.14285

namespace Nitro
{
	Wave2D::Wave2D()
		: a(2),
		  b(2),
		  dx(0.025f),
		  dz(0.025f),
		  M(7),
		  N(7),
		  amplitude(0.2f),
		  frequency(0.3f),
		  funcType(1),
		  t(0)
	{
		int p = a / dx;
		int q = b / dz;

		vertices.reserve(1600);
		for (int i = 0; i < p; i++)
		{
			for (int j = 0; j < q; j++)
			{
				float xi = (float)i*dx;
				float zi = (float)j*dz;
				surface[i][j] = glm::vec3(xi,0,zi);
				vertices.push_back(surface[i][j]);
			}
		}

		CalculateFourierCoeffs();
	}

	void Wave2D::OnInit(GLFWwindow* window)
	{
		Scene::OnInit(window);

		mesh = new ClothMesh(glm::vec3(0.25,0.5,1));
	}

	void Wave2D::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mods)
	{
		Scene::OnKeyInput(window, key, code, action, mods);
		
		if (keys[GLFW_KEY_Y])
		{
			if (amplitude < 1.0f)
				amplitude += 0.1f;
		}
		if (keys[GLFW_KEY_T])
		{
			if (amplitude > 0.2f)
				amplitude -= 0.1f;
		}
		if (keys[GLFW_KEY_C])
		{
			if (frequency < 1.0f)
				frequency += 0.1f;
		}
		if (keys[GLFW_KEY_X])
		{
			if (frequency > 0.2f)
			{
				frequency -= 0.1f;
			}
		}
		if (keys[GLFW_KEY_M])
		{
			if (M < 10)
			{
				M += 1;
				N += 1;
			}
		}
		if (keys[GLFW_KEY_N])
		{
			if (M > 3)
			{
				M -= 1;
				N -= 1;
			}
		}
		if (keys[GLFW_KEY_F])
		{
			if (funcType < 5)
			{
				funcType += 1;
				CalculateFourierCoeffs();
			}
			else
			{
				funcType = 1;
				CalculateFourierCoeffs();
			}
		}
		if (keys[GLFW_KEY_R])
		{
			amplitude = 0.3f;
			frequency = 0.2f;
			M = 7;
			N = 7;
			funcType = 1;
			CalculateFourierCoeffs();
		}
	}

	void Wave2D::CalculateFourierCoeffs()
	{
		for (int m = 1; m < M; m++)
		{
			for (int n = 1; n < N; n++)
			{
				a_mn[m][n] = RiemannSum(m, n);
			}
		}
	}

	float Wave2D::Height_t0(float x, float z)
	{
		switch (funcType)
		{
		case 1:
			return (x*z*(1 - x)*(1 - z));
		case 2:
			return (x+z+x*z);
		case 3:
			return (x+z);
		case 4:
			return (x*z);
		case 5:
			return (5.0f*sinf(x+z));
		}
	}

	float Wave2D::RiemannSum(int m, int n)
	{
		float sum = 0.0f;
		
		int p = a / dx;
		int q = b / dz;

		for (int i = 0; i < p; i++)
		{
			for (int j = 0; j < q; j++)
			{
				float xi = surface[i][j].x;
				float zi = surface[i][j].z;
				float fxizi = Height_t0(xi, zi);
				sum += (fxizi)*(sin((float)m*PI*xi / (float)a))*(sin((float)n*PI*zi / (float)b))*dx*dz;
			}
		}
		sum *= (float)(4.0f/(a*b));

		return sum;
	}

	float Wave2D::Height_t(float x, float z, float t)
	{
		float y = 0.0f;

		for (int m = 1; m < M; m++)
		{
			for (int n = 1; n < N; n++)
			{
				float m2 = m*m;
				float n2 = n*n;
				float a2 = a*a;
				float b2 = b*b;
				float k_mn = PI*(sqrtf((m2/a2)+(n2/b2)));
				y += (a_mn[m][n])*(cos(k_mn*(frequency)*(t)))*(sin((float)m*PI*x/(float)a))*(sin((float)n*PI*z)/(float)b);
			}
		}

		y *= amplitude;

		return y;
	}

	void Wave2D::Update()
	{
		static const float dt = 1.0f / 60.0f;
		t += dt;

		int p = a / dx;
		int q = b / dz;

		for (int i = 0; i < p; i++)
		{
			for (int j = 0; j < q; j++)
			{
				surface[i][j].y = Height_t(surface[i][j].x, surface[i][j].z, t);
				vertices[i*(p)+j] = surface[i][j];
			}
		}

		glm::mat4 V = Camera::GetInstance().GetViewMatrix();
		glm::mat4 P = Camera::GetInstance().GetProjectionMatrix();
		glm::mat4 VP = P*V;

		Scene::Update();

		for (int i = 0; i < vertices.size(); i++)
		{
			vertices[i] *= 100.0f;
		}

		mesh->SetPoints(vertices);
		mesh->SetMVP(VP);
		mesh->Render();
	}
}