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
#include "Timer.hpp"
#include "Debugger.hpp"

namespace ALC {

	Timer::Timer() : targetFPS(0), lastDelta(0.0) {
		SetTargetFPS(60);
		lastTime = currentTime = steady_clock::now();
	}

	Timer::~Timer() { }

	Timestep Timer::GetTimestep() const {
		return Timestep(lastDelta);
	}

	void Timer::BeginFrame() {
		lastDelta = (currentTime - lastTime).count();
		// save the previous time point
		lastTime = currentTime;
	}

	void Timer::EndFrame() { 
		currentTime = steady_clock::now();
	}

	void Timer::WaitForEndOfFrame() {
		// get new time
		time_point t = steady_clock::now();

		// loop as long as this frame is not done yet
		while ((t - lastTime).count() < secondsPerFrame) {
			t = steady_clock::now();
		}

	}

	bool Timer::CheckIfFrameComplete() {
		// check if this frame is done yet
		if ((steady_clock::now() - lastTime).count() > secondsPerFrame) {
			// assume that the caller wants to update time
			currentTime = steady_clock::now();
			return true;
		}
		return false;
	}

	float Timer::GetFPS() const {
		return static_cast<float>(1.0 / lastDelta);
	}

	float Timer::GetDelta() const {
		return static_cast<float>(lastDelta);
	}

}