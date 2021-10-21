#include "object_factory.hpp"
#include "../core/debug.hpp"

namespace alc {

	// component ///////////////////////////////////////////////////

	component::component() : m_gameobject(nullptr) { }

	component::~component() { m_gameobject = nullptr; }

	game_object* component::get_object() const { return m_gameobject; }

	void component::__set_object(game_object* _go) { m_gameobject = _go; }

	// behavior ////////////////////////////////////////////////////

	behavior::behavior() : component() { }

	behavior::~behavior() { }

	game_object* behavior::create() const {
		return get_object()->create();
	}

	bool behavior::destroy(game_object* go_, bool destroyChildren) {
		return get_object()->destroy(go_, destroyChildren);
	}

	component* behavior::add(type ty) {
		return get_object()->add(ty);
	}

	bool behavior::destroy(component* c) {
		return get_object()->destroy(c);
	}

	bool behavior::destroy(behavior* b) {
		return get_object()->destroy(b);
	}

	glm::vec3 behavior::get_position() const {
		return get_object()->get_position();
	}

	void behavior::set_position(const glm::vec3& position) {
		get_object()->set_position(position);
	}

	glm::vec3 behavior::get_relative_position() const {
		return get_object()->get_relative_position();
	}

	void behavior::set_relative_position(const glm::vec3& position) {
		get_object()->set_relative_position(position);
	}

	std::string behavior::get_name() const {
		return get_object()->get_name();
	}

	void behavior::set_name(const std::string& name) {
		get_object()->set_name(name);
	}

	object_factory* behavior::get_factory() const {
		return get_object()->get_factory();
	}

	game_object* behavior::get_parent() const {
		return get_object()->get_parent();
	}

	void behavior::set_parent(game_object* parent) {
		get_object()->set_parent(parent);
	}

	// game_object /////////////////////////////////////////////////

	game_object::game_object() : game_object("game_object") { }

	game_object::game_object(const std::string& name)
		: m_factory(nullptr), m_position(0.0f), m_name(name)
		, m_parent(nullptr), m_shouldUpdate(false), m_componentsToDestroy(0), m_behaviorsToDestroy(0) { }

	game_object::~game_object() {
		// null the factory and parent
		m_factory = nullptr;
		if (m_parent) m_parent->__remove_child(this);
		m_parent = nullptr;

		// unparent all children
		for (game_object* child : m_children) {
			child->__unparent();
		}
		m_children.clear();

		// destroy all components
		for (auto [c, _0] : m_components) {
			c->on_destroy();
			delete c;
		}
		m_components.clear();

		// destroy all behaviors
		for (auto [b, _0] : m_behaviors) {
			b->on_destroy();
			delete b;
		}
		m_behaviors.clear();

	}

	game_object* game_object::create() const {
		return m_factory->create();
	}

	bool game_object::destroy(game_object* go_, bool destroyChildren) {
		return m_factory->destroy(go_, destroyChildren);
	}

	component* game_object::add(type ty) {
		if (!ty || ty.parents_size() == 0) {
			if (ty) ALC_DEBUG_WARNING("Could not instance component of type " + ty.get_name());
			else ALC_DEBUG_WARNING("Could not instance component of type ");
			return nullptr;
		}

		static type _component = type::get<component>();
		static type _behavior = type::get<behavior>();

		for (size_t i = 0; i < ty.parents_size(); i++) {
			// new behavior
			if (ty.parent_get(i) == _behavior) {
				behavior* b = ty.instance<behavior*>();
				b->on_create();
				m_behaviors.emplace_back(b, false);
			}
			// new component
			else if (ty.parent_get(i) == _component) {
				component* c = ty.instance<component*>();
				c->on_create();
				m_components.emplace_back(c, false);
			}
		}

		return nullptr;
	}

