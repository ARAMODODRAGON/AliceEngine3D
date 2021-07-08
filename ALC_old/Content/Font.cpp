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
#include "Font.hpp"
#include <stdexcept>
#include "../Core/SceneManager.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glew.h>

namespace ALC {

	Font::Font() : m_textureID(0), m_textureSize(0), m_characters(nullptr), m_fontSize(0), m_verticalSpacing(0) { }

	Font::Font(std::nullptr_t) : m_textureID(0), m_textureSize(0), m_characters(nullptr), m_fontSize(0), m_verticalSpacing(0) { }

	Font::~Font() {

	}

	// STRING MANIPULATION FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////

	uvec2 Font::StringDimensions(const string& text) const {
		uvec2 dimensions(0.0f);
		float offset = 0.0f;
		
		// If no character set has been loaded for the font; don't attempt to calculate anything
		if (m_characters == nullptr) {
			ALC_DEBUG_ERROR("Invalid font has been used! No calculations will be processed.");
			return dimensions;
		}

		for (const char* p = text.c_str(); *p; p++) {
			if (*p < 32 && *p != '\n') continue;

			if (*p == '\n') { // Newline text
				offset = 0.0f; // Reset the x offset of the text
				dimensions[1] += (float) (GetFontSize() + GetVerticalSpacing());
				continue;
			}

			const Font::Character& c = m_characters.get()->at(*p);

			float x0 = offset + ((c.position.x / c.bitSize.x) * c.bitSize.x);
			float w = c.bitSize.x;

			// move cursor to the start of the next character
			offset += c.advance.x;

			if (dimensions.x < offset) { dimensions[0] = offset; }
		}

		// Add the height for the line of text that isn't hit within the loop
		if (!text.empty()) {
			dimensions[1] += GetFontSize();
		}

		return dimensions;
	}

	string Font::StringSplitLines(const string& text, const float maxStringWidth) {
		string curWord = "", curLine = "", result = "";
		float wordStartPos = 0.0f, wordWidth = 0.0f, offset = 0.0f;

		// If no character set has been loaded for the font; don't attempt to calculate anything
		if (m_characters == nullptr) {
			ALC_DEBUG_ERROR("Invalid font has been used! No calculations will be processed.");
			return "";
		}

		for (const char* p = text.c_str(); *p; p++) {
			if (*p < 32) continue;
			const Font::Character& c = m_characters.get()->at(*p);

			// Get the position of the quad on-screen, but unaltered by scale or translation
			float x0 = offset + ((c.position.x / c.bitSize.x) * c.bitSize.x);
			float w = c.bitSize.x;

			// move cursor to the start of the next character
			offset += c.advance.x;

			if (*p == ' ') { // A space has been found; see if the word can fit onto the current line or needs to be part of a new line
				wordWidth = offset - wordStartPos;

				// The word isn't able to fit onto the current line; place it on a new line and start adding other words to that new line as well
				if (offset + wordWidth > maxStringWidth) {

					result += curLine + '\n';
					offset = wordWidth;

					curLine = curWord;
					curWord = "";

					continue;
				}

				// The word can fit onto the current line; add a space in between words as well
				if (!curLine.empty()) { curLine += " "; }
				curLine += curWord;
				curWord = "";

			} else { // Continue adding characters to the word until a space is reached

				if (curWord.empty()) { wordStartPos = x0; }
				curWord += *p;

			}
		}
		
		// Finally, add the last line and final word into the string; checking if the word will surpass the maximum alloted width or not -- like how it works within the loop
		wordWidth = offset - wordStartPos;
		if (offset + wordWidth > maxStringWidth) { // The final word is placed onto a new and final line
			result += curLine + '\n' + curWord;
		} else { // The final line and word both fit on the same line; combine them
			result += curLine + " " + curWord;
		}

		return result;
	}

	vector<float> Font::StringAlignOffsetX(string text, const HAlign hAlign, const vec2& scale) const {
		vector<float> offsets;
		offsets.reserve(3);

		if (text.empty()) { // Return an empty vector if an empty string is attempting to be aligned
			return vector<float>();
		}

		size_t pos = 0;
		while ((pos = text.find('\n')) != string::npos) {
			offsets.push_back(StringGetOffsetX(text.substr(0, pos), hAlign) * scale.x);
			text.erase(0, pos + string("\n").length());
		}
		offsets.push_back(StringGetOffsetX(text.substr(0, pos), hAlign) * scale.x);

		return offsets;
	}

