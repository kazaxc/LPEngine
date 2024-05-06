#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "Defines.h"
#include "../rendering/Model.h"

namespace LPEngine
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		// Matrix = T * Ry * Rx * Rz * S
		// Tait-Bryan angles (YXZ)
		// https://en.wikipedia.org/wiki/Euler_angles
		glm::mat4 Mat4()
		{
			const float c3 = glm::cos(rotation.z);
			const float s3 = glm::sin(rotation.z);
			const float c2 = glm::cos(rotation.x);
			const float s2 = glm::sin(rotation.x);
			const float c1 = glm::cos(rotation.y);
			const float s1 = glm::sin(rotation.y);
			return glm::mat4
			{
				{
					scale.x * (c1 * c3 + s1 * s2 * s3),
					scale.x * (c2 * s3),
					scale.x * (c1 * s2 * s3 - c3 * s1),
					0.0f,
				},
				{
					scale.y * (c3 * s1 * s2 - c1 * s3),
					scale.y * (c2 * c3),
					scale.y * (c1 * c3 * s2 + s1 * s3),
					0.0f,
				},
				{
					scale.z * (c2 * s1),
					scale.z * (-s2),
					scale.z * (c1 * c2),
					0.0f,
				},
				{translation.x, translation.y, translation.z, 1.0f}
			};
		}
	};

	class LPENGINE_API GameObject
	{
	public:
		using id_t = unsigned int;

		static GameObject CreateGameObject()
		{
			static id_t nextId = 0;
			return GameObject(nextId++);
		}

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		const id_t GetId() const { return m_id; }

		std::shared_ptr<Model> model;
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		GameObject(id_t id) : m_id(id) {}
		id_t m_id;
	};
}