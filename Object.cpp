
#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Nitro
{
	Object::Object()
	{

	}

	Object::Object(Model* model, Body* body)
		: model(model), body(body)
	{

	}

	Object::~Object()
	{
		delete model;

		delete body;
	}

	Model* Object::GetModel() const
	{
		return model;
	}

	Body* Object::GetBody() const
	{
		return body;
	}

	void Object::Update(float dt)
	{
		glm::mat4 T = glm::translate(body->GetPosition());
		glm::mat4 R = glm::toMat4(glm::normalize(body->GetOrientation()));
		glm::mat4 S = glm::scale(body->GetScale());

		model->Render();
	}
}