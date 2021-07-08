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
#ifndef ALC_INPUT_MOUSE_HPP
#define ALC_INPUT_MOUSE_HPP
#include "../General.hpp"
#include "Button.hpp"

namespace ALC {

	class Mouse {
		ALC_NON_CONSTRUCTABLE(Mouse);
	public:

		static Button GetLeft() { return Button(mouseState.leftState, mouseState.leftChanged); }
		static Button GetRight() { return Button(mouseState.rightState, mouseState.rightChanged); }
		static Button GetMiddle() { return Button(mouseState.middleState, mouseState.middleChanged); }
		static vec2 GetPosition() { return mouseState.newPos; }
		static vec2 GetAxis() { return mouseState.posDiff; }
		static float GetScrollDelta() { return mouseState.scrollDelta; }

	public:
		static void __SetNewPosition(const ivec2& pos);
		static void __SetScrollPos(const float scrollpos);
		static void __SetButtons(bool left, bool right, bool middle);
		struct MouseState {
			union {
				struct {
					uint8 leftState : 1;
					uint8 leftChanged : 1;
					uint8 rightState : 1;
					uint8 rightChanged : 1;
					uint8 middleState : 1;
					uint8 middleChanged : 1;
				};
				uint8 allStatesChanges;
			};
			vec2 oldPos;
			vec2 newPos;
			vec2 posDiff;
			float scrollPos;
			float oldScrollPos;
			float scrollDelta;
			MouseState();
		};
		static MouseState mouseState;
	};

}

#endif // !ALC_INPUT_MOUSE_HPP