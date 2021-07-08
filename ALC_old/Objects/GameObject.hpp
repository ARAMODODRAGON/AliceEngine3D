#ifndef ALC_OBJECTS_GAMEOBJECT_HPP
#define ALC_OBJECTS_GAMEOBJECT_HPP
#include "Transform.hpp"
#include "ObjectFactory.hpp"

namespace ALC {

	class GameObject;

	struct Component {
		ALC_NO_COPY(Component);

		Component();
		virtual ~Component() = 0;

		// returns the object that this component is attached to
		GameObject* GetObject();

	private:
		GameObject* m_object;
	public:
		void __SetObject(GameObject* object);
	};


	struct Transform final : public Component {
		ALC::vec3 position;
		ALC::vec3 scale;
		ALC::quat rotation;

		Transform() : position(0.0f), scale(1.0f), rotation(ALC::vec3(0.0f)) { }
	};

	class Behavior {
		ALC_NO_COPY(Behavior);
	public:

		Behavior();
		virtual ~Behavior() = 0;

		virtual void Start() { }
		virtual void OnDestroy() { }
		virtual void Step(ALC::Timestep ts) { }

		// creates a new gameobject
		GameObject* Instantiate();

		// marks an object to be deleted
		bool Destroy(GameObject* object);

		// creates a new gameobject with component of type T and returns the component
		template<typename T> T* Instantiate();

		// returns component of type T
		template<typename T> T* GetComponent();

		// returns vector containing at most 'max' components of type T
		template<typename T> vector<T*> GetComponents(size_t max = -1);

		// creates a new component of type T and returns it
		// if component is Transform then it will return the existing transform on this object
		template<typename T> T* AddComponent();

		// destroys a component of type T
		// requires that the component be attached to this object
		// otherwise will result in failure and returns false
		template<typename T> bool DestroyComponent(T* component);

		// returns the GameObject that this behavior is attached to
		GameObject* GetObject() const;

	private:
		GameObject* m_object;
	public:
		void __SetObject(GameObject* object);
	};

	class GameObject final {
		ALC_NO_COPY(GameObject);
	public:

		GameObject();
		~GameObject();

		// creates a new gameobject
		GameObject* Instantiate();

		// marks an object to be deleted
		bool Destroy(GameObject* object);

		// creates a new gameobject with component of type T and returns the component
		template<typename T> T* Instantiate();

		// returns component of type T
		template<typename T> T* GetComponent();

		// returns vector containing at most 'max' components of type T
		template<typename T> vector<T*> GetComponents(size_t max = -1);

		// creates a new component of type T and returns it
		// if component is Transform then it will return the existing transform on this object
		template<typename T> T* AddComponent();

		// destroys a component of type T
		// requires that the component be attached to this object
		// otherwise will result in failure and returns false
		template<typename T> bool DestroyComponent(T* component);

	private:
		using CompPair = pair<Component*, TypeID>;
		using BehPair = pair<Behavior*, bool>;
		ObjectFactory* m_factory;
		Transform m_transform;
		vector<CompPair> m_components;
		vector<BehPair> m_behaviors;
		bool m_shouldCheckForDeletable;
	public:
		void __SetFactory(ObjectFactory* factory);
		void __UpdateEvent(Timestep ts);
	};

}

#include "GameObject.inl"

#endif // !ALC_OBJECTS_GAMEOBJECT_HPP