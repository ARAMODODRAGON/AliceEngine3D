#ifndef ALC_OBJECTS_OBJECTFACTORY_HPP
#define ALC_OBJECTS_OBJECTFACTORY_HPP
#include "../General.hpp"
#include "../Core/Timer.hpp"
#include "../Reflection/TypeID.hpp"

namespace ALC {

	struct Component;
	class GameObject;

	class ComponentBuilder {
	public:
		ComponentBuilder(TypeID componentID) : m_componentID(componentID) { }
		virtual ~ComponentBuilder() = 0 { }

		virtual void OnCreate(Component* component) = 0;
		virtual void OnDestroy(Component* component) = 0;

		TypeID GetComponentType() const { return m_componentID; }

	private:
		TypeID m_componentID;
	};

	class ObjectFactory final {
		ALC_NO_COPY(ObjectFactory);
	public:

		ObjectFactory();
		~ObjectFactory();

		// steps all objects 
		// at the end of the step, destroys all objects marked for destruction
		void Step(Timestep ts);

		// creates a new gameobject
		GameObject* Instatiate();

		// marks an object to be destroyed at the end of a step
		bool Destroy(GameObject* object);

		// attaches a builder of a specific component type
		// overwrites any already existing builder with the same type
		void AttachBuilder(ComponentBuilder* builder);

		// detaches builder that matches
		void DetachBuilder(ComponentBuilder* builder);

		// returns a builder with for the specified component type
		ComponentBuilder* GetBuilder(TypeID componentID);

	private:
		using ObjPair = pair<GameObject*, bool>;
		vector<ObjPair> m_objects;
		bool m_shouldCheckDeleteable;
		unordered_map<size_t, ComponentBuilder*> m_builders;
	};

}

#endif // !ALC_OBJECTS_OBJECTFACTORY_HPP