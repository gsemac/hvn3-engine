#pragma once
#include "hvn3/gui2/WidgetRendererRenderArgs.h"
#include "hvn3/math/MathUtils.h"
#include <utility>

namespace hvn3 {
	namespace Gui {

		WidgetRendererRenderArgs::WidgetRendererRenderArgs() {
			_last_state = static_cast<WidgetState>(0);
			_initialized = false;
		}

		WidgetState WidgetRendererRenderArgs::LastState() const {
			return _last_state;
		}
		void WidgetRendererRenderArgs::SetLastState(WidgetState value) {
			_last_state = value;
		}
		bool WidgetRendererRenderArgs::Initialized() const {
			return _initialized;
		}

		const WidgetRendererRenderArgs::TransitionData<Color>* WidgetRendererRenderArgs::GetColorTransitionData(WidgetProperty prop) const {
			auto it = _color_transitions.find(prop);
			if (it == _color_transitions.end())
				return nullptr;
			return &(it->second);
		}
		const WidgetRendererRenderArgs::TransitionData<float>* WidgetRendererRenderArgs::GetFloatTransitionData(WidgetProperty prop) const {
			auto it = _float_transitions.find(prop);
			if (it == _float_transitions.end())
				return nullptr;
			return &(it->second);
		}
		void WidgetRendererRenderArgs::SetColorTransitionData(WidgetProperty prop, const Color& to, float duration) {
			auto it = _color_transitions.find(prop);
			if (it == _color_transitions.end())
				it = _color_transitions.insert(std::make_pair(prop, TransitionData<Color>())).first;
			it->second.from = it->second.to;
			it->second.to = to;
			it->second.progress = 0.0f;
			it->second.duration = duration;
			_initialized = true;
		}
		void WidgetRendererRenderArgs::SetColorTransitionData(WidgetProperty prop, const Color& to, const Color& from, float duration) {
			auto it = _color_transitions.find(prop);
			if (it == _color_transitions.end())
				it = _color_transitions.insert(std::make_pair(prop, TransitionData<Color>())).first;
			it->second.to = to;
			it->second.from = from;
			it->second.progress = 0.0f;
			it->second.duration = duration;
			_initialized = true;
		}
		void WidgetRendererRenderArgs::SetFloatTransitionData(WidgetProperty prop, float to, float duration) {
			auto it = _float_transitions.find(prop);
			if (it == _float_transitions.end())
				it = _float_transitions.insert(std::make_pair(prop, TransitionData<float>())).first;
			it->second.to = to;
			it->second.progress = 0.0f;
			it->second.duration = duration;
			_initialized = true;
		}
		void WidgetRendererRenderArgs::SetFloatTransitionData(WidgetProperty prop, float to, float from, float duration) {
			auto it = _float_transitions.find(prop);
			if (it == _float_transitions.end())
				it = _float_transitions.insert(std::make_pair(prop, TransitionData<float>())).first;
			it->second.to = to;
			it->second.from = from;
			it->second.progress = 0.0f;
			it->second.duration = duration;
			_initialized = true;
		}
		void WidgetRendererRenderArgs::UpdateTransitionData(float delta) {
			for (auto i = _color_transitions.begin(); i != _color_transitions.end(); ++i)
				i->second.progress = Math::Min(i->second.progress + delta, i->second.duration);
			for (auto i = _float_transitions.begin(); i != _float_transitions.end(); ++i)
				i->second.progress = Math::Min(i->second.progress + delta, i->second.duration);
		}

	}
}