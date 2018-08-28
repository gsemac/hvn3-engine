#pragma once
#include "hvn3/io/DisplayEventArgs.h"

namespace hvn3 {

	class IDisplayListener {

	public:
		virtual void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) = 0;
		virtual void OnDisplayLost(DisplayLostEventArgs& e) = 0;
		virtual void OnDisplayFound(DisplayFoundEventArgs& e) = 0;

	};

}