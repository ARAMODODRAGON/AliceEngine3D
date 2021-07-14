#ifndef ALC_DATATYPES_HASH_HPP
#define ALC_DATATYPES_HASH_HPP
#include "../common.hpp"

namespace alc {

	// basic hash value with 32 bit representation
	enum class hash32_t : uint32 { };

	// basic hash value with 64 bit representation
	enum class hash64_t : uint64 { };

	// basic string hash function
	template<typename hashTy = hash32_t>
	hashTy hash_string(const std::string& value);


	// implementations

	template<typename hashTy>
	inline hashTy hash_string(const std::string& value) {
		return std::hash<std::string>{}(value);
	}

}

#endif // !ALC_DATATYPES_HASH_HPP