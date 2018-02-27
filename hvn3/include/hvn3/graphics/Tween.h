#pragma once
#include "hvn3/graphics/Color.h"
#include <type_traits>

namespace hvn3 {

	namespace Graphics {
		enum class TweenFunction {
			Linear
		};
	}

	namespace System {
		namespace Graphics {
			template<typename T>
			struct TweenTraits {
				static T Interpolate(T from, T to, float progress, hvn3::Graphics::TweenFunction func) {
					switch (func) {
					case hvn3::Graphics::TweenFunction::Linear:
						return from + progress * (to - from);
						break;
					default:
						return from;
					}
				}
			};
			template<>
			struct TweenTraits<Color> {
				static Color Interpolate(const Color& from, const Color& to, float progress, hvn3::Graphics::TweenFunction func) {
					// At the moment, colors only use linear interpolation.
					return Color::Merge(from, to, progress);
				}
			};
		}
	}

	namespace Graphics {

		template <typename EasingFromType, typename EasingToType = EasingFromType>
		class Tween {

		public:
			Tween(EasingFromType from, EasingToType to) :
				Tween(from, to, 10) {
			}
			Tween(EasingFromType from, EasingToType to, int duration) :
				Tween(from, to, duration, TweenFunction::Linear) {
			}
			Tween(EasingFromType from, EasingToType to, int duration, TweenFunction func) :
				_from(from), _to(to) {
				_step = 0;
				_duration = duration;
				_func = func;
			}

			const EasingFromType Value() const {
				return System::Graphics::TweenTraits<EasingFromType>::Interpolate(_from, _to, Progress(), _func);
			}

			bool Step() {
				return Step(1);
			}
			bool Step(int amount) {

				// This is checked before adjusting the step so that it doesn't return true until the user has had a chance
				// to react to the final step. By returning a bool, this method can conveniently be called in loops.
				bool finished = _step >= _duration;

				if (-amount > _step)
					_step = 0;
				else if (_step + amount > _duration)
					_step = _duration;
				else
					_step += amount;

				return !finished;

			}

			float Progress() const {
				if (_step >= _duration)
					return 1.0f;
				return _step / static_cast<float>(_duration);
			}

		private:
			EasingFromType _from;
			EasingToType _to;
			int _step, _duration;
			TweenFunction _func;

		};

		/*

		One idea was to allow reference types to used as the EasingFromType, but in the end I decided that the added flexibility reduced flexibility in other areas
		(such as making it more difficult to allow for a Tween to have multiple "from" and "to" values), and it wasn't worth it. Here's what the implementation
		looked like before being scrapped; maybe it's worth returning to at some point.


		template <typename EasingFromType, typename EasingToType = EasingFromType>
		typename std::enable_if<!std::is_lvalue_reference<EasingFromType>::value, EasingFromType>::type Interpolate(EasingFromType from, EasingToType to, float amount, TweenFunction func) {
		...
		}
		template <typename EasingFromType, typename EasingToType = EasingFromType>
		typename std::enable_if<std::is_lvalue_reference<EasingFromType>::value, EasingFromType>::type Interpolate(EasingFromType from, EasingToType to, float amount, TweenFunction func) {
		from = Interpolate<typename std::remove_reference<EasingFromType>::type, EasingToType>(from, to, amount, func);
		return from;
		}

		namespace Implementation {
		template <typename EasingFromType>
		struct TweenBaseRef {
		EasingFromType _from;
		EasingFromType& _ref;
		};
		template <typename EasingFromType>
		struct TweenBaseVal {
		EasingFromType _from;
		};
		}

		template <typename EasingFromType, typename EasingToType = EasingFromType>
		class Tween : public std::conditional<std::is_lvalue_reference<EasingFromType>::value, Implementation::TweenBaseRef<EasingFromType>, Implementation::TweenBaseVal<EasingFromType>>::type {
		...
		};

		*/

	}
}