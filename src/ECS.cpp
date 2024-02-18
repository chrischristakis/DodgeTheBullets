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