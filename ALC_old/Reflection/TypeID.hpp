#ifndef ALC_REFLECTION_TYPEID_HPP
#define ALC_REFLECTION_TYPEID_HPP
#include <inttypes.h>
#include <functional>

namespace ALC {

	enum class TypeID : uint32_t { };
	inline TypeID nulltype = static_cast<TypeID>(0);

	namespace detail {
		inline uint32_t _NextCounter() {
			static uint32_t counter = 1;
			return counter++;
		}
	}

	template<typename T>
	inline TypeID GetTypeID() {
		static TypeID typeID = static_cast<TypeID>(detail::_NextCounter());
		return typeID;
	}

	inline size_t HashType(TypeID id) {
		return std::hash<uint32_t>()(static_cast<uint32_t>(id));
	}

}

namespace std {

}

#endif // !ALC_REFLECTION_TYPEID_HPP