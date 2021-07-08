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
#include "SoundSystem.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

namespace ALC {

	static unordered_map<string, Mix_Music*> g_music;
	static unordered_map<string, Mix_Chunk*> g_sounds;

	void SoundSystem::PlayMusic(const string& file, int _loops) {
		auto it = g_music.find(file);
		if (it != g_music.end()) {
			Mix_PlayMusic(it->second, _loops);
			return;
		}
		ALC_DEBUG_ERROR("Could not play music file. It was not found. " + file);
	}

	void SoundSystem::PlaySound(const string& file, int _loops, int _channel) {
		auto it = g_sounds.find(file);
		if (it != g_sounds.end()) {
			Mix_PlayChannel(_channel, it->second, _loops);
		}
		ALC_DEBUG_ERROR("Could not play sound file. It was not found. " + file);
	}

	void SoundSystem::PauseMusic() {
		if (Mix_PlayingMusic() != 0) {
			Mix_PauseMusic();
		}
	}

	void SoundSystem::ResumeMusic() {
		if (Mix_PausedMusic() != 0) {
			Mix_ResumeMusic();
		}
	}

	bool SoundSystem::LoadMusic(const string& file) {
		if (g_music.find(file) != g_music.end()) {
			return true;
		}

		Mix_Music* music = Mix_LoadMUS(file.c_str());
		if (music != nullptr) {
			g_music.emplace(file, music);
			return true;
		}

		ALC_DEBUG_ERROR("Could not load music file at " + file);
		return false;
	}

	bool SoundSystem::LoadSound(const string& file) {
		if (g_sounds.find(file) != g_sounds.end()) {
			return true;
		}

		Mix_Chunk* sound = Mix_LoadWAV(file.c_str());
		if (sound != nullptr) {
			g_sounds.emplace(file, sound);
			return true;
		}

		ALC_DEBUG_ERROR("Could not load sound file at " + file);
		return false;
	}

	void SoundSystem::UnloadMusic(const string& file) {
		auto mus = g_music.find(file);
		if (mus != g_music.end()) {
			Mix_FreeMusic(mus->second);
			g_music.erase(mus);
		}
	}

	void SoundSystem::UnloadSound(const string& file) {
		auto sou = g_sounds.find(file);
		if (sou != g_sounds.end()) {
			Mix_FreeChunk(sou->second);
			g_sounds.erase(sou);
		}
	}

}