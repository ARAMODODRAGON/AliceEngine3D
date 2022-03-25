#include "world.hpp"

namespace alc {

	///////////////////////////////////////////////////////////////////////////////// 

	component::component()
		: m_object(nullptr) { }

	object* component::get_object() const {
		return m_object;
	}

	object* component::get_first_ancestor() {
		return m_object->get_first_ancestor();
	}

	bool component::destroy() {
		return world::destroy(this);
	}

	void component::__set_object(object* object) {
		m_object = object;
	}

	///////////////////////////////////////////////////////////////////////////////// 

	object::object() { }

	object::~object() {
		for (auto* c : m_components) {
			c->on_destroy();
			delete c;
		}
		m_components.clear();

		for (auto* o : m_children) {
			o->on_destroy();
			delete o;
		}
		m_children.clear();
	}

	object* object::get_parent() const {
		return m_parent;
	}

	void object::set_parent(object* parent) {
		if (m_parent != parent) {
			if (m_parent) m_parent->__remove_child(this);
			else world::__remove_global(this);

			if (parent) m_parent->m_children.push_back(this);
			else world::__add_global(this);

			m_parent = parent;
		}
	}

	object* object::get_first_ancestor() {
		if (m_parent) return m_parent->get_first_ancestor();
		return this;
	}

	std::string object::get_name() const {
		return m_name;
	}

	void object::set_name(const std::string& name) {
		m_name = name;
	}

	object_tag object::get_tag() const {
		return m_tag;
	}

	void object::set_tag(object_tag tag) {
		m_tag = tag;
	}

	bool object::destroy() {
		return world::destroy(this);
	}

	void object::__set_parent(object* parent) { 
		m_parent = parent;
	}

	void object::__remove_child(object* o) {
		for (auto it = m_children.begin(); it != m_children.end(); it++) {
			if ((*it) == o) {
				m_children.erase(it);
				return;
			}
		}
	}

	void object::__remove_component(component* c) {
		for (auto it = m_components.begin(); it != m_components.end(); it++) {
			if ((*it) == c) {
				m_components.erase(it);
				return;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////// 

	bool world::destroy(object* o) {
		#ifdef _DEBUG
		for (size_t i = 0; i < s_objectsToDelete.size(); i++) {
			if (s_objectsToDelete[i] == o) {
				ALC_DEBUG_ERROR("Cannot delete object since it has already been marked deleted");
				return false;
			}
		}
		#endif
		s_objectsToDelete.push_back(o);
		return true;
	}

	bool world::destroy(component* c) {
		#ifdef _DEBUG
		for (size_t i = 0; i < s_objectsToDelete.size(); i++) {
			if (s_componentsToDelete[i] == c) {
				ALC_DEBUG_ERROR("Cannot delete component since it has already been marked deleted");
				return false;
			}
		}
		#endif
		s_componentsToDelete.push_back(c);
		return true;
	}

	size_t world::globals_size() {
		return s_globalObjects.size();
	}

	void world::__init() {
		s_globalObjects.clear();
		s_objectsToDelete.clear();
		s_componentsToDelete.clear();
	}

	void world::__exit() {
		s_objectsToDelete.clear();
		s_componentsToDelete.clear();

		for (auto* o : s_globalObjects) {
			o->on_destroy();
			delete o;
		}
		s_globalObjects.clear();
	}

	void world::__update() {
		for (size_t i = 0; i < s_objectsToDelete.size(); i++) {
			// call on destroy
			s_objectsToDelete[i]->on_destroy();

			// remove
			if (s_objectsToDelete[i]->get_parent())
				s_objectsToDelete[i]->get_parent()->__remove_child(s_objectsToDelete[i]);
			else
				__remove_global(s_objectsToDelete[i]);

			// delete
			delete s_objectsToDelete[i];
		}
		s_objectsToDelete.clear();

		for (size_t i = 0; i < s_componentsToDelete.size(); i++) {
			// call on destroy
			s_componentsToDelete[i]->on_destroy();

			// remove
			s_componentsToDelete[i]->get_object()->__remove_component(s_componentsToDelete[i]);

			// delete
			delete s_objectsToDelete[i];
		}
		s_componentsToDelete.clear();

	}

	void world::__add_global(object* obj) {
		s_globalObjects.push_back(obj);
	}

	void world::__remove_global(object* obj) {
		for (auto it = s_globalObjects.begin(); it != s_globalObjects.end(); it++) {
			if ((*it) == obj) {
				s_globalObjects.erase(it);
				return;
			}
		}
		ALC_DEBUG_ERROR("Could not reparent correctly, object was thought to be global but was not");
	}

}