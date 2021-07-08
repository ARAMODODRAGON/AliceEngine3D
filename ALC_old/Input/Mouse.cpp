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
#include "Mouse.hpp"

namespace ALC {

	Mouse::MouseState Mouse::mouseState;

	Mouse::MouseState::MouseState()
		: allStatesChanges(0) { }

	void Mouse::__SetNewPosition(const ivec2& pos) {
		mouseState.oldPos = mouseState.newPos;
		mouseState.newPos = pos;
		mouseState.posDiff = mouseState.newPos - mouseState.oldPos;
	}

	void Mouse::__SetScrollPos(const float scrollpos) { 
		mouseState.oldScrollPos = mouseState.scrollPos;
		mouseState.scrollPos = scrollpos;
		mouseState.scrollDelta = mouseState.scrollPos - mouseState.oldScrollPos;
	}

	void Mouse::__SetButtons(bool left, bool right, bool middle) {
		if (mouseState.leftState != left) mouseState.leftChanged = true;
		else mouseState.leftChanged = false;
		mouseState.leftState = left;

		if (mouseState.rightState != right) mouseState.rightChanged = true;
		else mouseState.rightChanged = false;
		mouseState.rightState = right;

		if (mouseState.middleState != middle) mouseState.middleChanged = true;
		else mouseState.middleChanged = false;
		mouseState.middleState = middle;
	}

}