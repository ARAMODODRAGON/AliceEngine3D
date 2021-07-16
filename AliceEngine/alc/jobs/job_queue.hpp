#ifndef ALC_JOB_QUEUE_HPP
#define ALC_JOB_QUEUE_HPP
#include "../common.hpp"
#include "cjs\cjs.hpp"

namespace alc {

	using cjs::handle;
	using cjs::context;
	using cjs::ijob;

	class job_queue {
		ALC_STATIC_CLASS(job_queue);
	public:

		// checks if the job queue is enabled
		static bool is_enabled();

		// submits the job to the queue
		static void submit(ijob* ijob_);

		// submits the handle to the queue
		static void submit(handle* handle_);

	private:
		static inline std::unique_ptr<context> s_context = nullptr;
	public:
		static void __init(size_t maxthreads);
		static void __exit();
	};

}

#endif // !ALC_JOB_QUEUE_HPP