#include "hvn3/utility/Thread.h"
#include <chrono>
#include <thread>

namespace hvn3 {
	namespace Threading {

		void Thread::Sleep(long milliseconds) {

			std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

		}

	}
}