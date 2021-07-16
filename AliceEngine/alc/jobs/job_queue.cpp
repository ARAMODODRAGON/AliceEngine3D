#include "job_queue.hpp"

namespace alc {

	bool job_queue::is_enabled() {
		return s_context.get() != nullptr;
	}

	void job_queue::submit(ijob* ijob_) {
		if (is_enabled()) s_context->submit(ijob_);
	}

	void job_queue::submit(handle* handle_) {
		if (is_enabled()) s_context->submit(handle_);
	}

	void job_queue::__init(size_t maxthreads) {
		if (is_enabled()) return;
		// get the number of available threads that doesn't include the current thread
		size_t curthreads = std::thread::hardware_concurrency() - 1;
		// max a context with the fewest number of threads
		s_context.reset(new context(curthreads < maxthreads ? curthreads : maxthreads));
	}

	void job_queue::__exit() {
		if (!is_enabled()) return;
		// delete the context
		s_context.reset();
	}

}