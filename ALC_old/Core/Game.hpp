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
#ifndef ALC_CORE_GAME_HPP
#define ALC_CORE_GAME_HPP
#include "Window.hpp"
#include "Timer.hpp"

namespace ALC {

	class Game {
	public:

		Game();
		~Game();

		virtual void Init() = 0;
		virtual void Exit() = 0;
		virtual void Step(Timestep t) = 0;
		virtual void PreDraw() = 0;
		virtual void Draw() = 0;
		virtual void PostDraw() = 0;

		Window* GetWindow() { return m_window; }
		Timer* GetTimer() { return m_timer; }

	public:
		void __Initialize(Window* window_, Timer* timer_);
	private:
		Window* m_window;
		Timer* m_timer;
	};

}

#endif // !ALC_CORE_GAME_HPP