#pragma once
#include "hvn3/gui2/GuiTypeDefs.h"
#include <hvn3/graphics/Color.h>
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class WidgetRendererRenderArgs {

		public:
			template <typename T>
			struct TransitionData {
				T from;
				T to;
				float duration;
				float progress;
				float Percentage() const {
					if (duration == 0.0f)
						return 1.0f;
					return progress / duration;
				}
			};

			WidgetRendererRenderArgs();

			WidgetState LastState() const;
			void SetLastState(WidgetState value);
			bool Initialized() const;
		
			const TransitionData<Color>* GetColorTransitionData(WidgetProperty prop) const;
			const TransitionData<float>* GetFloatTransitionData(WidgetProperty prop) const;
			void SetColorTransitionData(WidgetProperty prop, const Color& to, float duration);
			void SetColorTransitionData(WidgetProperty prop, const Color& to, const Color& from, float duration);
			void SetFloatTransitionData(WidgetProperty prop, float to, float duration);
			void SetFloatTransitionData(WidgetProperty prop, float to, float from, float duration);
			void UpdateTransitionData(float delta);

		private:
			std::unordered_map<WidgetProperty, TransitionData<Color>> _color_transitions;
			std::unordered_map<WidgetProperty, TransitionData<float>> _float_transitions;

			WidgetState _last_state;
			bool _initialized;
		
		};

	}
}