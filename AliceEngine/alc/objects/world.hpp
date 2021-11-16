#ifndef ALC_OBJECTS_WORLD_HPP
#define ALC_OBJECTS_WORLD_HPP
#include "../common.hpp"
#include "object.hpp"
#include "../core/debug.hpp"

namespace alc {

	// contains and manages all objects and subtypes 
	class world final {
		ALC_STATIC_CLASS(world);
	public:

		//// group

		// creates a new generic object
		static object* create(const std::string& name = "");

		// creates a new object of type
		template<class T> static T* create();

		// creates a new object of type
		template<class T> static T* create(const std::string& name = "");

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

		static inline bool s_deletingState = false;

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
	inline T* world::create() {
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}
		// create object of type
		if constexpr (std::is_base_of_v<object, T>) {
			T* o = new T();
			s_objects.push_back(o);
			return o;
		}
		// else return nullptr
		return nullptr;
	}

	template<class T>
	inline T* world::create(const std::string& name) {
		T* o = create<T>();
		if (o == nullptr) return nullptr;
		o->set_name(name);
		return o;
	}

	// detail

	namespace detail {

		inline event<void, timestep> onWorldBehaviorUpdate;
		inline event<void, timestep> onWorldComponentUpdate;

	}
}

#endif // !ALC_OBJECTS_WORLD_HPP