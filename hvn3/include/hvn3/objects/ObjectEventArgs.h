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

}