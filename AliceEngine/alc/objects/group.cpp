#include "group.hpp"
#include "world.hpp"

namespace alc {

	behavior::behavior() : m_group(nullptr), m_shouldUpdate(false) { }

	behavior::~behavior() {
		if (m_shouldUpdate) {
			auto func = make_function<&behavior::on_update>(this);
			detail::onWorldBehaviorUpdate -= func;
		}
		world::__remove_delete_behavior(this);
	}

	group* behavior::get_group() const {
		return m_group;
	}

	bool behavior::get_should_update() const {
		return m_shouldUpdate;
	}

	void behavior::set_should_update(bool shouldUpdate) {
		if (m_shouldUpdate != shouldUpdate) {
			m_shouldUpdate = shouldUpdate;

			auto func = make_function<&behavior::on_update>(this);

			if (shouldUpdate)
				detail::onWorldBehaviorUpdate += func;
			else
				detail::onWorldBehaviorUpdate -= func;
		}
	}

	bool behavior::delete_this() {
		return world::__delete_behavior(this);
	}

	void behavior::__set_group(group* g) {
		m_group = g;
	}

	////////////////////////////////////////////////////

	group::group(const std::string& name)
		: m_parent(nullptr), m_name(name) { }

	group::~group() {
		// delete all subgroups
		for (auto* g : m_subgroups) {
			delete g;
		}
		m_subgroups.clear();

		// delete all objects
		for (auto* o : m_objects) {
			delete o;
		}
		m_objects.clear();

		// delete all behaviors
		for (auto* b : m_behaviors) {
			b->on_delete();
			delete b;
		}
		m_behaviors.clear();

		world::__remove_delete_group(this);
	}

	group* group::get_parent() const {
		return m_parent;
	}

	void group::set_parent(group* g) {
		if (g == get_parent()) return;

		// remove from old parent
		if (m_parent) m_parent->__remove_subgroup(this);
		// remove from world
		else world::__remove_group(this);

		// set new parent
		m_parent = g;

		// add to parents list
		if (m_parent) m_parent->m_subgroups.push_back(this);
		// add to world list
		else world::__add_group(this);
	}

	std::string group::get_name() const {
		return m_name;
	}

	void group::set_name(const std::string& name) {
		m_name = name;
	}

	group* group::create_group(const std::string& name) {
		group* g = world::create_group(name);
		g->set_parent(this);
		return g;
	}

	bool group::delete_group(group* g) {
		if (g == nullptr) return false;
		return g->delete_this();
	}

	bool group::delete_this() {
		return world::delete_group(this);
	}

	size_t group::subgroup_size() const {
		return m_subgroups.size();
	}

	group* group::get_subgroup(size_t index) {
		return m_subgroups[index];
	}

	bool group::is_primary_group() const {
		return world::primary() == this;
	}

	bool group::delete_behavior(behavior* b) {
		return world::__delete_behavior(b);
	}

	gameobject* group::create_object(const std::string& name) {
		gameobject* object = new gameobject(name);
		m_objects.push_back(object);
		object->__set_group(this);
		return object;
	}

	bool group::delete_object(gameobject* object) {
		return object->delete_this();
	}

	gameobject* group::get_object(const std::string& name) {
		for (auto* go : m_objects) {
			if (go->get_name() == name) {
				return go;
			}
		}
		return nullptr;
	}

	void group::__remove_subgroup(group* g) {
		for (size_t i = 0; i < m_subgroups.size(); i++) {
			if (g == m_subgroups[i]) {
				m_subgroups.erase(m_subgroups.begin() + i);
				g->m_parent = nullptr;
				return;
			}
		}
	}

	void group::__delete_object(gameobject* object) {
		// remove from list
		for (size_t i = 0; i < m_objects.size(); i++) {
			if (m_objects[i] == object) {
				delete object;
				m_objects.erase(m_objects.begin() + i);
				return;
			}
		}
	}

	void group::__delete_group(group* g) {
		// remove from list
		for (size_t i = 0; i < m_subgroups.size(); i++) {
			if (m_subgroups[i] == g) {
				delete g;
				m_subgroups.erase(m_subgroups.begin() + i);
				return;
			}
		}
	}

	void group::__remove_object(gameobject* object) {
		for (size_t i = 0; i < m_objects.size(); i++) {
			if (m_objects[i] == object) {
				m_objects.erase(m_objects.begin() + i);
				return;
			}
		}
	}

	void group::__add_object(gameobject* object) {
		m_objects.push_back(object);
	}

}