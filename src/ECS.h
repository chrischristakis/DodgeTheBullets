#pragma once

#include <queue>
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include "Logging.h"

#include "Components/Transform.h"

using EntityID = unsigned int;

class BaseComponentContainer {
public:
	virtual ~BaseComponentContainer() { };
};

// Holds generic components
template <typename T>
class ComponentContainer : public BaseComponentContainer {
public:

	~ComponentContainer() override {
		std::cout << "Destroying container\n";
		m_Components.clear();
	}

	T& AddComponent(EntityID id, T* component) {
		LOG_ASSERT(m_Components.find(id) == m_Components.end(),
			"Entity [" << std::to_string(id) << "] already has component of type '" << typeid(T).name() << "'");

		m_Components.insert({ id, std::unique_ptr<T>(component)});
		return *m_Components[id].get();
	}

	T& GetComponent(EntityID id) {
		LOG_ASSERT(m_Components.find(id) != m_Components.end(),
			"Entity [" << std::to_string(id) << "] has no component of type '" << typeid(T).name() << "'");

		return *m_Components[id].get();
	}

private:

	std::map<EntityID, std::unique_ptr<T>> m_Components;

};



class ECS {
public:

	ECS(int MAX_ENTITIES);
	
	EntityID CreateEntity();
	void DeleteEntity(EntityID);

	template <typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();
		m_componentContainerMap.insert({ typeName, std::make_unique<ComponentContainer<T>>()});
	}
	
	template <typename T>
	T& AddComponent(EntityID id, T* component) {
		const char* typeName = typeid(T).name();
		
		// Add the component to the map with an empty container list if it doesn't exist.
		if (m_componentContainerMap.find(typeName) == m_componentContainerMap.end())
			RegisterComponent<T>();

		BaseComponentContainer* genericPtr = m_componentContainerMap[typeName].get();
		ComponentContainer<T>* container = dynamic_cast<ComponentContainer<T>*>(genericPtr);

		// If dynamic cast fails
		LOG_ASSERT(container, "ECS dynamic cast failed when casting Component Container of type '" << typeName << "'");

		T& res = container->AddComponent(id, component);
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

private:

	std::queue<EntityID> m_availableIds;
	const int m_MAX_ENTITIES;

	using TypeName = const char*;
	std::map<TypeName, std::unique_ptr<BaseComponentContainer>> m_componentContainerMap;
};