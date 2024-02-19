#pragma once

#include <queue>
#include <map>
#include <any>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include "Logging.h"

#include "Components/Component.h"
#include "Components/Transform.h"

using EntityID = unsigned int;

// Base class needed for polymorphism and storing generic containers.
class BaseComponentContainer {
public:
	virtual ~BaseComponentContainer() { };

	// Prints out all IDs belonging to a component
	virtual std::vector<EntityID> GetAllIDs() = 0;
};

// Holds generic components
template <typename T>
class ComponentContainer : public BaseComponentContainer {
public:

	~ComponentContainer() {
		m_Components.clear();
	}

	T& AddComponent(EntityID id, T&& component) {
		LOG_ASSERT(m_Components.find(id) == m_Components.end(),
			"Entity [" << std::to_string(id) << "] already has component of type '" << typeid(T).name() << "'");

		m_Components.emplace(id, component);
		return m_Components[id];
	}

	T& GetComponent(EntityID id) {
		LOG_ASSERT(m_Components.find(id) != m_Components.end(),
			"Entity [" << std::to_string(id) << "] has no component of type '" << typeid(T).name() << "'");

		return m_Components[id];
	}

	void DeleteComponent(EntityID id) {
		LOG_ASSERT(m_Components.find(id) != m_Components.end(),
			"Entity [" << std::to_string(id) << "] has no component of type '" << typeid(T).name() << "'");

		m_Components.erase(id);
	}

	// Get all IDs registered to this component
	std::vector<EntityID> GetAllIDs() override {
		std::vector<EntityID> res;
		for (auto& [id, component] : m_Components)
			res.push_back(id);
		return res;
	}

private:

	// Using a map, so no data locality.
	std::unordered_map<EntityID, T> m_Components;

};



class ECS {
public:

	ECS(int MAX_ENTITIES);
	
	EntityID CreateEntity();
	void DeleteEntity(EntityID);

	template <typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();
		m_componentContainerMap.insert({ typeName, std::make_unique<ComponentContainer<T>>() });
	}
	
	template <typename T>
	T& AddComponent(EntityID id, T&& component) {
		const char* typeName = typeid(T).name();
		
		// Add the component to the map with an empty container list if it doesn't exist.
		if (m_componentContainerMap.find(typeName) == m_componentContainerMap.end())
			RegisterComponent<T>();

		BaseComponentContainer* genericPtr = m_componentContainerMap[typeName].get();
		ComponentContainer<T>* container = dynamic_cast<ComponentContainer<T>*>(genericPtr);

		// If dynamic cast fails
		LOG_ASSERT(container, "ECS dynamic cast failed when casting Component Container of type '" << typeName << "'");

		T& res = container->AddComponent(id, std::move(component));
		return res;
	}

	template <typename T>
	T& GetComponent(EntityID id) {
		const char* typeName = typeid(T).name();

		LOG_ASSERT(m_componentContainerMap.find(typeName) != m_componentContainerMap.end(), 
			"No components of type '" << typeName << "' exist");

		BaseComponentContainer* genericPtr = m_componentContainerMap[typeName].get();
		ComponentContainer<T>* container = reinterpret_cast<ComponentContainer<T>*>(genericPtr);

		return container->GetComponent(id);
	}

	template <typename T>
	void DeleteComponent(EntityID id) {
		const char* typeName = typeid(T).name();

		LOG_ASSERT(m_componentContainerMap.find(typeName) != m_componentContainerMap.end(),
			"No components of type '" << typeName << "' exist");

		BaseComponentContainer* genericPtr = m_componentContainerMap[typeName].get();
		ComponentContainer<T>* container = reinterpret_cast<ComponentContainer<T>*>(genericPtr);

		container->DeleteComponent(id);
	}

	// For debugging, prints out all registered components and the entities which have them.
	void PrintConnections();

private:

	const int m_MAX_ENTITIES;

	// A queue which holds all of the ID's we can use for entities.
	std::queue<EntityID> m_availableIds;

	// Contains the name of the component as a key, and the container holding those components as a value.
	using TypeName = const char*;
	std::map<TypeName, std::unique_ptr<BaseComponentContainer>> m_componentContainerMap;
};


// ENTITY TESTING

struct Entity {
	using TypeName = const char*;
	std::unordered_map<TypeName, Component> components;

	template <typename T>
	T& AddComponent(T&& component) {
		const char* typeName = typeid(T).name();

		LOG_ASSERT(components.find(typeName) == components.end(), "Entity already has component '" << typeName << "'")

		components.emplace(typeName, std::move(component));
		return static_cast<T&>(components[typeName]);
	}

	template <typename T>
	T& GetComponent() {
		const char* typeName = typeid(T).name();

		LOG_ASSERT(components.find(typeName) != components.end(),
			"No components of type '" << typeName << "' exist");

		return static_cast<T&>(components[typeName]);
	}

	template <typename T>
	void DeleteComponent() {
		const char* typeName = typeid(T).name();

		LOG_ASSERT(components.find(typeName) != components.end(),
			"No components of type '" << typeName << "' exist");

		components.erase(typeName);
	}
};