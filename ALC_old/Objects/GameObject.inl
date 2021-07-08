

inline ALC::Component::Component()
	: m_object(nullptr) { }

inline ALC::Component::~Component() { }

inline ALC::GameObject* ALC::Component::GetObject() {
	return m_object;
}

inline void ALC::Component::__SetObject(GameObject* object) {
	m_object = object;
}

inline ALC::Behavior::Behavior()
	: m_object(nullptr) { }

inline ALC::Behavior::~Behavior() {

}

inline ALC::GameObject* ALC::Behavior::Instantiate() {
	return m_object->Instantiate();
}

inline bool ALC::Behavior::Destroy(GameObject* object) {
	return m_object->Destroy(object);
}

template<typename T>
inline T* ALC::Behavior::Instantiate() {
	return m_object->Instantiate<T>();
}

template<typename T>
inline T* ALC::Behavior::GetComponent() {
	ALC_ASSERT(m_object != nullptr, "Behavior has not been properly or fully created");
	return m_object->GetComponent<T>();
}

template<typename T>
inline ALC::vector<T*> ALC::Behavior::GetComponents(ALC::size_t max) {
	ALC_ASSERT(m_object != nullptr, "Behavior has not been properly or fully created");
	return m_object->GetComponents<T>(max);
}

template<typename T>
inline T* ALC::Behavior::AddComponent() {
	ALC_ASSERT(m_object != nullptr, "Behavior has not been properly or fully created");
	return m_object->AddComponent<T>();
}

template<typename T>
inline bool ALC::Behavior::DestroyComponent(T* component) {
	ALC_ASSERT(m_object != nullptr, "Behavior has not been properly or fully created");
	return m_object->DestroyComponent<T>(component);
}

inline ALC::GameObject* ALC::Behavior::GetObject() const {
	return m_object;
}

inline void ALC::Behavior::__SetObject(GameObject* object) {
	m_object = object;
}

inline ALC::GameObject::GameObject()
	: m_factory(nullptr), m_shouldCheckForDeletable(false) {
	m_transform.__SetObject(this);
}

inline ALC::GameObject::~GameObject() {
	// delete components
	for (auto [c, id] : m_components) delete c;
	m_components.clear();

	// delete behaviors
	for (auto [b, _delete] : m_behaviors) {
		b->OnDestroy();
		delete b;
	}
	m_behaviors.clear();
}

inline ALC::GameObject* ALC::GameObject::Instantiate() {
	return m_factory->Instatiate();
}

inline bool ALC::GameObject::Destroy(GameObject* object) {
	return m_factory->Destroy(object);
}

template<typename T>
inline T* ALC::GameObject::Instantiate() {
	GameObject* object = m_factory->Instatiate();
	T* component = object->Instantiate<T>();
	return component;
}

template<typename T>
inline T* ALC::GameObject::GetComponent() {
	// return transform as is
	if constexpr (std::is_same_v<T, Transform>) {
		return &m_transform;
	}
	// search for component of type
	else if constexpr (std::is_base_of_v<Component, T>) {
		for (auto [c, id] : m_components)
			if (T* component = dynamic_cast<T*>(c))
				return component;
	}
	// search for behavior of type
	else if constexpr (std::is_base_of_v<Behavior, T>) {
		for (auto [b, _delete] : m_behaviors)
			if (T* behavior = dynamic_cast<T*>(b); behavior && !_delete)
				return behavior;
	}
	// not component nor behavior 
	// or wasnt found
	// return nullptr
	return nullptr;
}

template<typename T>
inline ALC::vector<T*> ALC::GameObject::GetComponents(ALC::size_t max) {
	// quick exit
	if (max == 0) return {};

	// return transform as is
	if constexpr (std::is_same_v<T, Transform>) {
		return { &m_transform };
	}
	// search for components of type
	else if constexpr (std::is_base_of_v<Component, T>) {
		vector<T*> components;
		if (max != -1) components.reserve(max);
		for (auto [c, id] : m_components) {
			if (T* component = dynamic_cast<T*>(c)) {
				if (max != -1) --max;
				components.push_back(component);
			}
			if (max == 0) break;
		}
		return components;
	}
	// search for behaviors of type
	else if constexpr (std::is_base_of_v<Behavior, T>) {
		vector<T*> behaviors;
		if (max != -1) behaviors.reserve(max);
		for (auto [b, _delete] : m_behaviors) {
			if (T* behavior = dynamic_cast<T*>(b); behavior && !_delete) {
				if (max != -1) --max;
				behaviors.push_back(behavior);
			}
			if (max == 0) break;
		}
		return behaviors;
	}
	// not component nor behavior 
	// return empty vector
	return {};
}

template<typename T>
inline T* ALC::GameObject::AddComponent() {
	// return transform as is
	if constexpr (std::is_same_v<T, Transform>) {
		return &m_transform;
	}
	// create component of type
	else if constexpr (std::is_base_of_v<Component, T>) {
		T* component = new T();
		const TypeID id = GetTypeID<T>();
		m_components.emplace_back(component, id);
		component->__SetObject(this);
		if (auto* builder = m_factory->GetBuilder(id)) builder->OnCreate(component);
		return component;
	}
	// create behavior of type
	else if constexpr (std::is_base_of_v<Behavior, T>) {
		T* behavior = new T();
		m_behaviors.emplace_back(behavior, false);
		behavior->__SetObject(this);
		behavior->Start();
		return behavior;
	}
	// not component nor behavior 
	// return nullptr
	return nullptr;
}

template<typename T>
inline bool ALC::GameObject::DestroyComponent(T* component) {
	ALC_ASSERT(component != nullptr, "Component must not be null");
	// create component of type
	if constexpr (std::is_base_of_v<Component, T>) {
		// quick exit
		if (m_components.size() == 0) return false;

		// find and delete
		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			if ((*it) == component) {
				if (auto* builder = m_factory->GetBuilder(it->second))
					builder->OnDestroy(it->first);
				delete (it->first);
				m_components.erase(it);
				return true;
			}
		}
	}
	// create behavior of type
	else if constexpr (std::is_base_of_v<Behavior, T>) {
		// quick exit
		if (m_behaviors.size() == 0) return false;

		// find and mark
		for (auto it = m_behaviors.begin(); it != m_behaviors.end(); ++it) {
			if ((it->first) == component) {
				m_shouldCheckForDeletable = true;
				return it->second = true;
			}
		}
	}
	// not component nor behavior 
	// return nullptr
	return false;
}

inline void ALC::GameObject::__SetFactory(ObjectFactory* factory) {
	m_factory = factory;
}

inline void ALC::GameObject::__UpdateEvent(Timestep ts) {
	for (auto [b, _delete] : m_behaviors) {
		b->Step(ts);
	}

	if (m_shouldCheckForDeletable) {
		m_shouldCheckForDeletable = false;

		// search and delete
		for (auto it = m_behaviors.begin(); it != m_behaviors.end(); ++it) {
			if (it->second) {
				it->first->OnDestroy();
				delete it->first;
				it = m_behaviors.erase(it);
			}
		}
	}
}