	bool game_object::destroy(component* c) {
		if (c == nullptr) {
			ALC_DEBUG_WARNING("Could not delete component because it was nullptr");
			return false;
		}
		if (c->get_object() == nullptr) {
			ALC_DEBUG_WARNING("Could not delete component because it was not attached to anything");
			return false;
		}
		if (c->get_object() != this) return c->get_object()->destroy(c);

		for (auto& pair : m_components) {
			if (pair.first == c) {
				++m_componentsToDestroy;
				m_shouldUpdate = true;
				pair.second = true;
				return true;
			}
		}

		ALC_DEBUG_WARNING("Could not delete component because it was not found");
		return false;
	}

	bool game_object::destroy(behavior* b) {
		if (b == nullptr) {
			ALC_DEBUG_WARNING("Could not delete behavior because it was nullptr");
			return false;
		}
		if (b->get_object() == nullptr) {
			ALC_DEBUG_WARNING("Could not delete behavior because it was not attached to anything");
			return false;
		}
		if (b->get_object() != this) return b->get_object()->destroy(b);

		for (auto& pair : m_behaviors) {
			if (pair.first == b) {
				++m_behaviorsToDestroy;
				pair.second = true;
				return true;
			}
		}

		ALC_DEBUG_WARNING("Could not delete behavior because it was not found");
		return false;
	}

	glm::vec3 game_object::get_position() const {
		if (m_parent) return m_parent->get_position() + m_position;
		return m_position;
	}

	void game_object::set_position(const glm::vec3& position) {
		m_position = position - m_parent->get_position();
	}

	glm::vec3 game_object::get_relative_position() const {
		return m_position;
	}

	void game_object::set_relative_position(const glm::vec3& position) {
		m_position = position;
	}

	std::string game_object::get_name() const {
		return m_name;
	}

	void game_object::set_name(const std::string& name) {
		m_name = name;
	}

	object_factory* game_object::get_factory() const {
		return m_factory;
	}

	game_object* game_object::get_parent() const {
		return m_parent;
	}

	void game_object::set_parent(game_object* parent) {
		if (m_parent) {
			m_position = m_position + m_parent->get_position();
			m_parent->__remove_child(this);
		}
		m_parent = parent;
		if (m_parent) {
			m_parent->__add_child(this);
			m_position = m_position - m_parent->get_position();
		}
	}

	game_object::iterator game_object::begin() {
		return m_children.begin();
	}

	game_object::iterator game_object::end() {
		return m_children.end();
	}

	void game_object::__unparent() {
		if (m_parent) m_position = m_position + m_parent->get_position();
		m_parent = nullptr;
	}

