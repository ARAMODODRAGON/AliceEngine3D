#ifndef ALC_OBJECTS_IGROUP_HPP
#define ALC_OBJECTS_IGROUP_HPP
#include "../common.hpp"
#include "../datatypes/function.hpp"
#include "gameobject.hpp"

namespace alc {

	// provides custom logic to groups
	class behavior {
	public:

		behavior();
		virtual ~behavior() = 0;

		// virtual functions

		virtual void on_create() { }
		virtual void on_delete() { }
		virtual void on_update(timestep ts) { }

		// returns the group that this is attached to
		group* get_group() const;

		// returns whether this behavior should actively update or not
		bool get_should_update() const;

		// sets if this behavior should actively update or not
		void set_should_update(bool shouldUpdate);

		// deletes this behavior
		bool delete_this();

	private:
		group* m_group;
		bool m_shouldUpdate : 1;
	public:
		void __set_group(group* g);
	};

	// groups can hold gameobjects, behaviors, and subgroups 
	// can provide special functionality such as multi frame loading sequences and group components
	class group final {
	public:

		group(const std::string& name);
		~group();

		// gets the parent group
		group* get_parent() const;

		// sets the parent group, can also be set to nullptr to make group global
		void set_parent(group* g);

		// returns the name of this group
		std::string get_name() const;

		// sets the name of this group
		void set_name(const std::string& name);

		//// groups

		// creates a group attached to this group
		group* create_group(const std::string& name = "");

		// creates a global group with behavior of type
		template<class T> T* create_group(const std::string& name = "");

		// deletes a group
		bool delete_group(group* g);

		// deletes this group
		bool delete_this();

		/// iteration

		// returns the number of subgroups
		size_t subgroup_size() const;

		// returns the subgroup at index
		group* get_subgroup(size_t index);

		/// primary group

		// returns if this is the primary group
		bool is_primary_group() const;


		//// behavior

		// creates a behavior attached to this group
		template<class T> T* create_behavior();

		// deletes the behavior if it's attached to this group
		bool delete_behavior(behavior* b);

		// gets a behavior of type
		template<class T> T* get_behavior();

		// gets multiple group_behaviors of type
		// returns amount of group_behaviors found
		template<class T> size_t get_behaviors(std::vector<T*>& outGroupBehaviors);


		//// gameobject

		// creates a new object
		gameobject* create_object(const std::string& name = "");

		// creates an object with component of type
		template<class T> T* create_object(const std::string& name = "");

		// deletes an object
		bool delete_object(gameobject* object);

		// gets an object by name
		gameobject* get_object(const std::string& name);


		////// events
		//
		//// event invoked when a component change is made and the flags are set for the particular component
		//event<void, component_alert_flags, component*> componentAlertEvent;


	private:

		group* m_parent;

		std::string m_name;

		std::vector<behavior*> m_behaviors;
		std::vector<gameobject*> m_objects;
		std::vector<group*> m_subgroups;

		void __remove_subgroup(group* g);

	public:
		void __delete_object(gameobject* object);
		void __delete_group(group* g);
		void __remove_object(gameobject* object);
		void __add_object(gameobject* object);
	};


	// template implementation

	template<class T>
	inline T* group::create_group(const std::string& name) {
		group* g = create_group(name);
		T* b = g->create_behavior<T>();
		return b;
	}

	template<class T>
	inline T* group::create_behavior() {
		// get behavior if only one can exist
		if (behavior_only_one_v<T>) {
			if (T* b = get_behavior<T>()) return b;
		}
		// create new
		T* b = new T();
		m_behaviors.push_back(b);
		m_behaviors.back()->__set_group(this);
		m_behaviors.back()->on_create();
		return b;
	}

	template<class T>
	inline T* group::get_behavior() {
		for (auto* b : m_behaviors) {
			if (T* t = dynamic_cast<T*>(b)) {
				return t;
			}
		}
		return nullptr;
	}

	template<class T>
	inline size_t group::get_behaviors(std::vector<T*>& outGroupBehaviors) {
		size_t total = 0;
		for (auto* b : m_behaviors) {
			if (T* t = dynamic_cast<T*>(b)) {
				++total;
				outGroupBehaviors.push_back(t);
				if (behavior_only_one_v<T>) return 1; // quit early
			}
		}
		return total;
	}

	template<class T> 
	inline T* group::create_object(const std::string& name) {
		gameobject* object = create_object(name);
		return object->create_component<T>();
	}

}

#endif // !ALC_OBJECTS_IGROUP_HPP