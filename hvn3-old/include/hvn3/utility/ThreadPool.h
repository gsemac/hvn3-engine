#pragma once
#include <algorithm>
#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace hvn3 {
	namespace Threading {

		class ThreadPool {

		public:
			typedef std::function<void()> task_type;

			// Instantiates a thread pool using the hardware-defined core count.
			ThreadPool();
			~ThreadPool();
			// Instantiates a thread pool with the given number of threads.
			ThreadPool(unsigned int max_threads);

			// Adds a new task to the thread pool.
			void AddTask(task_type task);
			// Blocks the calling thread until all tasks have completed.
			void WaitAll();
			// Joins all task threads to the calling thread.
			void JoinAll();

			// Returns the number of unexecuted tasks.
			unsigned int TasksRemaining() const;
			// Returns the number of tasks currently executing.
			unsigned int ActiveTasks() const;
			// Returns the maximum number of concurrent threads.
			unsigned int ThreadCount() const;

		private:
			std::queue<task_type> _tasks;
			std::vector<std::thread> _threads;
			std::condition_variable _task_available;
			std::condition_variable _waiting_cond;

			std::atomic<unsigned int> _thread_count;
			std::atomic<unsigned int> _tasks_remaining;
			std::atomic<unsigned int> _active_tasks;
			std::atomic<bool> _quit;

			mutable std::mutex _tasks_mutex;
			mutable std::mutex _wait_mutex;

			void _worker();
			void _createThreads();
			task_type _getNextTask();

		};

	}
}