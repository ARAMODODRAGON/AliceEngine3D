#include "world.hpp"
#include "../core/debug.hpp"

namespace alc {

	group* world::create_group(const std::string& name) {
		if (s_groups.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}
		// create, add, and return
		group* g = new group(name);
		s_groups.push_back(g);
		return g;
	}

	bool world::delete_group(group* g) {
		if (g == nullptr) return false;
		if (s_deletingState) return false;
		if (s_groups.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return false;
		}

		// cannot delete primary group
		if (s_groups[0] == g) {
			ALC_DEBUG_WARNING("cannot delete primary group");
			return false;
		}

		// mark for deletion and return true
		// TODO: check if already in list
		s_groupsToDelete.push_back(g);
		return true;
	}

	group* world::primary() {
		if (s_groups.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}
		return s_groups[0];
	}

	size_t world::group_size() {
		return s_groups.size();
	}

	group* world::get_group(size_t index) {
		if (s_groups.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}
		return s_groups[index];
	}

	group* world::get_group(const std::string& name) {
		if (s_groups.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}

		for (auto* g : s_groups) {
			if (g->get_name() == name) {
				return g;
			}
		}

		return nullptr;
	}

	void world::__init() {
		if (s_groups.size() != 0) {
			ALC_DEBUG_ERROR("world is already enabled");
			return;
		}
		s_groupsToDelete.clear();
		s_behaviorsToDelete.clear();
		s_objectsToDelete.clear();
		s_componentsToDelete.clear();

		s_groups.reserve(20);
		s_groups.push_back(new group("PRIMARY"));
	}

	void world::__exit() {
		if (s_groups.size() == 0) {
			ALC_DEBUG_ERROR("world is already disabled");
			return;
		}

		// clear everything
		s_groupsToDelete.clear();
		s_behaviorsToDelete.clear();
		s_objectsToDelete.clear();
		s_componentsToDelete.clear();

		// delete all groups
		s_deletingState = true;
		for (auto* g : s_groups) delete g;
		s_groups.clear();
		s_deletingState = false;
	}

	void world::__update(timestep ts) {
		// first, update the world
		onWorldUpdate(ts);
		detail::onWorldBehaviorUpdate(ts);
		detail::onWorldComponentUpdate(ts);

		// now, delete stuff
		s_deletingState = true;

		// delete groups
		while (s_groupsToDelete.size()) {
			auto* g = s_groupsToDelete.back();
			s_groupsToDelete.pop_back();
			if (g->get_parent())
				g->get_parent()->__delete_group(g);
			else {
				__remove_group(g);
				delete g;
			}
		}

		// delete behaviors
		while (s_behaviorsToDelete.size()) {
			auto* b = s_behaviorsToDelete.back();
			s_behaviorsToDelete.pop_back();
			b->get_group()->__delete_behavior(b);
		}

		// delete objects
		while (s_objectsToDelete.size()) {
			auto* object = s_objectsToDelete.back();
			s_objectsToDelete.pop_back();
			object->get_group()->__delete_object(object);
		}

		// delete components
		while (s_componentsToDelete.size()) {
			auto* c = s_componentsToDelete.back();
			s_componentsToDelete.pop_back();
			c->get_object()->__delete_component(c);
		}

		s_deletingState = false;
	}

	void world::__remove_group(group* g) {
		for (size_t i = 0; i < s_groups.size(); i++) {
			if (s_groups[i] == g) {
				s_groups.erase(s_groups.begin() + i);
				return;
			}
		}
	}

	void world::__add_group(group* g) {
		s_groups.push_back(g);
	}

	bool world::__delete_behavior(behavior* b) {
		if (b == nullptr) return false;
		if (s_deletingState) return false;
		if (s_groups.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return false;
		}

		// TODO: check if already in list
		s_behaviorsToDelete.push_back(b);
		return true;
	}

	bool world::__delete_object(gameobject* o) {
		if (o == nullptr) return false;
		if (s_deletingState) return false;
		if (s_groups.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return false;
		}

		// TODO: check if already in list
		s_objectsToDelete.push_back(o);
		return true;
	}

	bool world::__delete_component(component* c) {
		if (c == nullptr) return false;
		if (s_deletingState) return false;
		if (s_groups.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return false;
		}

		// TODO: check if already in list
		s_componentsToDelete.push_back(c);
		return true;
	}

	void world::__remove_delete_group(group* g) {
		if (g == nullptr) return;
		for (size_t i = 0; i < s_groupsToDelete.size(); i++) {
			if (s_groupsToDelete[i] == g) {
				s_groupsToDelete.erase(s_groupsToDelete.begin() + i);
				return;
			}
		}
	}

	void world::__remove_delete_behavior(behavior* b) {
		if (b == nullptr) return;
		for (size_t i = 0; i < s_behaviorsToDelete.size(); i++) {
			if (s_behaviorsToDelete[i] == b) {
				s_behaviorsToDelete.erase(s_behaviorsToDelete.begin() + i);
				return;
			}
		}
	}

	void world::__remove_delete_object(gameobject* o) {
		if (o == nullptr) return;
		for (size_t i = 0; i < s_objectsToDelete.size(); i++) {
			if (s_objectsToDelete[i] == o) {
				s_objectsToDelete.erase(s_objectsToDelete.begin() + i);
				return;
			}
		}
	}

	void world::__remove_delete_component(component* c) {
		if (c == nullptr) return;
		for (size_t i = 0; i < s_componentsToDelete.size(); i++) {
			if (s_componentsToDelete[i] == c) {
				s_componentsToDelete.erase(s_componentsToDelete.begin() + i);
				return;
			}
		}
	}


}