#ifndef ALC_ENTITIES_ENTITY_FACTORY_HPP
#define ALC_ENTITIES_ENTITY_FACTORY_HPP
#include "../common.hpp"

namespace alc {

	// components hold data in an entity
	struct icomponent {
		virtual ~icomponent() = 0 { }
	};

	// behaviors contain logic and data in an entity
	class behavior {
	public:
		virtual ~behavior() = 0 { }

		entity* get_entity() const;

		template<typename T> T* add();
		template<typename T> T* get();
		bool destroy(icomponent* c);
		bool destroy(behavior* b);

	protected:
		virtual void on_create() { }
		virtual void on_destroy() { }
		virtual void on_update(float delta) { }

	private:
		friend class entity;
		entity* m_entity;
	};

	// object that holds components and behaviors
	class entity final {
	public:

		template<typename T> T* add();
		template<typename T> T* get();
		bool destroy(icomponent* c);
		bool destroy(behavior* b);

	private:
		std::vector<icomponent*> m_components;
		std::vector<behavior*> m_behaviors;
		void on_update(float delta);
	};

	// a list of entities
	class entity_factory final {
		ALC_STATIC_CLASS(entity_factory);
	public:



	};

}

#endif // !ALC_ENTITIES_ENTITY_FACTORY_HPP