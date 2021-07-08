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
#include "Debugger.hpp"
#include <iostream>
#include <fstream>
//#include "../Jobs\Jobs.hpp"
#include <mutex>

#define LOG_FILE_PATH string("Logs/Log_")

namespace ALC {

	using mutex_guard = std::lock_guard<std::mutex>;

	namespace {
		std::mutex m;
	}

	static const string& GetLogPath() {
		static string logFilepath = "";
		if (logFilepath == "") {
			// find a unique file name to use
			size_t i = 0;
			logFilepath = LOG_FILE_PATH + std::to_string(i);
			std::fstream file(logFilepath);
			while (file.is_open()) {
				// try to open a file with name:
				file.close();
				++i;
				logFilepath = LOG_FILE_PATH + std::to_string(i);
				file.open(logFilepath);
			}
			file.close();
		}
		return logFilepath;
	}

	void Debugger::Log(const string& msg, const string& file, size_t line) {
		mutex_guard mg(m);
		// open file at the end
		std::ofstream filestream;
		filestream.open(GetLogPath(), std::ios::app);

		// add msg
		filestream << "[Log(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
		//#if _DEBUG
		std::cout << "[Log]: " << msg << std::endl;
		//#endif

		// close
		filestream.close();
	}

	void Debugger::Trace(const string& msg, const string& file, size_t line) {
		mutex_guard mg(m);
		// open file at the end
		std::ofstream filestream;
		filestream.open(GetLogPath(), std::ios::app);

		// add msg
		filestream << "[Trace(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
		//#if _DEBUG
		std::cout << "[Trace]: " << msg << std::endl;
		//#endif

		// close
		filestream.close();
	}

	void Debugger::Warning(const string& msg, const string& file, size_t line) {
		mutex_guard mg(m);
		// open file at the end
		std::ofstream filestream;
		filestream.open(GetLogPath(), std::ios::app);

		// add msg
		filestream << "[Warning(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
		//#if _DEBUG
		std::cout << "[Warning]: " << msg << std::endl;
		//#endif

		// close
		filestream.close();
	}

	void Debugger::Error(const string& msg, const string& file, size_t line) {
		mutex_guard mg(m);
		// open file at the end
		std::ofstream filestream;
		filestream.open(GetLogPath(), std::ios::app);

		// add msg
		filestream << "[Error(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
		//#if _DEBUG
		std::cout << "[Error]: " << msg << std::endl;
		//#endif

		// close
		filestream.close();
	}

	void Debugger::FatalError(const string& msg, const string& file, size_t line) {
		mutex_guard mg(m);
		// open file at the end
		std::ofstream filestream;
		filestream.open(GetLogPath(), std::ios::app);

		// add msg
		filestream << "[FatalError(ln:" << line << ")]: " << msg << std::endl << "\t[file: " << file << "]" << std::endl;
		//#if _DEBUG
		std::cout << "[FatalError]: " << msg << std::endl;
		//#endif

		// close
		filestream.close();
	}

}