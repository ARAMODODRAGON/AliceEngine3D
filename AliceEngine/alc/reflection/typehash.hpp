#ifndef ALC_REFLECTION_TYPEHASH_HPP
#define ALC_REFLECTION_TYPEHASH_HPP
#include "../common.hpp"

namespace alc {

	namespace detail { uint32 type_counter(); }

	// represents a type
	// the value of a typehash is not tied to the values indexed in the enum
	enum class typehash : uint32 { null = static_cast<uint32>(-1) };

	// returns the typehash for the specific type
	// garunteed unique for every type
	template<typename T>
	typehash get_typehash();

	// implementations

	namespace detail {
		inline uint32 type_counter() {
			static uint32 counter = 0;
			return counter++;
		}
	}

	template<typename T>
	inline typehash get_typehash() {
		static const typehash tyhash = static_cast<typehash>(detail::type_counter());
		return tyhash;
	}

}

#endif // !ALC_REFLECTION_TYPEHASH_HPP