#ifndef ALC_OBJECT_LOADER_HPP
#define ALC_OBJECT_LOADER_HPP
#include "object_factory.hpp"

namespace alc {

	// represents an object that you can use to load/unload to a file
	class object_prefab {

	};

	// builder class used to load and save factories into object files
	class object_loader {
	public:

		object_loader();
		~object_loader();

		// adds types of components that this object loader can load
		template<typename... Ty>
		void add_component_types();

		void save_object_map(object_factory* factory, const std::string& outputfile);

		void load_object_map(object_factory* factory, const std::string& inputfile);

	private:
		std::vector<type> m_componentTypes;
	};

	template<typename... Ty> 
	object_loader make_object_loader();

	// template implementations /////////////////

	template<typename ...Ty>
	inline void object_loader::add_component_types() { 
		type types[] = { type::get<Ty>()... };
		for (size_t i = 0; i < (sizeof...(Ty)); i++) {
			m_componentTypes.push_back(types[i]);
		}
	}

	template<typename ...Ty>
	object_loader make_object_loader() {
		object_loader load;
		load.add_component_types<Ty...>();
		return load;
	}
}

#endif // !ALC_OBJECT_LOADER_HPP