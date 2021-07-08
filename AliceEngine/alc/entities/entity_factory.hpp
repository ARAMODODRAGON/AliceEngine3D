#ifndef ALC_ENTITIES_ENTITY_FACTORY_HPP
#define ALC_ENTITIES_ENTITY_FACTORY_HPP
#include "../common.hpp"
#include "../datatypes/hash.hpp"
#include "../core/alice_events.hpp"

namespace alc {

	struct component;
	class behavior;
	class entity;
	class entity_factory;

	// components hold data in an entity
	struct component {

		virtual ~component() = 0 { }

		// returns the entity that this is attached to
		entity* get_entity() const;

	protected:

		// creation event
		virtual void on_create() { }

		// destruction event
		virtual void on_destroy() { }

	private:
		friend entity;
		entity* m_entity;
		void __set_entity(entity* _entity);
	};

	// behaviors contain logic and data in an entity
	class behavior {
	public:

		virtual ~behavior() = 0 { }

		// returns the entity that this is attached to
		entity* get_entity() const;

		// creates a new entity
		entity* create() const;

		// creates an entity with the component of type Ty and returns it
		template<typename Ty> Ty* create();

		// marks an entity for destruction
		// if destroyChildren is false then the children become unparented
		bool destroy(entity* entity_, bool destroyChildren = true);

		// adds a component of type Ty
		template<typename Ty> Ty* add();

		// returns a component of type Ty
		template<typename Ty> Ty* get();

		// returns multiple components of type Ty
		template<typename Ty, typename Container> size_t get(Container* container);

		// marks a component for destruction
		// returns false if it was not found, was null, or was not attached to anything
		bool destroy(component* c);

		// marks a behavior for destruction
		// returns false if it was not found, was null, or was not attached to anything
		bool destroy(behavior* b);

		// the position in world space
		glm::vec3 get_position() const;

		// the position in world space
		void set_position(const glm::vec3& position);

		// the position relative to the parent
		glm::vec3 get_relative_position() const;

		// the position relative to the parent
		void set_relative_position(const glm::vec3& position);

		// the name of this entity
		std::string get_name() const;

		// the name of this entity
		void set_name(const std::string& name);

		// returns the factory this is attached to
		entity_factory* get_factory() const;

		// the parent of this entity, can be null
		entity* get_parent() const;

		// the parent of this entity, can be null
		void set_parent(entity* parent);

	protected:

		// creation event
		virtual void on_create() { }

		// destruction event
		virtual void on_destroy() { }

		// step event
		virtual void on_update(timestep ts) { }

	private:
		friend entity;
		entity* m_entity;
		void __set_entity(entity* _entity);
	};

	// object that holds components and behaviors
	class entity final {
	public:

		entity();
		entity(const std::string& name);
		~entity();

		// creates a new entity
		entity* create() const;

		// creates an entity with the component of type Ty and returns it
		template<typename Ty> Ty* create();

		// marks an entity for destruction
		// if destroyChildren is false then the children become unparented
		bool destroy(entity* entity_, bool destroyChildren = true);

		// adds a component of type Ty
		template<typename Ty> Ty* add();

		// returns a component of type Ty
		template<typename Ty> Ty* get();

		// returns multiple components of type Ty
		template<typename Ty, typename Container> size_t get(Container* container);

		// marks a component for destruction
		// returns false if it was not found, was null, or was not attached to anything
		bool destroy(component* c);

		// marks a behavior for destruction
		// returns false if it was not found, was null, or was not attached to anything
		bool destroy(behavior* b);

		// the position in world space
		glm::vec3 get_position() const;

		// the position in world space
		void set_position(const glm::vec3& position);

		// the position relative to the parent
		glm::vec3 get_relative_position() const;

		// the position relative to the parent
		void set_relative_position(const glm::vec3& position);

		// the name of this entity
		std::string get_name() const;

		// the name of this entity
		void set_name(const std::string& name);

		// returns the factory this is attached to
		entity_factory* get_factory() const;

		// the parent of this entity, can be null
		entity* get_parent() const;

		// the parent of this entity, can be null
		void set_parent(entity* parent);

	private:

		entity_factory* m_factory;

