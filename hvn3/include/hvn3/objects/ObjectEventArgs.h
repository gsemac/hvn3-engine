#pragma once
#include "hvn3/core/Context.h"
#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class CreateEventArgs :
		public System::EventArgs {

	public:
		CreateEventArgs(const Context& context) {
			_context = context;
		}

		const Context& Context() const {
			return _context;
		}

	private:
		class Context _context;

	};

	class DestroyEventArgs : public System::EventArgs {
	};

	class AnimationEndEventArgs :
		public System::EventArgs {

	public:
		AnimationEndEventArgs(int endIndex) {
			_end_index = endIndex;
		}
		int EndIndex() const {
			return _end_index;
		}

	private:
		int _end_index;

	};

}