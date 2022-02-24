#include "world.hpp"
#include "../core/scene_manager.hpp"

namespace alc {

	component::component() : m_object(nullptr), m_shouldUpdate(false) { }

	component::~component() {
		if (m_shouldUpdate) {
			auto func = make_function<&component::on_update>(this);
			world::onWorldUpdate -= func;
		}
	}

	object* component::get_object() const {
		return m_object;
	}

	bool component::get_should_update() const {
		return m_shouldUpdate;
	}

	void component::set_should_update(bool shouldUpdate) {
		if (m_shouldUpdate != shouldUpdate) {
			m_shouldUpdate = shouldUpdate;

			auto func = make_function<&component::on_update>(this);

			if (shouldUpdate)
				world::onWorldUpdate += func;
			else
				world::onWorldUpdate -= func;
		}
	}

	bool component::destroy_this() {
		return world::__delete_component(this);
	}

	object* component::get_parent() const {
		return m_object->get_parent();
	}

	void component::set_parent(object* parent) { }

	std::string component::get_name() const {
		return m_object->get_name();
	}

	void component::set_name(const std::string& name) {
		m_object->set_name(name);
	}

	object* component::create_object(const std::string& name) {
		return m_object->create_object();
	}

	bool component::destroy_object(object* object) {
		return m_object->destroy_object(object);
	}

	bool component::delete_component(component* c) {
		return m_object->delete_component(c);
	}

	void component::__set_object(object* object) {
		m_object = object;
	}

	////////////////////////////////////////////////////

	object::object(const std::string& name)
		: m_parent(nullptr), m_name(name), m_shouldUpdate(false) { }

	object::~object() {
		// delete all subobjects
		for (auto* o : m_subobjects) {
			delete o;
		}
		m_subobjects.clear();

		// delete all components
		for (auto* c : m_components) {
			c->on_destroy();
			delete c;
		}
		m_components.clear();

		if (m_shouldUpdate) {
			auto func = make_function<&object::on_update>(this);
			world::onWorldUpdate -= func;
		}
	}

	object* object::get_parent() const {
		return m_parent;
	}

	void object::set_parent(object* parent) {
		if (parent == m_parent) return;

		// remove old parent
		if (m_parent) m_parent->__remove_subobject(this);

		// set new parent
		m_parent = parent;

		// add to new parent list
		if (m_parent) m_parent->m_subobjects.push_back(this);
	}

	std::string object::get_name() const {
		return m_name;
	}

	void object::set_name(const std::string& name) {
		m_name = name;
	}

	// returns whether this component should actively update or not
	bool object::get_should_update() const {
		return m_shouldUpdate;
	}

	// sets if this component should actively update or not
	void object::set_should_update(bool shouldUpdate) {
		if (m_shouldUpdate != shouldUpdate) {
			m_shouldUpdate = shouldUpdate;

			auto func = make_function<&object::on_update>(this);

			if (shouldUpdate)
				world::onWorldUpdate += func;
			else
				world::onWorldUpdate -= func;
		}
	}

	scene* object::get_scene() {
		if (!m_parent) return nullptr;
		if (scene* s = dynamic_cast<scene*>(m_parent)) return s;
		return m_parent->get_scene();
	}

	object* object::create_object(const std::string& name) {
		object* object = world::create(name);
		object->set_parent(this);
		return object;
	}

	bool object::destroy_object(object* object) {
		return object->destroy_this();
	}

	bool object::destroy_this() {
		return world::__delete_object(this);
	}

	bool object::delete_component(component* c) {
		return c->destroy_this();
	}

	void object::__remove_subobject(object* o) {
		for (size_t i = 0; i < m_subobjects.size(); i++) {
			if (m_subobjects[i] == o) {
				m_subobjects.erase(m_subobjects.begin() + i);
				return;
			}
		}
	}

	void object::__delete_component(component* c) {
		// remove from list
		for (size_t i = 0; i < m_components.size(); i++) {
			if (m_components[i] == c) {
				c->on_destroy();
				delete c;
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
	}

}