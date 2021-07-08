/*
* MIT License
*
* Copyright (c) 2021 Domara Shlimon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef ALC_GENERAL_HPP
#define ALC_GENERAL_HPP
#include <string>
//#include <string_view>
#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <utility>
#include <memory>
#include <nlohmann\json.hpp>
#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm\gtc\quaternion.hpp>
#undef assert
#include <assert.h>
#ifdef NDEBUG
#define ALC_ASSERT(condition, text)
#else
#define ALC_ASSERT(condition, text) assert(condition)
#endif
#include <type_traits>
#include "DataTypes\Function.hpp"

#define ALC_PI 3.141592653f
#define ALC_TO_RADIANS(degrees) (degrees * (ALC_PI / 180.0f))
#define ALC_TO_DEGREES(radians) (radians * (180.0f / ALC_PI))


namespace ALC {

	using std::size_t;

	using nlohmann::json;

	using std::string;
	//using std::string_view;
	using std::array;
	using std::vector;
	using std::list;
	using std::forward_list;
	using std::map;
	using std::unordered_map;
	using std::pair;

	// using statement for std::shared_ptr<T>
	template<typename T>
	using Ref = std::shared_ptr<T>;

	// using statement for std::unique_ptr<T>
	template<typename T>
	using Scope = std::unique_ptr<T>;

	using uint64 = uint64_t;
	using uint32 = uint32_t;
	using uint16 = uint16_t;
	using uint8 = uint8_t;
	using int64 = int64_t;
	using int32 = int32_t;
	using int16 = int16_t;
	using int8 = int8_t;

	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using glm::ivec2;
	using glm::ivec3;
	using glm::ivec4;
	using glm::uvec2;
	using glm::uvec3;
	using glm::uvec4;
	using glm::mat3;
	using glm::mat4;
	using glm::quat;

	// colors

	#define ALC_COLOR_CLEAR		::ALC::vec4(0.0f, 0.0f, 0.0f, 0.0f)
	#define ALC_COLOR_BLACK		::ALC::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	#define ALC_COLOR_WHITE		::ALC::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	#define ALC_COLOR_RED		::ALC::vec4(1.0f, 0.0f, 0.0f, 1.0f)
	#define ALC_COLOR_GREEN		::ALC::vec4(0.0f, 1.0f, 0.0f, 1.0f)
	#define ALC_COLOR_BLUE		::ALC::vec4(0.0f, 0.0f, 1.0f, 1.0f)
	#define ALC_COLOR_YELLOW	::ALC::vec4(1.0f, 1.0f, 0.0f, 1.0f)
	#define ALC_COLOR_ORANGE	::ALC::vec4(1.0f, (165.0f / 255.0f), 0.0f, 1.0f)
	#define ALC_COLOR_PURPLE	::ALC::vec4(1.0f, 0.0f, 1.0f, 1.0f)

	// creates a color 
	#define ALC_COLOR(r, g, b, a) ::ALC::vec4(r, g, b, a);
	
	// creates a color using only the RGB values and alpha defaulted to 1.0f
	#define ALC_COLOR_RGB(r, g, b) ::ALC::vec4(r, g, b, 1.0f);

	#define _ALC_CEXPR (1.0f / 255.0f)

	// creates a color where the values range from 0 to 255
	#define ALC_COLOR_8(r, g, b, a) ::ALC::vec4(float(r) * _ALC_CEXPR, float(g) * _ALC_CEXPR, float(b) * _ALC_CEXPR, float(a) * _ALC_CEXPR)

	// creates a color where the values range from 0 to 255
	// using only the RGB values and alpha defaulted to 255
	#define ALC_COLOR_RGB8(r, g, b) ::ALC::vec4(float(r) * _ALC_CEXPR, float(g) * _ALC_CEXPR, float(b) * _ALC_CEXPR, 1.0f)

	#define DEFAULT_PERCISION 0.0001f
	inline bool NearlyZero(const float v, const float percision = DEFAULT_PERCISION) {
		return (abs(v) < percision);
	}
	inline bool NearlyEqual(const float v0, const float v1, const float percision = DEFAULT_PERCISION) {
		return (abs(v0 - v1) < percision);
	}

	inline bool NearlyZero(const vec2& v, const float percision = DEFAULT_PERCISION) {
		return (abs(v.x) < percision)
			&& (abs(v.y) < percision);
	}
	inline bool NearlyEqual(const vec2& v0, const vec2& v1, const float percision = DEFAULT_PERCISION) {
		return (abs(v0.x - v1.x) < percision)
			&& (abs(v0.y - v1.y) < percision);
	}

	inline bool NearlyZero(const vec3& v, const float percision = DEFAULT_PERCISION) {
		return (abs(v.x) < percision)
			&& (abs(v.y) < percision)
			&& (abs(v.z) < percision);
	}
	inline bool NearlyEqual(const vec3& v0, const vec3& v1, const float percision = DEFAULT_PERCISION) {
		return (abs(v0.x - v1.x) < percision)
			&& (abs(v0.y - v1.y) < percision)
			&& (abs(v0.z - v1.z) < percision);
	}

	inline bool NearlyZero(const vec4& v, const float percision = DEFAULT_PERCISION) {
		return (abs(v.x) < percision)
			&& (abs(v.y) < percision)
			&& (abs(v.z) < percision)
			&& (abs(v.w) < percision);
	}
	inline bool NearlyEqual(const vec4& v0, const vec4& v1, const float percision = DEFAULT_PERCISION) {
		return (abs(v0.x - v1.x) < percision)
			&& (abs(v0.y - v1.y) < percision)
			&& (abs(v0.z - v1.z) < percision)
			&& (abs(v0.w - v1.w) < percision);
	}
	#undef DEFAULT_PERCISION

	inline float Clamp(const float t, const float min, const float max) {
		if (t < min) return min;
		if (t > max) return max;
		return t;
	}

}

#include "DataTypes\Bounds.hpp"

// disables the copy constructor and operator
#define ALC_NO_COPY(TYPE)				\
TYPE(const TYPE&) = delete;				\
TYPE& operator=(const TYPE&) = delete;		

// disables the move constructor and operator
#define ALC_NO_MOVE(TYPE)				\
TYPE(TYPE&&) = delete;					\
TYPE& operator=(TYPE&&) = delete;

// disables copy/move constructors and operators
// and disables the default constructor and destructor
#define ALC_NON_CONSTRUCTABLE(TYPE)		\
ALC_NO_COPY(TYPE) ALC_NO_MOVE(TYPE)		\
TYPE() = delete;						\
~TYPE() = delete;

#define ALC_SINGLETON(TYPE)								\
ALC_NO_COPY(TYPE) ALC_NO_MOVE(TYPE)						\
public:													\
static TYPE* Get() {									\
	static ::ALC::Scope<TYPE> _instance(new TYPE());	\
	return &_instance.get();							\
}														\
private:

// declares that the class/function/variable is internal to the 
// library and should not be accessed by any external source
#define ALC_INTERNAL

#include "Core\Debugger.hpp"

#endif // !ALC_GENERAL_HPP