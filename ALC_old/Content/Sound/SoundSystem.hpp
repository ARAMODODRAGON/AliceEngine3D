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
#ifndef SOUND_SYSTEM_HPP
#define SOUND_SYSTEM_HPP

#include "../../General.hpp"

namespace ALC {

	class SoundSystem {
		ALC_NON_CONSTRUCTABLE(SoundSystem);
	public:
		
		// starts playing the music file,
		// overrides whatever was last playing
		static void PlayMusic(const string& file, int _loops = -1);

		// starts playing the sound file
		static void PlaySound(const string& file, int _loops = 0, int _channel = -1);

		// pauses the currently playing music
		static void PauseMusic();
		
		// plays the current music
		static void ResumeMusic();

		// loads a WAVE, MOD, MIDI, OGG, MP3, or FLAC file
		static bool LoadMusic(const string& file);

		// loads a WAVE, AIFF, RIFF, OGG, or VOC file
		static bool LoadSound(const string& file);

		static void UnloadMusic(const string& file);
		static void UnloadSound(const string& file);
	};

}

#endif