#ifndef ALC_OBJECTS_WORLD_HPP
#define ALC_OBJECTS_WORLD_HPP
#include "../common.hpp"
#include "../core/debug.hpp"
#include "../core/alice_events.hpp"
#include "../datatypes/function.hpp"
#include "../datatypes/timestep.hpp"
#include "../reflection/typehash.hpp"

namespace alc {

	class object;

	class iscript {
	public:
		virtual ~iscript() = 0 { }
		virtual bool load(const std::string& filepath) = 0;
		virtual void message(object* self, const std::string& function) = 0; // TODO: add arguments
	};

	// a tag type to allow easy searching of objects
	enum object_tag : uint64 { };

	// objects can be anything from a scene to a player or even a singleton
	class object {
	public:

		object();
		virtual ~object();

		virtual void on_create() { }
		virtual void on_destroy() { }
		virtual void on_update(alc::timestep ts) { }

		// returns the parent object
		object* get_parent() const;

		// reparents this object, can be set to nullptr to unparent
		void set_parent(object* parent);

		// returns the closest ancestor of type
		// slow function
		template<typename T> T* get_ancestor() const;

		// returns the topmost ancestor
		object* get_first_ancestor();

		// returns if update is enabled
		bool get_update() const;

		// sets if update is enabled
		void set_update(bool set);

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


		/// scripts

		// creates a new script and attaches it to this object
		// deletes any preexisting script before loading new script
		template<typename T>
		T* load_script(const std::string& path);

		// deletes the current script
		bool close_script();

		// sends a message to the script on this object
		// TODO: add 'args'
		void message(const std::string& function, uint32 recursiveDepth = 0);


		/// events

		// called when the parent of this object changes
		// void (object* oldParent, object* newParent)
		event<void, object*, object*> onParentChanged;

	private:

		bool m_shouldUpdate;

		object* m_parent;

		std::string m_name;
		object_tag m_tag;

		std::vector<object*> m_children;
		std::unique_ptr<iscript> m_script;

	public:
		void __set_parent(object* parent);
		void __remove_child(object* o);
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

		// gets number of global objects
		static size_t globals_size();

		/// search functions

		//// finds an object of type
		//template<typename T> T* find_object_of_type(bool recursive = false);
		//
		//// finds objects of type
		//template<typename T> size_t find_objects_of_type(std::vector<T*>& out, bool recursive = false);
		//
		//// finds an object with name
		//object* find_object_with_name(bool recursive = false);
		//
		//// finds objects with name
		//size_t find_objects_with_name(std::vector<object*>& out, bool recursive = false);

	private:

		static inline std::vector<object*> s_globalObjects;

		static inline std::vector<object*> s_objectsToDelete;

	public:
		static void __init();
		static void __exit();
		static void __update();
		static void __add_global(object* obj);
		static void __remove_global(object* obj);
	};

	// template implementation

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

	template<typename T>
	inline T* object::load_script(const std::string& path) {
		if constexpr (std::is_base_of_v<iscript, T>) {
			T* script = new T();
			if (script->load(path)) {
				m_script.reset(script);
				return script;
			}
			delete script;
		}
		return nullptr;
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