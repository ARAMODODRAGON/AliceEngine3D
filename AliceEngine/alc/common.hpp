#ifndef ALC_COMMON_HPP
#define ALC_COMMON_HPP
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <inttypes.h>
#include <memory>
#include <utility>
#include <unordered_map>

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

// allows flag usage for enum class types
#define ALC_FLAGS_ENUM(Enumclass)															\
inline Enumclass operator&(Enumclass left, Enumclass right) {								\
	return static_cast<Enumclass>(static_cast<std::underlying_type_t<Enumclass>>(left) &	\
								  static_cast<std::underlying_type_t<Enumclass>>(right));	\
}																							\
inline Enumclass& operator&=(Enumclass& left, Enumclass right) {							\
	left = static_cast<Enumclass>(static_cast<std::underlying_type_t<Enumclass>>(left) &	\
								  static_cast<std::underlying_type_t<Enumclass>>(right));	\
	return left;																			\
}																							\
inline Enumclass operator|(Enumclass left, Enumclass right) {								\
	return static_cast<Enumclass>(static_cast<std::underlying_type_t<Enumclass>>(left) |	\
								  static_cast<std::underlying_type_t<Enumclass>>(right));	\
}																							\
inline Enumclass& operator|=(Enumclass& left, Enumclass right) {							\
	left = static_cast<Enumclass>(static_cast<std::underlying_type_t<Enumclass>>(left) |	\
								  static_cast<std::underlying_type_t<Enumclass>>(right));	\
	return left;																			\
}																							\
inline Enumclass operator^(Enumclass left, Enumclass right) {								\
	return static_cast<Enumclass>(static_cast<std::underlying_type_t<Enumclass>>(left) ^	\
								  static_cast<std::underlying_type_t<Enumclass>>(right));	\
}																							\
inline Enumclass& operator^=(Enumclass& left, Enumclass right) {							\
	left = static_cast<Enumclass>(static_cast<std::underlying_type_t<Enumclass>>(left) ^	\
								  static_cast<std::underlying_type_t<Enumclass>>(right));	\
	return left;																			\
}																							\
inline Enumclass operator~(Enumclass value) {												\
	return static_cast<Enumclass>(~static_cast<std::underlying_type_t<Enumclass>>(value));	\
}

#endif // !ALC_COMMON_HPP