#ifndef ALC_OBJECTS_SINGLETON_HPP
#define ALC_OBJECTS_SINGLETON_HPP
#include "world.hpp"

namespace alc {

	// class to make single instance objects
	class singleton : public object {
	public:

		template<typename ObjectTy>
		static ObjectTy* get();

	};

	// creates a static get function to access the singleton within the singleton class
	#define ALC_SINGLETON_GETTER(Class)						\
	public:													\
	static Class* get() { return singleton::get<Class>(); }	\
	private:

	// template implementation

	template<typename ObjectTy>
	inline ObjectTy* singleton::get() {
		if constexpr (std::is_base_of_v<singleton, ObjectTy>) {
			static ObjectTy* instance = nullptr;
			if (instance == nullptr) {
				instance = world::create<ObjectTy>("Singleton");
			}
			return instance;
		}
		ALC_DEBUG_ERROR("cannot make singleton of non singleton type");
		return nullptr;
	}

}

#endif // !ALC_OBJECTS_SINGLETON_HPP