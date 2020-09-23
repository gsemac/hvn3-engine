#pragma once
#include <hvn3/exceptions/Exception.h>
#include "hvn3/gui/GuiDefs.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/graphics/Tween.h"
#include "hvn3/math/Point2d.h"
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class WidgetRenderArgs {

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

		private:
			template<typename T>
			std::unordered_map<WidgetProperty, TransitionData<T>>* _getPropertyTransitionMap() {
				throw NotImplementedException();
			}

			template<>
			std::unordered_map<WidgetProperty, TransitionData<Color>>* _getPropertyTransitionMap() {
				return &_color_transitions;
			}
			template<>
			std::unordered_map<WidgetProperty, TransitionData<float>>* _getPropertyTransitionMap() {
				return &_float_transitions;
			}
			template<>
			std::unordered_map<WidgetProperty, TransitionData<PointF>>* _getPropertyTransitionMap() {
				return &_point_transitions;
			}

		public:

			WidgetRenderArgs();

			WidgetState LastState() const;
			void SetLastState(WidgetState value);
			bool Initialized() const;

			void UpdateTransitionData(float delta);

			template <typename T>
			const TransitionData<T>* GetTransitionData(WidgetProperty prop) {
				auto _map = _getPropertyTransitionMap<T>();
				auto it = _map->find(prop);
				if (it == _map->end())
					return nullptr;
				return &(it->second);
			}
			template <typename T>
			void SetTransitionData(WidgetProperty prop, const T& to, float duration) {
				auto _map = _getPropertyTransitionMap<T>();
				auto it = _map->find(prop);
				if (it == _map->end())
					it = _map->insert(std::make_pair(prop, TransitionData<T>())).first;
				it->second.from = Graphics::TweenTraits<T>::Interpolate(it->second.from, it->second.to, it->second.Percentage(), Graphics::EasingFunction::Linear);
				it->second.to = to;
				it->second.progress = 0.0f;
				it->second.duration = duration;
				_initialized = true;
			}
			template <typename T>
			void SetTransitionData(WidgetProperty prop, const T& to, const T& from, float duration) {
				auto _map = _getPropertyTransitionMap<T>();
				auto it = _map->find(prop);
				if (it == _map->end())
					it = _map->insert(std::make_pair(prop, TransitionData<T>())).first;
				it->second.to = to;
				it->second.from = from;
				it->second.progress = 0.0f;
				it->second.duration = duration;
				_initialized = true;
			}

		private:
			std::unordered_map<WidgetProperty, TransitionData<Color>> _color_transitions;
			std::unordered_map<WidgetProperty, TransitionData<float>> _float_transitions;
			std::unordered_map<WidgetProperty, TransitionData<PointF>> _point_transitions;

			WidgetState _last_state;
			bool _initialized;

		};

	}
}