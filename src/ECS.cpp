#include "ECS.h"

ECS::ECS(int MAX_ENTITIES) : m_MAX_ENTITIES(MAX_ENTITIES) {
	for (int i = 0; i < MAX_ENTITIES; i++)
		m_availableIds.push(i);
}

EntityID ECS::CreateEntity() {
	LOG_ASSERT(m_availableIds.size() > 0, "Exceeded max entity count");
	
	EntityID id = m_availableIds.front();
	m_availableIds.pop();
	return id;
}

void ECS::DeleteEntity(EntityID id) {
	// TODO: remove components
	m_availableIds.push(id);
}

void ECS::PrintConnections() {
	for (auto& [name, container] : m_componentContainerMap) {
		std::cout << name << " IDS: [";

		std::vector<EntityID> ids = container.get()->GetAllIDs();
		for (int i = 0; i < ids.size(); i++) {
			std::cout << ids[i];
			if (i != ids.size() - 1)
				std::cout << ", ";
		}
		std::cout << "]\n";
	}
}