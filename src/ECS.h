#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <queue>
#include "Logging.h"
#include "Components/Component.h"

/*! Unique identifier that binds data together in ECS
*/
using EntityID = unsigned int;

class ECS {
private:

	using TypeName = const char*;

	// Component name and a non-owning pointer to the component data
	using ComponentMap = std::unordered_map<TypeName, Component*>;

	/*! Relevant info pertaining to an entity
	*/
	struct EntityInfo {
		ComponentMap componentMap;
		bool inUse = false;
	};

	struct BaseComponentPool {
	public:
		virtual ~BaseComponentPool() { }
	};

	template <typename T>
	class ComponentPool : public BaseComponentPool {
	public:
		

		ComponentPool(int MAX_ENTITIES) : m_MAX_ENTITIES(MAX_ENTITIES) {

			// Reserve space and removes the exponential copies that happen as the vector resizes
			m_pool.reserve(MAX_ENTITIES);

			for (int i = 0; i < MAX_ENTITIES; i++)
				m_pool.push_back(T());

		}

		template <typename T>
		T* AddComponent(EntityID id, T&& component) {
			LOG_ASSERT(id >= 0 && id < m_MAX_ENTITIES, "Entity index out of range");

			m_pool[id] = component;
			return &m_pool[id];
		}

		template <typename T>
		T* GetComponent(EntityID id) {
			LOG_ASSERT(id >= 0 && id < m_MAX_ENTITIES, "Entity index out of range");

			return &m_pool[id];
		}

	private:

		int m_MAX_ENTITIES;

		/*! Contains a list of similar components T for every entity
		*   Each index corresponds to the entity ID, it is important this pool matches MAX_ENTTITIES in ECS
		*/
		std::vector<T> m_pool;

	};

public:

	ECS(int MAX_ENTITIES): m_MAX_ENTITIES(MAX_ENTITIES) {

		m_entityPool.reserve(MAX_ENTITIES);

		// Initialize entity pool for each entity ID
		for (int i = 0; i < MAX_ENTITIES; i++) {
			m_entityPool.emplace_back(EntityInfo());
			m_availableEntities.push(i);
		}

	}

	/*! Return an Entity ID that is ready for use, used to index the rest of the ECS
	*/
	EntityID CreateEntity() {
		LOG_ASSERT(!m_availableEntities.empty(), "Not enough entities available!");

		EntityID id = m_availableEntities.front();
		m_availableEntities.pop();

		LOG_ASSERT(!m_entityPool[id].inUse, "Creating an entity that is already in use");

		// Set this entity to in use
		m_entityPool[id].inUse = true;

		return id;
	}

	/*! Allocate a new memory pool for a generic component
	*/
	template <typename T>
	void RegisterComponent() {
		TypeName componentName = typeid(T).name();

		LOG_ASSERT(m_componentPools.find(componentName) == m_componentPools.end(), "Component '" << componentName << "' is already registered!");

		m_componentPools.insert({ componentName, std::make_unique<ComponentPool<T>>(m_MAX_ENTITIES) });
	}

	/*! Create and attach a component to an entity
	* 
	* @param id: ID of the entity we wish to attach a component to
	* @param component: rvalue to initialize component
	* 
	*/
	template <typename T>
	T& AddComponent(EntityID id, T&& component = {}) {
		LOG_ASSERT(id >= 0 && id < m_MAX_ENTITIES, "Entity index out of range");
		LOG_ASSERT(m_entityPool[id].inUse, "Trying to add component to entity with id " << id << " that is not in use");

		TypeName componentName = typeid(T).name();
		ComponentMap& componentMap = m_entityPool[id].componentMap;

		LOG_ASSERT(componentMap.find(componentName) == componentMap.end(), "Entity already has component '" << componentName << "'");
		
		// If component is not yet registered, then go ahead and register it.
		if (m_componentPools.find(componentName) == m_componentPools.end())
			RegisterComponent<T>();
		
		// Get the associated component pool for this component by recasting
		BaseComponentPool* genericPtr = m_componentPools[componentName].get();
		ComponentPool<T>* pool = dynamic_cast<ComponentPool<T>*>(genericPtr);

		LOG_ASSERT(pool, "Dynamic cast for component pool of type '" << componentName << "' failed");

		T* componentPtr = pool->AddComponent<T>(id, std::move(component));

		// Entity now contains the component we just added to it
		componentMap.insert({ componentName, componentPtr });

		return *componentPtr;
	}

	/*! Returns true if an entity has the specified component and false if not
	*
	* @param id: ID of the entity we wish to retrieve the component for
	*
	*/
	template <typename T>
	bool HasComponent(EntityID id) {
		LOG_ASSERT(id >= 0 && id < m_MAX_ENTITIES, "Entity index out of range");
		LOG_ASSERT(m_entityPool[id].inUse, "Trying to check for entity with id " << id << " that is not in use");
		
		ComponentMap& componentMap = m_entityPool[id].componentMap;
		TypeName componentName = typeid(T).name();

		return componentMap.find(componentName) != componentMap.end();
	}

