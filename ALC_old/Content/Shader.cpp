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
#include "Shader.hpp"
#include <fstream>
#include <glew.h>

namespace ALC {

	Shader::Shader() : m_programID(0) { }

	Shader::Shader(std::nullptr_t) : Shader() { }

	bool Shader::IsValid() const {
		return m_programID != 0;
	}

	Shader::operator uint32() const {
		return m_programID;
	}

	uint32 Shader::GetID() const {
		return m_programID;
	}

	bool Shader::operator==(const Shader& other) const {
		return m_programID == other.m_programID;
	}

	bool Shader::operator!=(const Shader& other) const {
		return m_programID != other.m_programID;
	}

	uint32 Shader::GetUniform(const string& name) {
		return glGetUniformLocation(m_programID, name.c_str());
	}

	static uint32 GetShaderTypeFromString(const string& shadertype) {
		if (shadertype == "vertex") return GL_VERTEX_SHADER;
		if (shadertype == "fragment") return GL_FRAGMENT_SHADER;
		return -1;
	}

	Shader Shader::LoadSource(const string& source) {
		// via "the cherno" https://youtu.be/8wFEzIYRZXg?t=1221

		unordered_map<uint32, string> sources;

		// read the file for each shader
		static const string typeToken = "#type";
		static const size_t typeTokenLen = typeToken.size();
		size_t pos = source.find(typeToken, 0);
		while (pos != string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			if (eol == string::npos) {
				ALC_DEBUG_ERROR("Syntax error");
				return nullptr;
			}
			size_t begin = pos + typeTokenLen + 1;
			uint32 shaderType = GetShaderTypeFromString(source.substr(begin, eol - begin));
			if (shaderType == -1) {
				pos = source.find(typeToken, eol);
				continue;
			}
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos + 2);
			size_t endPos = pos - (nextLinePos == string::npos ? source.size() - 1 : nextLinePos);
			sources.emplace(shaderType, source.substr(nextLinePos, endPos));
		}

		// end 'the cherno'

		if (sources.size() == 0) {
			ALC_DEBUG_ERROR("Invalid shader source");
			return nullptr;
		}

		vector<uint32> shaders;
		uint32 shaderProgram = glCreateProgram();

		for (auto& [type_, source_] : sources) {
			// create and load the shader
			uint32 shaderID = glCreateShader(type_);
			const char* csource = source_.c_str();
			glShaderSource(shaderID, 1, &csource, 0);
			glCompileShader(shaderID);

			// check for errors
			int32 success;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (!success) {
				// get error and print
				char infoLog[512];
				glGetShaderInfoLog(shaderID, 512, 0, infoLog);
				ALC_DEBUG_ERROR("Failed to compile shader: " + string(infoLog));
				ALC_DEBUG_LOG(source_);

				// delete the shader and return
				glDeleteShader(shaderID);
				continue;
			}

			glAttachShader(shaderProgram, shaderID);
			shaders.push_back(shaderID);
		}

		// no shaders were attached
		if (shaders.size() == 0) {
			ALC_DEBUG_ERROR("Could not load any shaders, deleted shader program");
			glDeleteProgram(shaderProgram);
			return nullptr;
		}

		// link
		glLinkProgram(shaderProgram);

		// check linking errors
		GLint success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			// get error message and print
			GLchar infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
			ALC_DEBUG_ERROR("Failed to link program: \n" + string(infoLog));
			// delete shaders/program
			for (GLuint id : shaders) glDeleteShader(id);
			glDeleteProgram(shaderProgram);
			return nullptr;
		}

		// delete shaders
		for (GLuint id : shaders) glDeleteShader(id);
		shaders.clear();

		// create shader and assign program ID
		Shader shader;
		shader.m_programID = shaderProgram;
		return shader;
	}

	Shader Shader::Load(const string& path) {
		std::ifstream file(path);
		if (!file.is_open()) {
			ALC_DEBUG_ERROR("Shader path was invalid: " + path);
			return nullptr;
		}

		// read file into string
		string source;
		file.seekg(0, std::ios::end);
		source.resize(static_cast<size_t>(file.tellg()));
		file.seekg(0, std::ios::beg);
		file.read(source.data(), source.size());
		file.close();

		return LoadSource(source);
	}

	void Shader::Delete(const Shader& shader) {
		glDeleteProgram(shader.m_programID);
	}

}