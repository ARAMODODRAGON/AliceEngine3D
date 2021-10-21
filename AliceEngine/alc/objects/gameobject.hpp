#ifndef ALC_OBJECTS_GAMEOBJECT_HPP
#define ALC_OBJECTS_GAMEOBJECT_HPP
#include "../common.hpp"
#include "../datatypes/timestep.hpp"
#include "component_info.hpp"

namespace alc {

	class group;
	class gameobject;
	class component;

	// add custom functionality to gameobjects
	class component {
	public:

		component();
		virtual ~component() = 0;

		// virtual functions

		virtual void on_create() { }
		virtual void on_delete() { }
		virtual void on_update(timestep ts) { }

		// returns the attached object
		gameobject* get_object() const;

		// returns whether this component should actively update or not
		bool get_should_update() const;

		// sets if this component should actively update or not
		void set_should_update(bool shouldUpdate);

		// deletes this component
		bool delete_this();

	private:
		gameobject* m_object;
		bool m_shouldUpdate : 1;
	public:
		void __set_object(gameobject* object);
	};

	// objects that exists in the world with a position, rotation, scale and attached logic
	class gameobject final {
	public:

		gameobject(const std::string& name = "");
		~gameobject();

		// returns the parent gameobject
		gameobject* get_parent() const;

		// reparents this object, can be set to nullptr to unparent
		void set_parent(gameobject* parent);

		// returns the group this object is attached to
		// gameobjects cannot change groups
		group* get_group() const;

		// returns the name of this object
		std::string get_name() const;

		// sets the name of this object
		void set_name(const std::string& name);

		// returns the position
		glm::vec3 get_position() const;

		// sets the position
		void set_position(const glm::vec3& position);

		// returns the rotation
		glm::quat get_rotation() const;

		// sets the rotation
		void set_rotation(const glm::quat& rotation);

		// returns the scale
		glm::vec3 get_scale() const;

		// sets the scale
		void set_scale(const glm::vec3& scale);

		// returns the transformation matrix
		glm::mat4 get_transform() const;


		//// gameobjects

		// creates an object attached to this object
		gameobject* create_object(const std::string& name = "");

		// creates an object attached to this object with component of type
		template<class T> T* create_object(const std::string& name = "");

		// deletes the object
		bool delete_object(gameobject* object);

		// deletes this object
		bool delete_this();


		//// components

		// creates a component of type attached to this gameobject
		template<class T> T* create_component();

		// deletes a component
		bool delete_component(component* c);

		// gets a component of type attached to this gameobject
		template<class T> T* get_component();

		// gets multiple components of type attached to this gameobject
		template<class T> size_t get_components(std::vector<T*>& outComponents);

	private:

		gameobject* m_parent;
		group* m_group;

		std::string m_name;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;

		mutable glm::mat4 m_transform;
		mutable bool m_transformIsDirty : 1;

		std::vector<component*> m_components;
		std::vector<gameobject*> m_subobjects;

	public:
		void __set_group(group* g);
		void __remove_subobject(gameobject* o);
		void __delete_component(component* c);
	};

	// template definitions

	template<class T>
	inline T* gameobject::create_object(const std::string& name) {
		gameobject* object = create_object(name);
		return object->create_component<T>();
	}

	template<class T>
	inline T* gameobject::create_component() {
		// try to get prexisting one if we can only have one component of this type
		if (component_only_one_v<T>) {
			if (T* c = get_component<T>()) return c;
		}
		// create new
		T* c = new T();
		m_components.push_back(c);
		m_components.back()->__set_object(this);
		m_components.back()->on_create();
		return c;
	}

	template<class T>
	inline T* gameobject::get_component() {
		for (auto* c : m_components) {
			if (T* t = dynamic_cast<T*>(c)) {
				return t;
			}
		}
		return nullptr;
	}

	template<class T>
	inline size_t gameobject::get_components(std::vector<T*>& outComponents) {
		size_t total = 0;
		for (auto* c : m_components) {
			if (T* t = dynamic_cast<T*>(c)) {
				++total;
				outComponents.push_back(t);
				if (component_only_one_v<T>) return 1; // quit early
			}
		}
		return total;
	}

}

#endif // !ALC_OBJECTS_GAMEOBJECT_HPP