#ifndef ALC_COMMON_HPP
#define ALC_COMMON_HPP
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include <list>
#include <inttypes.h>
#include <memory>
#include <utility>

namespace alc {

	using int8 = int8_t;
	using int16 = int16_t;
	using int32 = int32_t;
	using int64 = int64_t;
	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;

	using size_t = std::size_t;
}

#define ALC_NO_COPY(Type)				\
Type(const Type&) = delete;				\
Type& operator=(const Type&) = delete;

#define ALC_NO_MOVE(Type)				\
Type(Type&&) = delete;					\
Type& operator=(Type&&) = delete;

#define ALC_STATIC_CLASS(Type)			\
ALC_NO_COPY(Type) ALC_NO_MOVE(Type)		\
Type() = delete;

#endif // !ALC_COMMON_HPP