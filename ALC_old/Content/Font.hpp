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
#ifndef ALC_CONTENT_FONT_HPP
#define ALC_CONTENT_FONT_HPP
#include "../General.hpp"
#include "Texture.hpp"

namespace ALC {

	enum class HAlign : ALC::uint8 {
		Left,
		Center,
		Right
	};

	enum class VAlign : ALC::uint8 {
		Top,
		Middle,
		Bottom
	};

	struct Font final {

		// struct containing character data
		struct Character final {
			vec2 advance;
			vec2 bitSize;
			vec2 position;
			float xoffset;
		};

		// default constructor
		Font();

		// de initializes the font
		Font(std::nullptr_t);

		~Font();

		// String manipulation function(s)
		uvec2 StringDimensions(const string& text) const; // Gets both the width and the height of the provided string in pixels on the screen
		string StringSplitLines(const string& text, const float maxStringWidth); // Splits a single-line block of text into multiple lines dynamically
		vector<float> StringAlignOffsetX(string text, const HAlign hAlign, const vec2& scale) const; // Finds the horizontal offset for the alignment of the string relative to its origin
		float StringAlignOffsetY(string text, const VAlign vAlign, const vec2& scale) const; // Finds the vertical offset for the alignment of the string relative to its origin

		// returns true if this is a valid font
		bool IsValid() const;

		// returns the texture ID
		operator uint32() const;

		// returns the texture
		Texture GetTexture() const;

		// returns the texture ID
		uint32 GetID() const;

		// returns the texture size
		uvec2 GetSize() const;

		// Returns the font's size
		uint32 GetFontSize() const;

		// return's the font's vertical spacing
		uint32 GetVerticalSpacing() const;

		// checks if it contains the given character
		bool Contains(const char c) const;

		// returns the character
		// if not found, throws std::out_of_range
		const Character& At(const char c) const;

		// returns the character
		const Character& operator[](const char c) const;

		// returns the number of characters
		size_t Size() const;

		// compare the fonts
		bool operator==(const Font& other) const;

		// compare the fonts
		bool operator!=(const Font& other) const;

		// functions for loading and deleting fonts

		// loads a font based on the path
		static Font Load(const string& path, const uint32 size, const uint32 vSpacing);

		// deletes a font
		static void Delete(const Font& font);

	private:
		uint32 m_textureID;
		uvec2 m_textureSize;
		Ref<unordered_map<char, Character>> m_characters;
		uint32 m_fontSize;
		uint32 m_verticalSpacing;

		// Private string manipulation functions
		float StringGetOffsetX(string substr, HAlign hAlign) const;
	};

}

#endif // !ALC_CONTENT_FONT_HPP