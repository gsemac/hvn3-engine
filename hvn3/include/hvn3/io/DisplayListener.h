#pragma once 
#include "hvn3/io/IDisplayListener.h"
#include "hvn3/io/Listener.h"

namespace hvn3 {

	namespace System {
		class Runner;
	}

	class DisplayListener : 
		public System::Listener<DisplayListener>,
		public virtual IDisplayListener {

		friend class System::Runner;

	public:
		void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) override {}
		void OnDisplayLost(DisplayLostEventArgs& e) override {}
		void OnDisplayFound(DisplayFoundEventArgs& e) override {}

	};

}