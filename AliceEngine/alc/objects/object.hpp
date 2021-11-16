#ifndef ALC_OBJECTS_GAMEOBJECT_HPP
#define ALC_OBJECTS_GAMEOBJECT_HPP
#include "../common.hpp"
#include "../datatypes/timestep.hpp"
#include "../datatypes/function.hpp"
#include "component_info.hpp"

namespace alc {

	class object;
	class component;

	// add data and logic to gameobjects
	class component {
	public:

		component();
		virtual ~component() = 0;

		// virtual functions

		virtual void on_create() { }
		virtual void on_destroy() { }
		virtual void on_update(timestep ts) { }

		// returns the attached object
		object* get_object() const;

		// returns whether this component should actively update or not
		bool get_should_update() const;

		// sets if this component should actively update or not
		void set_should_update(bool shouldUpdate);

		// deletes this component
		bool destroy_this();

	private:
		object* m_object;
		bool m_shouldUpdate : 1;
	public:
		void __set_object(object* object);
	};

	// objects can be anything from a scene to an entity or even a singleton
	// objects that exists in the world with a name and components and other relative objects
	class object {
	public:

		object(const std::string& name = "");
		virtual ~object();

		// returns the parent object
		object* get_parent() const;

		// reparents this object, can be set to nullptr to unparent
		void set_parent(object* parent);

		// returns the name of this object
		std::string get_name() const;

		// sets the name of this object
		void set_name(const std::string& name);


		//// gameobjects

		// creates an object attached to this object
		object* create_object(const std::string& name = "");

		// creates an object attached to this object with component of type
		template<class T> T* create_object(const std::string& name = "");

		// deletes the object
		bool destroy_object(object* object);

		// deletes this object
		bool destroy_this();


		//// components

		// creates a component of type attached to this object
		template<class T> T* create_component();

		// deletes a component
		bool delete_component(component* c);

		// gets a component of type attached to this object
		template<class T> T* get_component();

		// gets multiple components of type attached to this object
		template<class T> size_t get_components(std::vector<T*>& outComponents);

	private:

		object* m_parent;

		std::string m_name;

		std::vector<component*> m_components;
		std::vector<object*> m_subobjects;

	public:

		void __remove_subobject(object* o);
		void __delete_component(component* c);
	};

	// template definitions

	template<class T>
	inline T* object::create_object(const std::string& name) {
		object* object = create_object(name);
		return object->create_component<T>();
	}

	template<class T>
	inline T* object::create_component() {
		// try to get prexisting one if we can only have one component of this type
		if (component_only_one_v<T>) {
			if (T* c = get_component<T>()) return c;
		}
		// create new
		T* c = new T();
		m_components.push_back(c);
		m_components.back()->__set_object(this);
		m_components.back()->on_create();
		return c;
	}

	template<class T>
	inline T* object::get_component() {
		for (auto* c : m_components) {
			if (T* t = dynamic_cast<T*>(c)) {
				return t;
			}
		}
		return nullptr;
	}

	template<class T>
	inline size_t object::get_components(std::vector<T*>& outComponents) {
		size_t total = 0;
		for (auto* c : m_components) {
			if (T* t = dynamic_cast<T*>(c)) {
				++total;
				outComponents.push_back(t);
				if (component_only_one_v<T>) return 1; // quit early
			}
		}
		return total;
	}

}

#endif // !ALC_OBJECTS_GAMEOBJECT_HPP