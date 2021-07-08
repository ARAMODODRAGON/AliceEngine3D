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
#ifndef ALC_DATATYPES_LAYERMASK_HPP
#define ALC_DATATYPES_LAYERMASK_HPP
#include "../General.hpp"

namespace ALC {

	template<typename I>
	struct Layermask {

		// mask with all layers set
		static const Layermask<I> ALL;

		// mask with no layers set
		static const Layermask<I> NONE;

		// constructors / operators
		Layermask() : mask(0) { }
		Layermask(const I mask_) : mask(mask_) { }
		Layermask& operator=(const I mask_) {
			mask = mask_;
			return *this;
		}

		// operators

		// cast to unsigned int
		explicit operator I() const { return mask; }

		// checks if the masks match exactly
		bool operator==(const Layermask& other) const { return mask == other.mask; }

		// checks if the masks dont match exactly
		bool operator!=(const Layermask& other) const { return mask != other.mask; }

		// returns true if *any* layers match
		bool operator&&(const Layermask& other) const { return mask & other.mask; }

		// returns a layermask with only the matching layers
		Layermask operator&(const Layermask& other) const { return Layermask(mask & other.mask); }
		Layermask& operator&=(const Layermask& other) {
			mask &= other.mask;
			return *this;
		}

		// used to check if any layers are set
		operator bool() const { return mask != 0; }

		// creates a new layermask with the layers of the two old ones
		Layermask operator|(const Layermask& other) const { return Layermask(mask | other.mask); }
		Layermask& operator|=(const Layermask& other) {
			mask |= other.mask;
			return *this;
		}

		// returns a reversed layermask
		Layermask operator~() const { return Layermask(~mask); }

		// function to set/unset layers
		void SetLayer(const uint32 layer, bool value) {
			if (value)	mask |= (1 << layer);  // set true
			else		mask &= ~(1 << layer); // set false
		}

		// function to get layer
		bool GetLayer(const uint32 layer) const {
			return mask & (1 << layer);
		}

	private:

		// 32 layers of bools essentially
		I mask;

	};


	template<typename I>
	const Layermask<I> Layermask<I>::ALL = ~static_cast<I>(0);
	template<typename I>
	const Layermask<I> Layermask<I>::NONE = static_cast<I>(0);


	// 64 layers
	using Layermask64 = Layermask<uint64>;
	// 32 layers
	using Layermask32 = Layermask<uint32>;
	// 16 layers
	using Layermask16 = Layermask<uint16>;
	// 8 layers
	using Layermask8 = Layermask<uint8>;

}

#endif // !ALC_DATATYPES_LAYERMASK_HPP