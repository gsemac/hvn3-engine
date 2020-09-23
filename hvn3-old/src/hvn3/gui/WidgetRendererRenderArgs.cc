#pragma once
#include "hvn3/gui/WidgetRenderArgs.h"
#include "hvn3/math/MathUtils.h"
#include <utility>

namespace hvn3 {
	namespace Gui {

		WidgetRenderArgs::WidgetRenderArgs() {
			_last_state = static_cast<WidgetState>(0);
			_initialized = false;
		}

		WidgetState WidgetRenderArgs::LastState() const {
			return _last_state;
		}
		void WidgetRenderArgs::SetLastState(WidgetState value) {
			_last_state = value;
		}
		bool WidgetRenderArgs::Initialized() const {
			return _initialized;
		}

		void WidgetRenderArgs::UpdateTransitionData(float delta) {
			for (auto i = _color_transitions.begin(); i != _color_transitions.end(); ++i)
				i->second.progress = Math::Min(i->second.progress + delta, i->second.duration);
			for (auto i = _float_transitions.begin(); i != _float_transitions.end(); ++i)
				i->second.progress = Math::Min(i->second.progress + delta, i->second.duration);
			for (auto i = _point_transitions.begin(); i != _point_transitions.end(); ++i)
				i->second.progress = Math::Min(i->second.progress + delta, i->second.duration);
		}

	}
}