	void game_object::__remove_child(game_object* go) {
		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			if (*it == go) {
				m_children.erase(it);
				return;
			}
		}
	}

	void game_object::__add_child(game_object* go) {
		m_children.push_back(go);
	}

	bool game_object::__should_update() {
		return m_shouldUpdate;
	}

	void game_object::__on_update(timestep ts) {

		for (auto [b, _0] : m_behaviors) {
			b->on_update(ts);
		}

		if (m_componentsToDestroy > 0 || m_behaviorsToDestroy > 0) {

			// checks for components to destroy and repeats until all are deleted
			while (m_componentsToDestroy > 0) {
				for (size_t i = 0; i < m_components.size(); ++i) {
					if (m_components[i].second) {
						m_components[i].first->on_destroy();
						delete m_components[i].first;
						m_components.erase(m_components.begin() + i);
						--i;
						--m_componentsToDestroy;
						if (m_componentsToDestroy == 0) break;
					}
				}
			}

			// checks for behaviors to destroy and repeats until all are deleted
			while (m_behaviorsToDestroy > 0) {
				for (size_t i = 0; i < m_behaviors.size(); ++i) {
					if (m_behaviors[i].second) {
						m_behaviors[i].first->on_destroy();
						delete m_behaviors[i].first;
						m_behaviors.erase(m_behaviors.begin() + i);
						--i;
						--m_behaviorsToDestroy;
						if (m_behaviorsToDestroy == 0) break;
					}
				}
			}

			// make sure we dont update next time unless we have to
			if (m_behaviors.size() == 0 && m_componentsToDestroy == 0 && m_behaviorsToDestroy == 0) {
				m_shouldUpdate = false;
			}
		}

	}

	void game_object::__set_factory(object_factory* factory) {
		m_factory = factory;
	}

	// object_factory //////////////////////////////////////////////


	object_factory::object_factory(size_t reserve) {
		// start listening
		alice_events::onUpdate += make_function<&object_factory::__on_update>(this);

		// reserve memory for the factory
		m_objects.reserve(reserve);
	}

	object_factory::~object_factory() {
		// stop listening
		alice_events::onUpdate -= make_function<&object_factory::__on_update>(this);
	}

	game_object* object_factory::create() {
		game_object* object = new game_object();
		m_objects.emplace_back(object);
		object->__set_factory(this);
		return object;
	}

	bool object_factory::destroy(game_object* go_, bool destroyChildren) {
		if (go_ == nullptr) {
			ALC_DEBUG_WARNING("Could not destroy object because it was nullptr");
			return false;
		}

		if (go_->get_factory() != this) {
			ALC_DEBUG_ERROR("Could not destroy object '" + go_->get_name() + "' because it was not attached to a factory");
			return false;
		}

		// check that this object is not already added to the list
		// if it is then return true
		for (size_t i = 0; i < m_objectsToDestroy.size(); i++) {
			if (m_objectsToDestroy[i] == go_) {
				return true;
			}
		}

		bool success = true;

		// add to the list
		m_objectsToDestroy.emplace_back(go_);

		// add all its children
		if (destroyChildren) {
			for (game_object* child : (*go_)) {
				if (!destroy(child, true)) {
					success = false;
				}
			}
		}

		// successfully marked
		return success;
	}

	size_t object_factory::size() const {
		return m_objects.size();
	}

	game_object* object_factory::operator[](size_t index) const {
		return m_objects[index];
	}

	object_factory::iterator object_factory::begin() {
		return iterator(0, &m_objects);
	}

	object_factory::iterator object_factory::end() {
		return iterator(m_objects.size(), &m_objects);
	}

	void object_factory::__on_update(timestep ts) {

		for (size_t i = 0; i < size(); i++) {
			if (m_objects[i]->__should_update()) {
				m_objects[i]->__on_update(ts);
			}
		}

		if (m_objectsToDestroy.size() > 0) {
			for (size_t i = 0; i < m_objectsToDestroy.size(); i++) {
				// find the object and delete it
				for (size_t j = 0; j < m_objects.size(); j++) {
					if (m_objectsToDestroy[i] == m_objects[j]) {
						delete m_objects[j];
						m_objects.erase(m_objects.begin() + j);
						break;
					}
				}
			}
			m_objectsToDestroy.clear();
		}

	}

	// object_factory::iterators ///////////////////////////////////

	object_factory::iterator::iterator() : m_pos(-1), m_objects(nullptr) { }

	object_factory::iterator& object_factory::iterator::operator++() {
		++m_pos;
		return *this;
	}

	object_factory::iterator object_factory::iterator::operator++(int) {
		iterator last = *this;
		++m_pos;
		return last;
	}

	object_factory::iterator& object_factory::iterator::operator--() {
		--m_pos;
		return *this;
	}

	object_factory::iterator object_factory::iterator::operator--(int) {
		iterator last = *this;
		--m_pos;
		return last;
	}

	game_object* object_factory::iterator::operator*() {
		return (*m_objects)[m_pos];
	}

	game_object& object_factory::iterator::operator->() {
		return *((*m_objects)[m_pos]);
	}

	object_factory::iterator::iterator(size_t pos, std::vector<game_object*>* objects)
		: m_pos(pos), m_objects(objects) { }

}