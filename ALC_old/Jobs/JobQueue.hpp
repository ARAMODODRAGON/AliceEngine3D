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
#ifndef ALC_JOBS_JOBQUEUE_HPP
#define ALC_JOBS_JOBQUEUE_HPP
#include "../General.hpp"
#include "cjs\cjs.hpp"

namespace ALC {

	// cleans up the usage of ijob and quick_fence to be included under the ALC namespace
	using IJob = cjs::ijob;
	using Handle = cjs::handle;

	class JobQueue final {
		ALC_NON_CONSTRUCTABLE(JobQueue);
	public:

		// submits a job to be worked on
		static void Submit(IJob* job);
		
		// submits a job to be worked on
		static void Submit(Handle* handle);

		// returns the number of active workers
		static uint32 GetWorkerCount();

	public:
		static void __Init(uint32 threadcount);
		static void __Exit();
	};

}

#endif // !ALC_JOBS_JOBQUEUE_HPP