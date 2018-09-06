#pragma once 
#include "hvn3/io/IDisplayListener.h"
#include "hvn3/io/ListenerBase.h"

namespace hvn3 {

	namespace System {
		class Runner;
	}

	class DisplayListener :
		public ListenerBase<IDisplayListener> {

		friend class System::Runner;

	public:
		void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) override {}
		void OnDisplayLost(DisplayLostEventArgs& e) override {}
		void OnDisplayFound(DisplayFoundEventArgs& e) override {}

	};

}