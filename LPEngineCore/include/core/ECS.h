#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <memory>
#include <vector>
#include <array>
#include <bitset>
#include <string>

#include "Defines.h"
#include "../rendering/Model.h"

namespace LPEngine
{
	class LPENGINE_API IComponent
	{
	public:
		virtual ~IComponent() {};
	};

	struct TransformComponent : public IComponent
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

	struct ColourComponent : public IComponent
	{
		glm::vec3 colour;
	};

	struct ModelComponent : public IComponent
	{
		std::shared_ptr<Model> model;
	};

	using ComponentType = std::size_t;

	inline ComponentType GetUniqueComponentType()
	{
		static ComponentType lastType = 0;
		return lastType++;
	}

	template<typename T>
	inline ComponentType GetComponentType()
	{
		static ComponentType type = GetUniqueComponentType();
		return type;
	}

	class LPENGINE_API Entity
	{
	public:
		template<typename T>
		void AddComponent(T* component)
		{
			ComponentType type = GetComponentType<T>();
			m_Components[type] = component;
		}

		template<typename T>
		T* GetComponent()
		{
			ComponentType type = GetComponentType<T>();
			auto it = m_Components.find(type);
			if (it == m_Components.end())
			{
				return nullptr;
			}
			return reinterpret_cast<T*>(it->second);
		}

		template<typename T>
		void RemoveComponent()
		{
			ComponentType type = GetComponentType<T>();
			m_Components.erase(type);
		}

	private:
		std::unordered_map<ComponentType, IComponent*> m_Components;
	};

	class LPENGINE_API System
	{
	public:
		virtual void Update(Entity& entity) = 0;
	};

	class LPENGINE_API ECSManager
	{
	public:
		void AddEntity(Entity entity);
		void AddSystem(System* system);
		void Update();

		std::vector<Entity> GetEntities() { return m_Entities; }

		template<typename... T>
		std::vector<Entity>& GetEntitiesWithComponent()
		{
			std::vector<Entity> entitiesWithComponent;
			for (auto& entity : m_Entities)
			{
				if ((entity.GetComponent<T>() && ...))
				{
					entitiesWithComponent.push_back(entity);
				}
			}
			return entitiesWithComponent;
		}

	private:
		std::vector<Entity> m_Entities;
		std::vector<System*> m_Systems;
	};
}