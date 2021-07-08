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
#include "SceneManager.hpp"
#include "../Input\detail\SystemEvents.hpp"
//#include "../Jobs/Jobs.hpp"
#include "../Rendering/SpriteBatch.hpp"

namespace ALC {

	EngineSettings SceneManager::s_settings;
	uint32 SceneManager::s_levelToLoad = -1;
	IScene* SceneManager::s_activeScene = nullptr;
	uint32 SceneManager::s_currentSceneIndex = -1;
	Game* SceneManager::s_activeGame = nullptr;
	Window* SceneManager::s_window = nullptr;
	Timer* SceneManager::s_timer = nullptr;
	FT_Library SceneManager::s_fontLib = nullptr;
	bool SceneManager::s_isRunning = false;
	bool SceneManager::s_shouldQuit = false;

	void SceneManager::StartGame(Game* game_) {
		if (s_settings.scenes.bindings.size() == 0) {
			ALC_DEBUG_FATAL_ERROR("No scenes were bound");
			return;
		}

		// setup jobs
		//if (s_settings.jobsystem.enable) JobQueue::__Init(s_settings.jobsystem.threadcount);

		// setup variables
		s_levelToLoad = -1;
		s_activeGame = game_;
		s_shouldQuit = false;
		Timer timer;
		s_timer = &timer;
		if (s_activeGame) s_activeGame->__Initialize(s_window, &timer);
		SpriteBatch::__Init();
		s_currentSceneIndex = s_settings.scenes.startScene;
		s_activeScene = s_settings.scenes.bindings[s_currentSceneIndex]();
		if (FT_Init_FreeType(&s_fontLib)) {
			ALC_DEBUG_ERROR("Font library did not initialized.");
			return;
		}

		// init
		try {
			if (s_activeGame) s_activeGame->Init();
			s_activeScene->Init();
		}
		// catch any errors and quit
		catch (const std::exception& e) {
			ALC_DEBUG_FATAL_ERROR(e.what());
			delete s_activeScene;
			return;
		}

		while (s_isRunning && !s_shouldQuit) {
			timer.BeginFrame();

			if (s_levelToLoad != -1) {
				if (s_levelToLoad < s_settings.scenes.bindings.size()) {
					s_activeScene->Exit();
					delete s_activeScene;
					s_currentSceneIndex = s_levelToLoad;
					s_activeScene = s_settings.scenes.bindings[s_levelToLoad]();
					s_activeScene->Init();
				}
				s_levelToLoad = -1;
			}

			detail::SystemEvents::PollEvents();

			Timestep t = timer.GetTimestep();
			if (s_activeGame) s_activeGame->Step(t);
			s_activeScene->Step(t);

			if (s_activeGame) s_activeGame->PreDraw();
			s_activeScene->PreDraw();

			if (s_activeGame) s_activeGame->Draw();
			s_activeScene->Draw();

			if (s_activeGame) s_activeGame->PostDraw();
			s_activeScene->PostDraw();

			timer.EndFrame();
		}

		// exit
		try {
			if (s_activeGame) s_activeGame->Exit();
			s_activeScene->Exit();
		} catch (const std::exception& e) {
			ALC_DEBUG_FATAL_ERROR(e.what());
		}

		// cleanup
		SpriteBatch::__Exit();
		//if (s_settings.jobsystem.enable) JobQueue::__Exit();
		FT_Done_FreeType(s_fontLib);
		delete s_activeScene, s_activeScene = nullptr;
	}

}