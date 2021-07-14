#ifndef ALC_OBJECTS_OBJECT_FACTORY_HPP
#define ALC_OBJECTS_OBJECT_FACTORY_HPP
#include "../common.hpp"
#include "../datatypes/hash.hpp"
#include "../core/alice_events.hpp"

namespace alc {

	struct component;
	class behavior;
	class game_object;
	class object_factory;
	class object_prefab;

	// components hold data in an game_object
	struct component {
		ALC_NO_COPY(component);
		ALC_NO_MOVE(component);

		component();
		virtual ~component() = 0;

		// returns the game_object that this is attached to
		game_object* get_object() const;

	protected:

		// creation event
		virtual void on_create() { }

		// destruction event
		virtual void on_destroy() { }

	private:
		friend game_object;
		game_object* m_gameobject;
		void __set_object(game_object* _go);
	};

	// behaviors contain logic and data in an game_object
	class behavior {
		ALC_NO_COPY(behavior);
		ALC_NO_MOVE(behavior);
	public:

		behavior();
		virtual ~behavior() = 0;

		// returns the game_object that this is attached to
		game_object* get_object() const;

		// creates a new game_object
		game_object* create() const;

		// creates an game_object with the component of type Ty and returns it
		template<typename Ty> Ty* create();

		// marks an game_object for destruction
		// if destroyChildren is false then the children become unparented
		bool destroy(game_object* entity_, bool destroyChildren = true);

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

		// the name of this game_object
		std::string get_name() const;

		// the name of this game_object
		void set_name(const std::string& name);

		// returns the factory this is attached to
		object_factory* get_factory() const;

		// the parent of this game_object, can be null
		game_object* get_parent() const;

		// the parent of this game_object, can be null
		void set_parent(game_object* parent);

	protected:

		// creation event
		virtual void on_create() { }

		// destruction event
		virtual void on_destroy() { }

		// step event
		virtual void on_update(timestep ts) { }

	private:
		friend game_object;
		game_object* m_gameobject;
		void __set_object(game_object* _go);
	};

	// object that holds components and behaviors
	class game_object final {
		ALC_NO_COPY(game_object);
	public:

		// used to iterate over this objects children
		using iterator = std::vector<game_object*>::iterator;

		game_object();
		game_object(const std::string& name);
		~game_object();

		// creates a new game_object
		game_object* create() const;

		// creates an game_object with the component of type Ty and returns it
		template<typename Ty> Ty* create();

		// marks an game_object for destruction
		// if destroyChildren is false then the children become unparented
		bool destroy(game_object* go_, bool destroyChildren = true);

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

		// the name of this game_object
		std::string get_name() const;

		// the name of this game_object
		void set_name(const std::string& name);

		// returns the factory this is attached to
		object_factory* get_factory() const;

		// the parent of this game_object, can be null
		game_object* get_parent() const;

		// the parent of this game_object, can be null
		void set_parent(game_object* parent);

		// returns an iterator to iterate over the child objects
		iterator begin();

		// returns an iterator to iterate over the child objects
		iterator end();

	private:

		object_factory* m_factory;

		std::vector<std::pair<component*, bool>> m_components;
		std::vector<std::pair<behavior*, bool>> m_behaviors;

		glm::vec3 m_position;
		std::string m_name;
		hash32_t m_nameHash;

		game_object* m_parent;
		std::vector<game_object*> m_children;
		void __unparent();
		void __remove_child(game_object* go);
		void __add_child(game_object* go);

		bool m_shouldUpdate : 1;
		uint32 m_componentsToDestroy;
		uint32 m_behaviorsToDestroy;

	public:
		bool __should_update();
		void __on_update(timestep ts);
		void __set_factory(object_factory* factory);
	};

	// a list of game_objects
	// listens to alice_events::onUpdate to update the game_objects
	class object_factory final {
		ALC_STATIC_CLASS(object_factory);
	public:

		object_factory(size_t reserve = 0);
		~object_factory();

		// creates a new game_object
		game_object* create();

		// creates an game_object with the component of type Ty and returns it
		template<typename Ty> Ty* create();

		// creates multiple game objects to fill every index 
		template<typename iter>
		void create(iter begin_, iter end_);
		
		// creates multiple game objects with a component of type Ty to fill every index 
		template<typename Ty, typename iter>
		void create(iter begin_, iter end_);

		// marks an game_object for destruction
		// if destroyChildren is false then the children become unparented
		bool destroy(game_object* go_, bool destroyChildren = true);

		// returns the number of objects in the factory
		size_t size() const;

		// returns the object at the index
		game_object* operator[](size_t index) const;

		// used to iterate over all the objects in the factory
		struct iterator {

			iterator();

			iterator& operator++();

			iterator operator++(int);

			iterator& operator--();

			iterator operator--(int);

			game_object* operator*();

			game_object& operator->();

		private:
			size_t m_pos;
			std::vector<game_object*>* m_objects;
		public:
			iterator(size_t pos, std::vector<game_object*>* objects);
		};

		iterator begin();

		iterator end();

	private:
		std::vector<game_object*> m_objects;
		std::vector<game_object*> m_objectsToDestroy;
		void __on_update(timestep ts);
	};


	// implementations for templates //////////////////////////////////////

	template<typename Ty>
	inline Ty* behavior::create() {
		return get_object()->create<Ty>();
	}

	template<typename Ty>
	inline Ty* behavior::add() {
		return get_object()->add<Ty>();
	}

	template<typename Ty>
	inline Ty* behavior::get() {
		return get_object()->get<Ty>();
	}

	template<typename Ty, typename Container>
	inline size_t behavior::get(Container* container) {
		return get_object()->get<Ty>(container);
	}

	template<typename Ty>
	inline Ty* game_object::create() {
		return get_factory()->create<Ty>();
	}

	template<typename Ty>
	inline Ty* game_object::add() {
		// add behavior
		if constexpr (std::is_base_of_v<behavior, Ty>) {
			m_shouldUpdate = true;
			behavior* b = new Ty();
			m_behaviors.emplace_back(b, false);
			b->__set_object(this);
			b->on_create();
		}
		// add component
		if constexpr (std::is_base_of_v<component, Ty>) {
			component* c = new Ty();
			m_components.emplace_back(c, false);
			c->__set_object(this);
			c->on_create();
		}

		// did not create any components, return nullptr
		return nullptr;
	}

	template<typename Ty>
	inline Ty* game_object::get() {
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
	inline size_t game_object::get(Container* container) {
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
	inline Ty* object_factory::create() {
		if constexpr (std::is_base_of_v<behavior, Ty> || std::is_base_of_v<component, Ty>) {
			game_object* go = create();
			return go->add<Ty>();
		}
		return nullptr;
	}

	template<typename iter>
	inline void object_factory::create(iter begin_, iter end_) {
		size_t sz = std::distance(begin_, end_);

		// make sure theres enough extra memory
		m_objects.reserve(m_objects.size() + sz);

		// foreach; make a gameobject
		while (begin_ != end_) {
			(*begin_) = create();
			++begin_;
		}
	}

	template<typename Ty, typename iter>
	inline void object_factory::create(iter begin_, iter end_) {
		size_t sz = std::distance(begin_, end_);

		// make sure theres enough extra memory
		m_objects.reserve(m_objects.size() + sz);

		// foreach; make a gameobject
		while (begin_ != end_) {
			(*begin_) = create<Ty>();
			++begin_;
		}
	}

}

#endif // !ALC_OBJECTS_OBJECT_FACTORY_HPP