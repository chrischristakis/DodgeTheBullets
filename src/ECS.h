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
class BaseComponentPool {
public:
	virtual ~BaseComponentPool() { };
};

// Holds generic components
template <typename T>
class ComponentPool : public BaseComponentPool {
public:

	ComponentPool(int MAX_ENTITIES) {
		for (int i = 0; i < MAX_ENTITIES; i++)
			m_Components.push_back({ T() });
	}

	~ComponentPool() {
		m_Components.clear();
	}

	T& AddComponent(EntityID id, T&& component) {

		m_Components[id] = std::move(component);
		return m_Components[id];
	}

	T& GetComponent(EntityID id) {

		return m_Components[id];
	}

	void DeleteComponent(EntityID id) {

		m_Components[id] = T();
	}

private:

	// Using a map, so no data locality.
	std::vector<T> m_Components;

};



class ECS {
public:

	ECS(int MAX_ENTITIES);
	
	EntityID CreateEntity();
	void DeleteEntity(EntityID);

	template <typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();
		m_componentContainerMap.insert({ typeName, std::make_unique<ComponentPool<T>>(m_MAX_ENTITIES) });
	}
	
	template <typename T>
	T& AddComponent(EntityID id, T&& component) {
		const char* typeName = typeid(T).name();
		
		// Add the component to the map with an empty container list if it doesn't exist.
		if (m_componentContainerMap.find(typeName) == m_componentContainerMap.end())
			RegisterComponent<T>();

		BaseComponentPool* genericPtr = m_componentContainerMap[typeName].get();
		ComponentPool<T>* container = dynamic_cast<ComponentPool<T>*>(genericPtr);

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

		BaseComponentPool* genericPtr = m_componentContainerMap[typeName].get();
		ComponentPool<T>* container = reinterpret_cast<ComponentPool<T>*>(genericPtr);

		return container->GetComponent(id);
	}

	template <typename T>
	void DeleteComponent(EntityID id) {
		const char* typeName = typeid(T).name();

		LOG_ASSERT(m_componentContainerMap.find(typeName) != m_componentContainerMap.end(),
			"No components of type '" << typeName << "' exist");

		BaseComponentPool* genericPtr = m_componentContainerMap[typeName].get();
		ComponentPool<T>* container = reinterpret_cast<ComponentPool<T>*>(genericPtr);

		container->DeleteComponent(id);
	}

private:

	const int m_MAX_ENTITIES;

	// A queue which holds all of the ID's we can use for entities.
	std::queue<EntityID> m_availableIds;

	// Contains the name of the component as a key, and the container holding those components as a value.
	using TypeName = const char*;
	std::map<TypeName, std::unique_ptr<BaseComponentPool>> m_componentContainerMap;
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