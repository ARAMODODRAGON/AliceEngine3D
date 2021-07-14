#ifndef ALC_OBJECT_LOADER_HPP
#define ALC_OBJECT_LOADER_HPP
#include "object_factory.hpp"

namespace alc {

	class object_loader {
	public:

		object_loader();
		~object_loader();

		// sets the types of components that this object loader can have
		template<typename... Ty>
		void add_component_types();

		void save_object_map(object_factory* factory, const std::string& outputfile);

		void load_object_map(object_factory* factory, const std::string& inputfile);

	private:
		struct loadable_component {
			component* (*addcomponent)(game_object*);
		};
		std::vector<loadable_component> m_componentTypes;
		template<typename Ty>
		loadable_component get_loadable_component();
	};

	template<typename... Ty> 
	object_loader make_object_loader();

	// template implementations /////////////////

	template<typename ...Ty>
	inline void object_loader::add_component_types() { 
		loadable_component loads[] = { get_loadable_component<Ty>()... };
		for (size_t i = 0; i < (sizeof...(Ty)); i++) {
			m_componentTypes.push_back(loads[i]);
		}
	}

	template<typename Ty>
	inline object_loader::loadable_component object_loader::get_loadable_component() {
		loadable_component load;
		load.addcomponent = [](game_object* object) {  
			if (object) return object->add<Ty>();
			return nullptr;
		};
		return loadable_component();
	}

	template<typename ...Ty>
	object_loader make_object_loader() {
		object_loader load;
		load.add_component_types<Ty...>();
		return load;
	}
}

#endif // !ALC_OBJECT_LOADER_HPP