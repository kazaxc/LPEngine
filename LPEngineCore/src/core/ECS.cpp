#include "../../include/core/ECS.h"

namespace LPEngine
{
	void ECSManager::AddEntity(Entity entity)
	{
		m_Entities.push_back(entity);
	}

	void ECSManager::AddSystem(System* system)
	{
		m_Systems.push_back(system);
	}

	void ECSManager::Update()
	{
		for (auto& entity : m_Entities)
		{
			for (auto& system : m_Systems)
			{
				system->Update(entity);
			}
		}
	}
}