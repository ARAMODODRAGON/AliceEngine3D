#ifndef ALC_OBJECTS_WORLD_HPP
#define ALC_OBJECTS_WORLD_HPP
#include "../common.hpp"
#include "../core/debug.hpp"
#include "../core/alice_events.hpp"
#include "../datatypes/function.hpp"
#include "../datatypes/timestep.hpp"
#include "../reflection/typehash.hpp"

namespace alc {

	// a tag type to allow easy searching of objects
	enum object_tag : uint64 { };

	class object;

	// add data and logic to gameobjects
	class component {
	public:

		component();
		virtual ~component() = 0 { }

		// virtual functions

		virtual void on_create() { }
		virtual void on_destroy() { }

		// returns the attached object
		object* get_object() const;

		// returns the closest ancestor of type
		// slow function
		template<typename T> T* get_ancestor() const;

		// returns the topmost ancestor
		object* get_first_ancestor();

		// deletes this component
		bool destroy();

	private:
		object* m_object;
	public:
		void __set_object(object* object);
	};

	// objects can be anything from a scene to a player or even a singleton
	class object {
	public:

		object();
		virtual ~object();

		virtual void on_create() { }
		virtual void on_destroy() { }

		// returns the parent object
		object* get_parent() const;

		// reparents this object, can be set to nullptr to unparent
		void set_parent(object* parent);

		// returns the closest ancestor of type
		// slow function
		template<typename T> T* get_ancestor() const;

		// returns the topmost ancestor
		object* get_first_ancestor();

		// returns the name of this object
		std::string get_name() const;

		// sets the name of this object
		void set_name(const std::string& name);

		// returns the tag
		object_tag get_tag() const;

		// sets the tag
		void set_tag(object_tag tag);


		//// objects

		// creates an object attached to this object of type
		template<class T> T* create_object(const std::string& name = "");

		// deletes this object
		bool destroy();


		//// components

		// creates a component of type attached to this object
		template<class T> T* create_component();

		// gets a component of type attached to this object
		template<class T> T* get_component();

		// gets multiple components of type attached to this object
		// returns the size of the out vector
		template<class T> size_t get_components(std::vector<T*>& outComponents);


		/// events

		// called when the parent of this object changes
		// void (object* oldParent, object* newParent)
		event<void, object*, object*> onParentChanged;

	private:

		object* m_parent;

		std::string m_name;
		object_tag m_tag;

		std::vector<component*> m_components;
		std::vector<object*> m_children;

	public:
		void __set_parent(object* parent);
		void __remove_child(object* o);
		void __remove_component(component* c);
	};

	// contains and manages all objects
	class world final {
		ALC_STATIC_CLASS(world);
	public:

		//// objects

		// creates a new object of type,
		// if no parent is specified, object becomes global and 
		// no longer depends on the lifetime of another object
		template<class T> static T* create(const std::string& name = "", object* parent = nullptr);

		// marks an object to be destroyed
		static bool destroy(object* o);

		// marks a component to be destroyed
		static bool destroy(component* c);

		// gets number of global objects
		static size_t globals_size();

		/// search functions

		//// finds an object of type
		//template<typename T> T* find_object_of_type(bool mustBeGlobal = false);
		//
		//// finds objects of type
		//template<typename T> size_t find_objects_of_type(std::vector<T*>& out, bool mustBeGlobal = false);
		//
		//// finds an object with name
		//object* find_object_with_name(bool mustBeGlobal = false);
		//
		//// finds objects with name
		//size_t find_objects_with_name(std::vector<object*>& out, bool mustBeGlobal = false);

	private:

		static inline std::vector<object*> s_globalObjects;

		static inline std::vector<object*> s_objectsToDelete;
		static inline std::vector<component*> s_componentsToDelete;

	public:
		static void __init();
		static void __exit();
		static void __update();
		static void __add_global(object* obj);
		static void __remove_global(object* obj);
	};

	// template implementation

	template<typename T>
	inline T* component::get_ancestor() const {
		if (m_object) {
			if (T* t = dynamic_cast<T*>(m_object))
				return t;
			return m_object->get_ancestor<T>();
		}
		return nullptr;
	}

	template<typename T>
	inline T* object::get_ancestor() const {
		if (m_parent) {
			if (T* t = dynamic_cast<T*>(m_parent))
				return t;
			return m_parent->get_ancestor<T>();
		}
		return nullptr;
	}

	template<class T>
	inline T* object::create_object(const std::string& name) {
		return world::create<T>(name, this);
	}

	template<class T>
	inline T* object::create_component() {
		if constexpr (std::is_base_of_v<component, T>) {
			T* c = new T();
			c->__set_object(this);
			m_components.push_back(c);
			c->on_create();
			return c;
		}
		return nullptr;
	}

	template<class T>
	inline T* object::get_component() {
		if constexpr (std::is_base_of_v<component, T>)
			for (size_t i = 0; i < m_components.size(); i++)
				if (T* t = dynamic_cast<T*>(m_components[i]))
					return t;
		return nullptr;
	}

	template<class T>
	inline size_t object::get_components(std::vector<T*>& outComponents) {
		if constexpr (std::is_base_of_v<component, T>)
			for (size_t i = 0; i < m_components.size(); i++)
				if (T* t = dynamic_cast<T*>(m_components[i]))
					outComponents.push_back(t);
		return outComponents.size();
	}

	template<class T>
	inline T* world::create(const std::string& name, object* parent) {
		if constexpr (std::is_base_of_v<object, T> || std::is_same_v<object, T>) {
			T* o = new T();
			o->set_name(name);
			if (parent) o->__set_parent(parent);
			else __add_global(o);
			o->on_create();
			return o;
		}
		return nullptr;
	}


}

#endif // !ALC_OBJECTS_WORLD_HPP