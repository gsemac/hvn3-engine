#pragma once

#include "hvn3/events/IUpdateListener.h"

namespace hvn3 {

	class UpdateListenerBase :
		public IUpdateListener {

	public:
		void OnBeginUpdate(UpdateEventArgs& e) override {}
		void OnUpdate(UpdateEventArgs& e) override {}
		void OnEndUpdate(UpdateEventArgs& e) {}

	};

}