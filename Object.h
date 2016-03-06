
#pragma once

#include "Body.h"
#include "Model.h"
#include "Camera.h"

namespace Nitro
{
	class Object
	{
	private:
		Model* model;

		Body* body;

	public:
		Object();

		Object(Model* model, Body* body);

		~Object();

		Model* GetModel() const;

		Body* GetBody() const;

		void Update(float dt);
	};
}