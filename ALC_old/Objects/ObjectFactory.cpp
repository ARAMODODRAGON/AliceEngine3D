#include "ObjectFactory.hpp"
#include "GameObject.hpp"

ALC::ObjectFactory::ObjectFactory() : m_shouldCheckDeleteable(false) {
	m_objects.reserve(30);
}

ALC::ObjectFactory::~ObjectFactory() {
	// destroy all objects
	for (auto [go, _0] : m_objects) delete go;
	m_objects.clear();
}

void ALC::ObjectFactory::Step(Timestep ts) {
	// step
	for (size_t i = 0; i < m_objects.size(); i++) {
		auto* go = m_objects[i].first;
		go->__UpdateEvent(ts);
	}

	// delete
	if (m_shouldCheckDeleteable) {
		m_shouldCheckDeleteable = false;

		for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
			if (it->second) {
				delete it->first;
				it = m_objects.erase(it);
			}
		}
	}
}

ALC::GameObject* ALC::ObjectFactory::Instatiate() {
	GameObject* go = new GameObject();
	m_objects.push_back({ go, false });
	return go;
}

bool ALC::ObjectFactory::Destroy(GameObject* object) {
	for (auto [go, shouldDelete] : m_objects) {
		if (go == object) {
			shouldDelete = true;
			m_shouldCheckDeleteable = true;
			return true;
		}
	}
	return false;
}

void ALC::ObjectFactory::AttachBuilder(ComponentBuilder* builder) { 
	auto it = m_builders.find(HashType(builder->GetComponentType()));
	if (it != m_builders.end()) it->second = builder;
	else						m_builders.emplace(HashType(builder->GetComponentType()), builder);
}

void ALC::ObjectFactory::DetachBuilder(ComponentBuilder* builder) { 
	m_builders.erase(HashType(builder->GetComponentType()));
}

ALC::ComponentBuilder* ALC::ObjectFactory::GetBuilder(TypeID componentID) {
	auto it = m_builders.find(HashType(componentID));
	if (it != m_builders.end()) return it->second;
	return nullptr;
}
