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
#ifndef ALC_CONTENT_TEXTURE_HPP
#define ALC_CONTENT_TEXTURE_HPP
#include "../General.hpp"

namespace ALC {

	struct Texture final {

		// default constructor
		Texture();

		// de initializes the texture
		Texture(std::nullptr_t);

		// initializes with the given id and size
		Texture(const uint32 textureID, const uvec2& textureSize);

		// returns true if this is a valid texture
		bool IsValid() const;

		// returns the texture ID
		operator uint32() const;

		// returns the texture ID
		uint32 GetID() const;

		// returns the texture size
		uvec2 GetSize() const;

		// returns a rect with the min set to 0,0 and max set to GetSize()
		Bounds2D GetBounds() const;

		// returns a rect with extents equal to half GetSize()
		Bounds2D GetCenteredBounds() const;

		// compare the textures
		bool operator==(const Texture& other) const;

		// compare the textures
		bool operator!=(const Texture& other) const;

		// functions for loading and deleting textures

		// loads a texture based on the path
		static Texture Load(const string& path);

		// deletes a texture
		static void Delete(const Texture& texture);

	private:
		uint32 m_textureID;
		uvec2 m_textureSize;
	};

}

#endif // !ALC_CONTENT_TEXTURE_HPP