#ifndef ALC_OBJECTS_WORLD_HPP
#define ALC_OBJECTS_WORLD_HPP
#include "../common.hpp"
#include "../core/debug.hpp"
#include "../datatypes/function.hpp"
#include "component_info.hpp"
#include "../datatypes/timestep.hpp"

namespace alc {

	class object;
	class component;
	class scene;

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

		//// objects

		// creates an object attached to this object
		object* create_object(const std::string& name = "");

		// creates an object attached to this object with component of type
		template<class T> T* create_object(const std::string& name = "");

		// deletes the object
		bool destroy_object(object* object);


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
		object* m_object;
		bool m_shouldUpdate : 1;
	public:
		void __set_object(object* object);
	};

	// objects can be anything from a scene to an world_object or even a singleton
	// objects that exists in the world with a name and components and other relative objects
	class object {
	public:

		object();
		virtual ~object();

		virtual void on_create() { }
		virtual void on_destroy() { }
		virtual void on_update(timestep ts) { }

		// returns the parent object
		object* get_parent() const;

		// reparents this object, can be set to nullptr to unparent
		void set_parent(object* parent);

		// returns the name of this object
		std::string get_name() const;

		// sets the name of this object
		void set_name(const std::string& name);

		// returns whether this component should actively update or not
		bool get_should_update() const;

		// sets if this component should actively update or not
		void set_should_update(bool shouldUpdate);

		// returns the scene that this object is attached to
		// slow function, should be called once to obtain the scene
		// then reuse the return value
		scene* get_scene();


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
		bool m_shouldUpdate : 1;

		std::vector<component*> m_components;
		std::vector<object*> m_subobjects;

	public:

		void __remove_subobject(object* o);
		void __delete_component(component* c);
	};

	// contains and manages all objects
	class world final {
		ALC_STATIC_CLASS(world);
	public:

		//// group

		// creates a new generic object
		static object* create(const std::string& name = "");

		// creates a new object of type
		template<class T> static T* create(const std::string& name = "", object* parent = nullptr);

		// marks an object to be destroyed
		static bool destroy(object* o);

		// returns a global object,
		// object will never destroy until engine stops
		static object* global();

		// gets number of objects
		static size_t size();

		// gets object at index, 0 being the global object
		static object* get(size_t index);

		// gets object by name
		static object* get(const std::string& name);


		//// events

		// world update event
		static inline event<void, timestep> onWorldUpdate;

	private:

		static inline std::vector<object*> s_objects;

		static inline std::vector<object*> s_objectsToDelete;
		static inline std::vector<component*> s_componentsToDelete;

	public:
		static void __init();
		static void __exit();
		static void __update(timestep ts);
		static bool __delete_object(object* o);
		static bool __delete_component(component* c);
		//static void __remove_delete_object(object* o);
		//static void __remove_delete_component(component* c);
	};

	// template implementation

	template<class T>
	inline T* object::create_object(const std::string& name) {
		T* object = world::create<T>(name, this);
		return object;
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

	template<class T>
	inline T* component::create_object(const std::string& name) {
		return m_object->create_object<T>(name);
	}

	template<class T>
	inline T* component::create_component() {
		return m_object->create_component<T>();
	}

	template<class T>
	inline T* component::get_component() {
		return m_object->get_component<T>();
	}

	template<class T>
	inline size_t component::get_components(std::vector<T*>& outComponents) {
		return m_object->get_components<T>(outComponents);
	}

	template<class T>
	inline T* world::create(const std::string& name, object* parent) {
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}
		// create object of type
		if constexpr (std::is_base_of_v<object, T>) {
			T* o = new T();
			o->set_name(name);
			o->set_parent(parent);
			o->on_create();
			return o;
		}
		// else return nullptr
		return nullptr;
	}

}

#endif // !ALC_OBJECTS_WORLD_HPP