#include "gameobject.hpp"
#include "world.hpp"
#include <glm\gtc\matrix_transform.hpp>

namespace alc {

	component::component() : m_object(nullptr), m_shouldUpdate(false) { }

	component::~component() {
		if (m_shouldUpdate) {
			auto func = make_function<&component::on_update>(this);
			detail::onWorldComponentUpdate -= func;
		}
	}

	gameobject* component::get_object() const {
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
				detail::onWorldComponentUpdate += func;
			else
				detail::onWorldComponentUpdate -= func;
		}
	}

	bool component::delete_this() {
		return world::__delete_component(this);
	}

	void component::__set_object(gameobject* object) {
		m_object = object;
	}

	////////////////////////////////////////////////////

	gameobject::gameobject(const std::string& name)
		: m_parent(nullptr), m_group(nullptr), m_name(name)
		, m_position(0.0f), m_rotation(), m_scale(1.0f)
		, m_transform(1.0f), m_transformIsDirty(true) { }

	gameobject::~gameobject() {
		// delete all subobjects
		for (auto* o : m_subobjects) {
			delete o;
		}
		m_subobjects.clear();

		// delete all components
		for (auto* c : m_components) {
			c->on_delete();
			delete c;
		}
		m_components.clear();
	}

	gameobject* gameobject::get_parent() const {
		return m_parent;
	}

	void gameobject::set_parent(gameobject* parent) {
		if (parent == m_parent) return;

		// remove old parent
		if (m_parent) m_parent->__remove_subobject(this);
		// remove from group
		else m_group->__remove_object(this);

		// set new parent
		m_parent = parent;

		// add to new parent list
		if (m_parent) m_parent->m_subobjects.push_back(this);
		// add to group list
		else m_group->__add_object(this);
	}

	group* gameobject::get_group() const {
		return m_group;
	}

	std::string gameobject::get_name() const {
		return m_name;
	}

	void gameobject::set_name(const std::string& name) {
		m_name = name;
	}

	glm::vec3 gameobject::get_position() const {
		return m_position;
	}

	void gameobject::set_position(const glm::vec3& position) {
		m_position = position;
		m_transformIsDirty = true;
	}

	glm::quat gameobject::get_rotation() const {
		return m_rotation;
	}

	void gameobject::set_rotation(const glm::quat& rotation) {
		m_rotation = rotation;
		m_transformIsDirty = true;
	}

	glm::vec3 gameobject::get_scale() const {
		return m_scale;
	}

	void gameobject::set_scale(const glm::vec3& scale) {
		m_scale = scale;
		m_transformIsDirty = true;
	}

	glm::mat4 gameobject::get_transform() const {
		if (m_transformIsDirty) {
			m_transformIsDirty = false;

			m_transform = glm::mat4(1.0f);
			m_transform = glm::scale(m_transform, m_scale);
			m_transform = m_transform * glm::mat4_cast(m_rotation);
			m_transform = glm::translate(m_transform, get_position());
		}
		return m_transform;
	}

	gameobject* gameobject::create_object(const std::string& name) {
		gameobject* object = m_group->create_object(name);
		object->set_parent(this);
		return object;
	}

	bool gameobject::delete_object(gameobject* object) {
		return object->delete_this();
	}

	bool gameobject::delete_this() {
		return world::__delete_object(this);
	}

	bool gameobject::delete_component(component* c) {
		return c->delete_this();
	}

	void gameobject::__set_group(group* g) {
		m_group = g;
	}

	void gameobject::__remove_subobject(gameobject* o) {
		for (size_t i = 0; i < m_subobjects.size(); i++) {
			if (m_subobjects[i] == o) {
				m_subobjects.erase(m_subobjects.begin() + i);
				return;
			}
		}
	}

	void gameobject::__delete_component(component* c) { 
		// remove from list
		for (size_t i = 0; i < m_components.size(); i++) {
			if (m_components[i] == c) {
				c->on_delete();
				delete c;
				m_components.erase(m_components.begin() + i);
				return;
			}
		}
	}

}