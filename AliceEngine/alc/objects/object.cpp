#include "object.hpp"

namespace alc {

	object::object() : m_shouldUpdate(false), m_parent(nullptr), m_name(""), m_tag((object_tag)0) { }

	object::~object() {
		for (auto* o : m_children) {
			o->on_destroy();
			delete o;
		}
		m_children.clear();

		// remove
		if (m_shouldUpdate) alice_events::onUpdate -= make_function<&object::on_update>(this);
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

			onParentChanged(m_parent, parent);
			m_parent = parent;
		}
	}

	object* object::get_first_ancestor() {
		if (m_parent) return m_parent->get_first_ancestor();
		return this;
	}

	bool object::get_update() const {
		return m_shouldUpdate;
	}

	void object::set_update(bool set) {
		if (set != m_shouldUpdate) {
			// enabled
			if (set)
				alice_events::onUpdate += make_function<&object::on_update>(this);
			// disable
			else
				alice_events::onUpdate -= make_function<&object::on_update>(this);
			m_shouldUpdate = set;
		}
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

	bool object::close_script() {
		bool isThereAScript = m_script.get() != nullptr;
		m_script.reset();
		return isThereAScript;
	}

	void object::message(const std::string& function, uint32 recursiveDepth) {
		// send message this script
		if (m_script.get())
			m_script->message(this, function);

		// check if recursive and send to all children
		if (recursiveDepth--) {
			for (auto o : m_children) {
				o->message(function, recursiveDepth);
			}
		}
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

	size_t world::globals_size() {
		return s_globalObjects.size();
	}

	void world::__init() {
		s_globalObjects.clear();
		s_objectsToDelete.clear();
	}

	void world::__exit() {
		s_objectsToDelete.clear();

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