	/*! Returns true if an entity has ALL of the specified components and false if not
	*
	* @usage: HasComponents<Transform, Physics>(entity);
	* @param id: ID of the entity we wish to retrieve the component for
	*
	*/
	template <typename... Ts>
	bool HasComponents(EntityID id) {
		// Fold operator, checks if the entity has each component in the parameter pack
		return (HasComponent<Ts>(id) && ...);
	}

	/*! Retrieve a reference to the component for a particular entity
	*
	* @param id: ID of the entity we wish to retrieve the component for
	*
	*/
	template <typename T>
	T& GetComponent(EntityID id) {
		LOG_ASSERT(id >= 0 && id < m_MAX_ENTITIES, "Entity index out of range");
		LOG_ASSERT(m_entityPool[id].inUse, "Trying to get component for entity with id " << id << " that is not in use");

		TypeName componentName = typeid(T).name();
		ComponentMap& componentMap = m_entityPool[id].componentMap;

		LOG_ASSERT(componentMap.find(componentName) != componentMap.end(), "Entity has no component '" << componentName << "'");
		LOG_ASSERT(m_componentPools.find(componentName) != m_componentPools.end(), "Component '" << componentName << "' is not registered");

		BaseComponentPool* genericPtr = m_componentPools[componentName].get();
		ComponentPool<T>* pool = dynamic_cast<ComponentPool<T>*>(genericPtr);

		LOG_ASSERT(pool, "Dynamic cast for component pool of type '" << componentName << "' failed");

		return *pool->GetComponent<T>(id);
	}

	/*! Remove a component from an entity
	*
	* @param id: ID of the entity we wish to remove the component from
	*
	*/
	template <typename T>
	void RemoveComponent(EntityID id) {
		LOG_ASSERT(id >= 0 && id < m_MAX_ENTITIES, "Entity index out of range");
		LOG_ASSERT(m_entityPool[id].inUse, "Trying to delete entity with id " << id << " that is not in use");

		TypeName componentName = typeid(T).name();
		ComponentMap& componentMap = m_entityPool[id].componentMap;

		LOG_ASSERT(componentMap.find(componentName) != componentMap.end(), "Entity with id: " << id << " has no component '" << componentName << "' to remove");
		componentMap.erase(componentName);
	}

	/*! Free up an entity and remove its associations to be returned to the available pool for use
	*
	* @param id: ID of the entity we wish to delete
	*
	*/
	void DeleteEntity(EntityID id) {
		LOG_ASSERT(id >= 0 && id < m_MAX_ENTITIES, "Entity index out of range");
		LOG_ASSERT(m_entityPool[id].inUse, "Trying to delete entity with id " << id << " that is not in use");

		EntityInfo& info = m_entityPool[id];

		// Remove all component associations and reset entity
		info.componentMap.clear();
		info.inUse = false;

		m_availableEntities.push(id);
	}

	/*! Returns a vector that contains the IDs of every entity in use
	*/
	std::vector<EntityID> GetAllActiveIDs() {
		std::vector<EntityID> res;
		for (int i = 0; i < m_entityPool.size(); i++) {
			if (m_entityPool[i].inUse)
				res.push_back(i);
		}
		return res;
	}

	/*! Returns a vector that contains the IDs of every entity in use with the specified components
	*/
	template <typename... Ts>
	std::vector<EntityID> GetAllActiveIDs() {
		std::vector<EntityID> res;
		for (int i = 0; i < m_entityPool.size(); i++) {
			if (m_entityPool[i].inUse && HasComponents<Ts...>(i))
				res.push_back(i);
		}
		return res;
	}

	/*
	*  HELPFUL PRINTING STATEMENTS FOR DEBUGGING
	*  - Slow though, should not be used in release
	*/
	void PrintAvailableEntities() {
		std::queue copy = m_availableEntities;
		std::cout << "Available Entities:\n";
		while (!copy.empty()) {
			std::cout << copy.front() << " ";
			copy.pop();
		}
		std::cout << std::endl;
	}

	void PrintEntityComponents(EntityID id) {
		LOG_ASSERT(id >= 0 && id < m_MAX_ENTITIES, "Entity index out of range");
		LOG_ASSERT(m_entityPool[id].inUse, "Cannot print components for entity " << id << " that is not in use");

		EntityInfo& info = m_entityPool[id];
		std::cout << "Entity [" << id << "] components:\n";
		for (auto& [name, ptr] : info.componentMap)
			std::cout << " - " << name << "\n";
	}

	void PrintRegisteredComponents() {
		std::cout << "Registered components:\n";
		for (auto& [name, ptr] : m_componentPools)
			std::cout << " - " << name << "\n";
	}

private:

	// Max capacity of the ECS object pool
	int m_MAX_ENTITIES;

	// Entities are just sets that define what components belong to an entity, entity ID is the index.
	std::vector<EntityInfo> m_entityPool;
	std::queue<EntityID> m_availableEntities;

	// Each component has a component pool associated with it
	std::unordered_map<TypeName, std::unique_ptr<BaseComponentPool>> m_componentPools;

};