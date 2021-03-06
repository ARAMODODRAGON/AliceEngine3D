#include "debug.hpp"
#include <iostream>
#include <fstream>

#define LOG_FILE_PATH std::string("Logs/Log_")
using mutex_guard = std::lock_guard<std::mutex>;

const std::string& alc::debugger::get_log_path() {
	if (s_logFilepath == "") {
		// find a unique file name to use
		size_t i = 0;
		s_logFilepath = LOG_FILE_PATH + std::to_string(i);
		std::fstream file(s_logFilepath);
		while (file.is_open()) {
			// try to open a file with name:
			file.close();
			++i;
			s_logFilepath = LOG_FILE_PATH + std::to_string(i);
			file.open(s_logFilepath);
		}
		file.close();
	}
	return s_logFilepath;
}

void alc::debugger::log(const std::string& msg, const std::string& file, size_t line) {
	mutex_guard mg(s_lock);
	// open file at the end
	std::ofstream filestream;
	filestream.open(get_log_path(), std::ios::app);

	// add msg
	filestream << "[Log(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
	//#if _DEBUG
	std::cout << "[Log]: " << msg << std::endl;
	//#endif

	// close
	filestream.close();
}

void alc::debugger::trace(const std::string& msg, const std::string& file, size_t line) {
	mutex_guard mg(s_lock);
	// open file at the end
	std::ofstream filestream;
	filestream.open(get_log_path(), std::ios::app);

	// add msg
	filestream << "[Trace(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
	//#if _DEBUG
	std::cout << "[Trace]: " << msg << std::endl;
	//#endif

	// close
	filestream.close();
}

void alc::debugger::warning(const std::string& msg, const std::string& file, size_t line) {
	mutex_guard mg(s_lock);
	// open file at the end
	std::ofstream filestream;
	filestream.open(get_log_path(), std::ios::app);

	// add msg
	filestream << "[Warning(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
	//#if _DEBUG
	std::cout << "[Warning]: " << msg << std::endl;
	//#endif

	// close
	filestream.close();
}

void alc::debugger::error(const std::string& msg, const std::string& file, size_t line) {
	mutex_guard mg(s_lock);
	// open file at the end
	std::ofstream filestream;
	filestream.open(get_log_path(), std::ios::app);

	// add msg
	filestream << "[Error(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
	//#if _DEBUG
	std::cout << "[Error]: " << msg << std::endl;
	//#endif

	// close
	filestream.close();
}

void alc::debugger::fatal_error(const std::string& msg, const std::string& file, size_t line) {
	mutex_guard mg(s_lock);
	// open file at the end
	std::ofstream filestream;
	filestream.open(get_log_path(), std::ios::app);

	// add msg
	filestream << "[FatalError(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
	//#if _DEBUG
	std::cout << "[FatalError]: " << msg << std::endl;
	//#endif

	// close
	filestream.close();
}
