#pragma once 
#include "hvn3/io/IDisplayListener.h"
#include "hvn3/io/Listener.h"

namespace hvn3 {

	class DisplayListener : 
		public System::Listener<DisplayListener>,
		public virtual IDisplayListener {

		friend class Display;

	public:
		void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) override {}

	};

}