		std::vector<std::pair<component*, bool>> m_components;
		std::vector<std::pair<behavior*, bool>> m_behaviors;

		glm::vec3 m_position;
		std::string m_name;
		hash32_t m_nameHash;

		entity* m_parent;
		std::vector<entity*> m_children;

		bool m_isUpdating;
		void __on_update(timestep ts);

	public:
		void __set_factory(entity_factory* factory);
	};

	// a list of entities
	// listens to alice_events::onUpdate to update the entities
	class entity_factory final {
		ALC_STATIC_CLASS(entity_factory);
	public:

		entity_factory(size_t reserve = 0);
		~entity_factory();

		// creates a new entity
		entity* create() const;

		// creates an entity with the component of type Ty and returns it
		template<typename Ty> Ty* create();

		// marks an entity for destruction
		// if destroyChildren is false then the children become unparented
		bool destroy(entity* entity_, bool destroyChildren = true);

	private:
		std::vector<std::pair<entity*, int8>> m_entities;
		void __on_update(timestep ts);
	};


	// implementations for templates

	template<typename Ty>
	inline Ty* behavior::create() {
		return get_entity()->create<Ty>();
	}

	template<typename Ty>
	inline Ty* behavior::add() {
		return get_entity()->add<Ty>();
	}

	template<typename Ty>
	inline Ty* behavior::get() {
		return get_entity()->get<Ty>();
	}

	template<typename Ty, typename Container>
	inline size_t behavior::get(Container* container) {
		return get_entity()->get<Ty>(container);
	}

	template<typename Ty>
	inline Ty* entity::create() {
		return get_factory()->create<Ty>();
	}

	template<typename Ty>
	inline Ty* entity::add() {
		// add behavior
		if constexpr (std::is_base_of_v<behavior, Ty>) {
			if (!m_isUpdating) {
				alice_events::onUpdate += make_function<&entity::__on_update>(this);
				m_isUpdating = true;
			}
			behavior* b = new Ty();
			m_behaviors.emplace_back(b, false);
			b->__set_entity(this);
			b->on_create();
		}
		// add component
		if constexpr (std::is_base_of_v<component, Ty>) {
			component* c = new Ty();
			m_components.emplace_back(c, false);
			c->__set_entity(this);
			c->on_create();
		}

		// did not create any components, return nullptr
		return nullptr;
	}

	template<typename Ty>
	inline Ty* entity::get() {
		// behaviors
		if constexpr (std::is_base_of_v<behavior, Ty>) {
			for (size_t i = 0; i < m_behaviors.size(); i++) {
				if (Ty* b = dynamic_cast<Ty*>(m_behaviors[i].first); b != nullptr) {
					return b;
				}
			}
		}

		// components
		if constexpr (std::is_base_of_v<component, Ty>) {
			for (size_t i = 0; i < m_components.size(); i++) {
				if (Ty* c = dynamic_cast<Ty*>(m_components[i].first); c != nullptr) {
					return c;
				}
			}
		}

		// nothing was found, return nullptr
		return nullptr;
	}

	template<typename Ty, typename Container>
	inline size_t entity::get(Container* container) {
		size_t count = 0;

		// behaviors
		if constexpr (std::is_base_of_v<behavior, Ty>) {
			for (size_t i = 0; i < m_behaviors.size(); i++) {
				if (Ty* b = dynamic_cast<Ty*>(m_behaviors[i].first); b != nullptr) {
					container->push_back(b);
					count++;
				}
			}
		}

		// components
		if constexpr (std::is_base_of_v<component, Ty>) {
			for (size_t i = 0; i < m_components.size(); i++) {
				if (Ty* c = dynamic_cast<Ty*>(m_components[i].first); c != nullptr) {
					container->push_back(c);
					count++;
				}
			}
		}

		// return count
		return count;
	}

	template<typename Ty>
	inline Ty* entity_factory::create() {
		if constexpr (std::is_base_of_v<behavior, Ty> || std::is_base_of_v<component, Ty>) {
			entity* e = create();
			return e->add<Ty>();
		}
		return nullptr;
	}

}

#endif // !ALC_ENTITIES_ENTITY_FACTORY_HPP