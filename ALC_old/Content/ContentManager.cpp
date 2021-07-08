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
#include "ContentManager.hpp"

namespace ALC {

	ContentStorage::ContentStorage() { }
	ContentStorage::~ContentStorage() {
		ContentManager::__RemoveContext(this);
		ContentManager::Clear(*this);
	}

	ContentStorage ContentManager::s_genericStorage;
	ContentStorage* ContentManager::s_contextStorage = nullptr;

	ContentStorage& ContentManager::Current() {
		if (s_contextStorage)
			return *s_contextStorage;
		return s_genericStorage;
	}
	
	ContentStorage& ContentManager::Default() {
		return s_genericStorage;
	}

	Texture ContentManager::LoadTexture(const string& path) {
		if (s_contextStorage)
			return LoadTexture(*s_contextStorage, path);
		return LoadTexture(s_genericStorage, path);
	}

	Texture ContentManager::LoadTexture(ContentStorage& storage, const string& path) {
		// check if it already exists
		auto it = storage.m_textures.find(path);
		if (it != storage.m_textures.end())
			return it->second;

		// load the texture and add it to the map
		Texture texture = Texture::Load(path);
		if (texture) storage.m_textures.emplace(path, texture);

		// return the newly loaded texture
		return texture;
	}

	Shader ContentManager::LoadShader(const string& path) {
		if (s_contextStorage)
			return LoadShader(*s_contextStorage, path);
		return LoadShader(s_genericStorage, path);
	}

	Shader ContentManager::LoadShader(ContentStorage& storage, const string& path) {
		// check if it already exists
		auto it = storage.m_shaders.find(path);
		if (it != storage.m_shaders.end())
			return it->second;

		// load the shader and add it to the map
		Shader shader = Shader::Load(path);
		if (shader) storage.m_shaders.emplace(path, shader);

		// return the newly loaded shader
		return shader;
	}

	Shader ContentManager::LoadShaderSource(const string& source) {
		if (s_contextStorage)
			return LoadShaderSource(*s_contextStorage, source);
		return LoadShaderSource(s_genericStorage, source);
	}

	Shader ContentManager::LoadShaderSource(ContentStorage& storage, const string& source) {
		// check if it already exists
		auto it = storage.m_shaders.find(source);
		if (it != storage.m_shaders.end())
			return it->second;

		// load the shader and add it to the map
		Shader shader = Shader::LoadSource(source);
		if (shader) storage.m_shaders.emplace(source, shader);

		// return the newly loaded shader
		return shader;
	}

	Font ContentManager::LoadFont(const string& path, const uint32 size, const uint32 vSpacing) {
		if (s_contextStorage)
			return LoadFont(*s_contextStorage, path, size, vSpacing);
		return LoadFont(s_genericStorage, path, size, vSpacing);
	}

	Font ContentManager::LoadFont(ContentStorage& storage, const string& path, const uint32 size, const uint32 vSpacing) {
		string code = path + "{Size:" + VTOS(size) + "}";

		// check if it already exists
		auto it = storage.m_fonts.find(code);
		if (it != storage.m_fonts.end())
			return it->second;

		// load the shader and add it to the map
		Font font = Font::Load(path, size, vSpacing);
		if (font) storage.m_fonts.emplace(code, font);

		// return the newly loaded shader
		return font;
	}

	void ContentManager::Clear() {
		if (s_contextStorage)
			Clear(*s_contextStorage);
		Clear(s_genericStorage);
	}

	void ContentManager::Clear(ContentStorage& storage) {

		// iterate through and delete all of the textures
		for (auto& [key, texture] : storage.m_textures) {
			Texture::Delete(texture);
		}
		storage.m_textures.clear();

		// iterate through and delete all of the shaders
		for (auto& [key, shader] : storage.m_shaders) {
			Shader::Delete(shader);
		}
		storage.m_shaders.clear();

		// iterate through and delete all of the fonts
		for (auto& [key, font] : storage.m_fonts) {
			Font::Delete(font);
		}
		storage.m_fonts.clear();

	}

	void ContentManager::SetContext(ContentStorage& storage) {
		s_contextStorage = &storage;
	}

	void ContentManager::__RemoveContext(ContentStorage* storage) {
		// remove the storage only if it is the context
		if (s_contextStorage == storage)
			s_contextStorage = nullptr;
	}

}