#ifndef _CORE_DEBUGGER_HPP
#define _CORE_DEBUGGER_HPP
#include <glm\glm.hpp>
#include <glm\gtx\string_cast.hpp>
#include <string>
#include <mutex>
#include "../common.hpp"

namespace alc {

	class debugger {
		ALC_STATIC_CLASS(debugger);
	public:

		static void log(const std::string& msg, const std::string& file, size_t line);
		static void trace(const std::string& msg, const std::string& file, size_t line);
		static void warning(const std::string& msg, const std::string& file, size_t line);
		static void error(const std::string& msg, const std::string& file, size_t line);
		static void fatal_error(const std::string& msg, const std::string& file, size_t line);

	private:

		static inline std::mutex s_lock;
		static inline std::string s_logFilepath = "";
		static const std::string& get_log_path();
	};

	namespace detail {
		using std::to_string;
		using glm::to_string;
	}

}

#define ALC_DEBUG_LOG(msg) \
	::alc::debugger::log(msg, __FILE__, __LINE__)
#define ALC_DEBUG_TRACE(msg) \
	::alc::debugger::trace(msg, __FILE__, __LINE__)
#define ALC_DEBUG_WARNING(msg) \
	::alc::debugger::warning(msg, __FILE__, __LINE__)
#define ALC_DEBUG_ERROR(msg) \
	::alc::debugger::error(msg, __FILE__, __LINE__)
#define ALC_DEBUG_FATAL_ERROR(msg) \
	::alc::debugger::fatal_error(msg, __FILE__, __LINE__)

#define VTOS(value) \
	::alc::detail::to_string(value)

#endif // !_CORE_DEBUGGER_HPP