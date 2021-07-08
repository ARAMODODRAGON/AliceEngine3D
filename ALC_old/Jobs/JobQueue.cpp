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
#include "JobQueue.hpp"

namespace ALC {

	namespace {

		Scope<cjs::context> g_queue;
		size_t g_threadCount;

	}

	void JobQueue::Submit(IJob* job) {
		#ifdef _DEBUG
		if (g_queue.get() == nullptr) {
			ALC_DEBUG_ERROR("JobSystem has not been enabled!");
			return;
		}
		#endif
		g_queue->submit(job);
	}

	void JobQueue::Submit(Handle* handle) {
		#ifdef _DEBUG
		if (g_queue.get() == nullptr) {
			ALC_DEBUG_ERROR("JobSystem has not been enabled!");
			return;
		}
		#endif
		g_queue->submit(handle);
	}

	uint32 JobQueue::GetWorkerCount() {
		return g_threadCount;
	}

	void JobQueue::__Init(uint32 threadcount) {
		// garuntees that there are at least 1 thread if none are specified
		if (threadcount == 0)
			threadcount = glm::max(int(std::thread::hardware_concurrency() - 1), 1);
		g_threadCount = threadcount;
		g_queue.reset(new cjs::context(threadcount));
	}

	void JobQueue::__Exit() {
		g_queue.reset();
	}

}