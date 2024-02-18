#pragma once

#include <queue>
#include <map>
#include <string>
#include "Logging.h"

#include "Components/Transform.h"

using EntityID = unsigned int;

// Need this interface to store ComponentContainers in a map
class IComponentContainer {
public:
	virtual ~IComponentContainer() = default;
};

// Holds generic components
template <typename T>
class ComponentContainer : public IComponentContainer {
public:

	T* AddComponent(EntityID id, T component) {
		LOG_ASSERT(m_Components.find(id) == m_Components.end(),
			"Entity [" << std::to_string(id) << "] already has component of type '" << typeid(T).name() << "'");

		m_Components.insert({ id, component });
		return &m_Components[id];
	}

	T* GetComponent(EntityID id) {
		LOG_ASSERT(m_Components.find(id) != m_Components.end(),
			"Entity [" << std::to_string(id) << "] has no component of type '" << typeid(T).name() << "'");

		return &m_Components[id];
	}

private:

	std::map<EntityID, T> m_Components;
};



class ECS {
public:

	ECS(int MAX_ENTITIES);
	
	EntityID CreateEntity();
	void DeleteEntity(EntityID);

	/*
	template <typename T>
	T* AddComponent(EntityID id, T t) {
		m_componentContainerMap.insert({ "ho", ComponentContainer<Transform>() });
	}

	template <typename T>
	ComponentList<T> GetComponentList() {

	}
	*/

private:


	std::queue<EntityID> m_availableIds;
	const int m_MAX_ENTITIES;

	using TypeName = const char*;
	std::map<TypeName, IComponentContainer> m_componentContainerMap;
};