	float Font::StringAlignOffsetY(string text, const VAlign vAlign, const vec2& scale) const {
		float height = 0.0f;

		if (vAlign == VAlign::Top || text.empty()) { // No offset necessary
			return 0.0f;
		}

		size_t pos = 0;
		while ((pos = text.find('\n')) != string::npos) {
			height += GetFontSize() + (float) GetVerticalSpacing();
			text.erase(0, pos + string("\n").length());
		}
		height += GetFontSize();

		if (vAlign == VAlign::Middle) { // Cut the offset value in half
			return roundf((height * scale.y) / 2.0f);
		}

		return height * scale.y;
	}

	// A private member that only returns the x offset of the line of text relative to the alignment that was set
	float Font::StringGetOffsetX(string substr, HAlign hAlign) const {
		if (hAlign == HAlign::Center) { // Cut the offset in half based on the line's width
			return roundf(StringDimensions(substr).x / 2.0f);
		} else if (hAlign == HAlign::Right) { // Make the offset the full length of the line's width
			return StringDimensions(substr).x;
		} else { // No offset necessary
			return 0.0f;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Font::IsValid() const {
		return m_textureID != 0;
	}

	Font::operator uint32() const {
		return m_textureID;
	}

	Texture Font::GetTexture() const {
		return Texture(m_textureID, m_textureSize);
	}

	uint32 Font::GetID() const {
		return m_textureID;
	}

	uvec2 Font::GetSize() const {
		return m_textureSize;
	}

	uint32 Font::GetFontSize() const {
		return m_fontSize;
	}

	uint32 Font::GetVerticalSpacing() const {
		return m_verticalSpacing;
	}

	bool Font::Contains(const char c) const {
		if (m_characters.get() == nullptr)
			return false;
		return m_characters.get()->find(c) != m_characters.get()->end();
	}

	const Font::Character& Font::At(const char c) const {
		if (m_characters.get() == nullptr)
			throw std::out_of_range("map was null");
		return m_characters.get()->at(c);
	}

	const Font::Character& Font::operator[](const char c) const {
		return m_characters.get()->operator[](c);
	}

	size_t Font::Size() const {
		if (m_characters.get() == nullptr)
			return 0;
		return m_characters->size();
	}

	bool Font::operator==(const Font& other) const {
		return m_textureID == other.m_textureID;
	}

	bool Font::operator!=(const Font& other) const {
		return m_textureID != other.m_textureID;
	}

	Font Font::Load(const string& path, const uint32 size, const uint32 vSpacing) {
		// get the library
		FT_Library lib = SceneManager::__GetFTLibrary();

		// create a face
		FT_Face face;
		if (FT_New_Face(lib, path.c_str(), 0, &face)) {
			ALC_DEBUG_ERROR("Failed to load the font at path " + path);
			return nullptr;
		}

		// dunno what this is about
		FT_Set_Pixel_Sizes(face, 0, size);

		// get the size of the texture atlas that the font will be stored on
		FT_GlyphSlot g = face->glyph;
		unsigned int w = 0, h = 0;
		for (int i = 32; i < 128; i++) {
			if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
				ALC_DEBUG_ERROR("Failed to load glyph at index: " + VTOS(i));
				continue;
			}
			w += g->bitmap.width;
			if (h < g->bitmap.rows)
				h = g->bitmap.rows;
		}

		// create our texture
		unsigned int textureID = -1;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		Ref<unordered_map<char, Character>> characters(new unordered_map<char, Character>());
		int x = 0;
		for (int i = 32; i < 128; i++) {
			if (FT_Load_Char(face, i, FT_LOAD_RENDER)) { // Skip over the elements that aren't properly loaded
				continue;
			}
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			Character character{};
			character.advance.x = static_cast<float>(g->advance.x >> 6);
			character.advance.y = static_cast<float>(g->advance.y >> 6);
			character.bitSize.x = static_cast<float>(g->bitmap.width);
			character.bitSize.y = static_cast<float>(g->bitmap.rows);
			character.position.x = static_cast<float>(g->bitmap_left);
			character.position.y = static_cast<float>(g->bitmap_top);
			character.xoffset = static_cast<float>(x) / static_cast<float>(w);

			characters->insert(pair<char, Character>(i, character));
			x += g->bitmap.width;
		}

		// cleanup
		glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		FT_Done_Face(face);

		// create and return the font
		Font font;
		font.m_textureSize = uvec2(w, h);
		font.m_textureID = textureID;
		font.m_characters = std::move(characters);
		font.m_fontSize = size;
		font.m_verticalSpacing = vSpacing;
		return font;
	}

	void Font::Delete(const Font& font) {
		glDeleteTextures(1, &font.m_textureID);
	}
}