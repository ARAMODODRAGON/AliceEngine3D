#ifndef ALC_OBJECTS_WORLD_HPP
#define ALC_OBJECTS_WORLD_HPP
#include "../common.hpp"
#include "group.hpp"

namespace alc {

	// contains and manages all groups and attached gameobjects 
	class world final {
		ALC_STATIC_CLASS(world);
	public:

		//// group

		// creates a new group
		static group* create_group(const std::string& name = "");

		// creates a global group with behavior of type
		template<class T> static T* create_group(const std::string& name = "");

		// deletes a group
		static bool delete_group(group* g);

		// returns the primary global group
		static group* primary();

		// gets number of groups
		static size_t group_size();

		// gets group at index, 0 being the primary group
		static group* get_group(size_t index);

		// gets group by name
		static group* get_group(const std::string& name);


		//// events

		// world update event
		static inline event<void, timestep> onWorldUpdate;

	private:

		static inline std::vector<group*> s_groups;

		static inline std::vector<group*> s_groupsToDelete;
		static inline std::vector<behavior*> s_behaviorsToDelete;
		static inline std::vector<gameobject*> s_objectsToDelete;
		static inline std::vector<component*> s_componentsToDelete;

		static inline bool s_deletingState = false;

	public:
		static void __init();
		static void __exit();
		static void __update(timestep ts);
		static void __remove_group(group* g);
		static void __add_group(group* g);
		static bool __delete_behavior(behavior* b);
		static bool __delete_object(gameobject* o);
		static bool __delete_component(component* c);
		static void __remove_delete_group(group* g);
		static void __remove_delete_behavior(behavior* b);
		static void __remove_delete_object(gameobject* o);
		static void __remove_delete_component(component* c);
	};

	// template implementation

	template<class T>
	inline T* world::create_group(const std::string& name) {
		group* g = create_group(name);
		if (g == nullptr) return nullptr;
		T* behavior = g->create_behavior<T>();
		return behavior;
	}

	// detail

	namespace detail {

		inline event<void, timestep> onWorldBehaviorUpdate;
		inline event<void, timestep> onWorldComponentUpdate;

	}
}

#endif // !ALC_OBJECTS_WORLD_HPP