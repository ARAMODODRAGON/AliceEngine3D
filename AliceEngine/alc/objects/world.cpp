#include "world.hpp"

namespace alc {

	object* world::create(const std::string& name) {
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}
		object* o = new object();
		o->set_name(name);
		s_objects.push_back(o);
		return o;
	}

	bool world::destroy(object* o) {
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return false;
		}
		__delete_object(o);
		return true;
	}

	object* world::global() {
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}
		return s_objects[0];
	}

	size_t world::size() {
		return s_objects.size();
	}

	object* world::get(size_t index) {
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}
		return s_objects[index];
	}

	object* world::get(const std::string& name) {
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return nullptr;
		}

		for (auto* o : s_objects) {
			if (o->get_name() == name) {
				return o;
			}
		}

		return nullptr;
	}

	void world::__init() {
		if (s_objects.size() != 0) {
			ALC_DEBUG_ERROR("world is already enabled");
			return;
		}
		s_objectsToDelete.clear();
		s_componentsToDelete.clear();

		s_objects.reserve(20);
		s_objects.push_back(new object("GLOBAL"));
	}

	void world::__exit() {
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world is already disabled");
			return;
		}

		// clear everything
		s_objectsToDelete.clear();
		s_componentsToDelete.clear();

		// delete all groups
		s_deletingState = true;
		for (size_t i = 0; i < s_objects.size(); ++i) {
			delete s_objects[i];
		}
		s_objects.clear();
		s_deletingState = false;
	}

	void world::__update(timestep ts) {
		// first, update the world
		onWorldUpdate(ts);
		detail::onWorldBehaviorUpdate(ts);
		detail::onWorldComponentUpdate(ts);

		// now, delete stuff
		s_deletingState = true;

		// delete objects
		while (s_objectsToDelete.size()) {
			auto* object = s_objectsToDelete.back();
			s_objectsToDelete.pop_back();
		}

		// delete components
		while (s_componentsToDelete.size()) {
			auto* c = s_componentsToDelete.back();
			s_componentsToDelete.pop_back();
			c->get_object()->__delete_component(c);
		}

		s_deletingState = false;
	}

	bool world::__delete_object(object* o) {
		if (o == nullptr) return false;
		if (s_deletingState) return false;
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return false;
		}
		if (global() == o) {
			ALC_DEBUG_WARNING("cannot delete global object");
			return false;
		}

		// TODO: check if already in list
		s_objectsToDelete.push_back(o);
		return true;
	}

	bool world::__delete_component(component* c) {
		if (c == nullptr) return false;
		if (s_deletingState) return false;
		if (s_objects.size() == 0) {
			ALC_DEBUG_ERROR("world was not enabled or engine has not started");
			return false;
		}

		// TODO: check if already in list
		s_componentsToDelete.push_back(c);
		return true;
	}

	//void world::__remove_delete_object(object* o) {
	//	if (o == nullptr) return;
	//	for (size_t i = 0; i < s_objectsToDelete.size(); i++) {
	//		if (s_objectsToDelete[i] == o) {
	//			s_objectsToDelete.erase(s_objectsToDelete.begin() + i);
	//			return;
	//		}
	//	}
	//}
	//
	//void world::__remove_delete_component(component* c) {
	//	if (c == nullptr) return;
	//	for (size_t i = 0; i < s_componentsToDelete.size(); i++) {
	//		if (s_componentsToDelete[i] == c) {
	//			s_componentsToDelete.erase(s_componentsToDelete.begin() + i);
	//			return;
	//		}
	//	}
	//}


}