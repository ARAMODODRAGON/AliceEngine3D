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
#ifndef ALC_CONTENT_CONTENTMANAGER_HPP
#define ALC_CONTENT_CONTENTMANAGER_HPP
#include "../General.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Font.hpp"

namespace ALC {

	// used as storage to store textures/shaders/fonts
	// cannot be directly interacted with
	class ContentStorage final {
		ALC_NO_COPY(ContentStorage);
	public:

		ContentStorage();
		~ContentStorage();

	private:
		friend class ContentManager;
		unordered_map<string, Texture> m_textures;
		unordered_map<string, Shader> m_shaders;
		unordered_map<string, Font> m_fonts;
	};

	class ContentManager final {
		ALC_NON_CONSTRUCTABLE(ContentManager)
	public:

		// returns the current storage
		static ContentStorage& Current();
		
		// returns the non contextual default storage
		static ContentStorage& Default();

		// loads a texture and stores it in an internal storage, or the set context
		static Texture LoadTexture(const string& path);

		// loads a texture and stores it in the storage
		static Texture LoadTexture(ContentStorage& storage, const string& path);

		// loads a shader file and stores it in an internal storage, or the set context
		static Shader LoadShader(const string& path);
		
		// loads a shader file and stores it in the storage
		static Shader LoadShader(ContentStorage& storage, const string& path);

		// loads a shader source and stores it in an internal storage, or the set context
		static Shader LoadShaderSource(const string& source);

		// loads a shader source and stores it in the storage
		static Shader LoadShaderSource(ContentStorage& storage, const string& source);

		// loads a font file and stores it in an internal storage, or the set context
		static Font LoadFont(const string& path, const uint32 size, const uint32 vSpacing = 1U);
		
		// loads a font file and stores it in the storage
		static Font LoadFont(ContentStorage& storage, const string& path, const uint32 size, const uint32 vSpacing = 1U);

		// clears out and deletes the content stored in the content manager
		static void Clear();

		// clears out and deletes the content stored in the storage
		static void Clear(ContentStorage& storage);

		// sets the passed in storage as the default storage to load into
		// any content loaded before setting the storage does not move to this storage
		static void SetContext(ContentStorage& context);

	private:
		static ContentStorage s_genericStorage;
		static ContentStorage* s_contextStorage;
	public:
		static void __RemoveContext(ContentStorage* context);
	};

}

#endif // !ALC_CONTENT_CONTENTMANAGER_HPP