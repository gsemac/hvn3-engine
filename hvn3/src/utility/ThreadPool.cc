#include "utility/ThreadPool.h"

namespace hvn3 {
	namespace Threading {

		ThreadPool::ThreadPool() :
			ThreadPool(std::max(std::thread::hardware_concurrency(), 1U)) {
		}
		ThreadPool::ThreadPool(unsigned int max_threads) {

			_thread_count.store(max_threads);
			_quit.store(false);
			_tasks_remaining.store(0);
			_active_tasks.store(0);

			_createThreads();

		}

		void ThreadPool::AddTask(task_type task) {

			std::lock_guard<std::mutex> guard(_tasks_mutex);

			_tasks.emplace(task);

			++_tasks_remaining;

			_task_available.notify_one();

		}
		void ThreadPool::WaitAll() {

			if (_tasks_remaining.load() <= 0)
				return;

			std::unique_lock<std::mutex> lock(_wait_mutex);
			_waiting_cond.wait(lock, [this] { return _tasks_remaining <= 0; });

		}
		void ThreadPool::JoinAll() {

			WaitAll();

			// Notify all threads waiting for a task so they can terminate.
			_quit.store(true);
			_task_available.notify_all();

			for (auto i = _threads.begin(); i != _threads.end(); ++i)
				if (i->joinable())
					i->join();

			// After all threads have been joined, they are cleared. No more tasks can be executed by the pool after this.
			_threads.clear();

		}

		unsigned int ThreadPool::TasksRemaining() const {

			return _tasks_remaining;

		}
		unsigned int ThreadPool::ActiveTasks() const {

			return _active_tasks;

		}
		unsigned int ThreadPool::ThreadCount() const {

			return _thread_count.load();

		}



		void ThreadPool::_worker() {

			while (!_quit.load()) {

				task_type task = _getNextTask();

				++_active_tasks;

				task();

				--_tasks_remaining;
				--_active_tasks;

				_waiting_cond.notify_one();

			}

		}
		void ThreadPool::_createThreads() {

			for (unsigned int i = 0; i < _thread_count; ++i)
				_threads.push_back(std::thread([this] { this->_worker(); }));

		}
		ThreadPool::task_type ThreadPool::_getNextTask() {

			std::unique_lock<std::mutex> lock(_tasks_mutex);

			_task_available.wait(lock, [this] { return _tasks.size() > 0 || _quit.load(); });

			if (_quit.load()) {
				// The number of tasks remaining will be decremented after completing this empty task.
				++_tasks_remaining;
				return [] {};
			}
			task_type task = _tasks.front();
			_tasks.pop();

			return task;

		}

	}
}