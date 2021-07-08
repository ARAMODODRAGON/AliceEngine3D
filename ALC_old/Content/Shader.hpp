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
#ifndef ALC_CONTENT_SHADER_HPP
#define ALC_CONTENT_SHADER_HPP
#include "../General.hpp"

namespace ALC {

	struct Shader final {

		// default constructor
		Shader();

		// creates invalid shader
		// same as default constructor
		Shader(std::nullptr_t);

		// checks if this is a valid shader
		bool IsValid() const;

		// returns the shader ID
		operator uint32() const;

		// returns the shader ID
		uint32 GetID() const;

		// compares this to other
		bool operator==(const Shader& other) const;

		// compares this to other
		bool operator!=(const Shader& other) const;

		// returns the uniform location of the given name
		uint32 GetUniform(const string& name);

		// functions for loading shaders

		// loads a shader from source
		static Shader LoadSource(const string& source);

		// loads a shader from the given path
		static Shader Load(const string& path);

		// deletes the shader
		static void Delete(const Shader& shader);

	private:
		uint32 m_programID;
	};

}

#endif // !ALC_CONTENT_SHADER_HPP