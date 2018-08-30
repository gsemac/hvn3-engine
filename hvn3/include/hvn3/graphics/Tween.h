#pragma once
#include "hvn3/graphics/Color.h"
#include <cassert>
#include <cmath>
#include <functional>
#include <type_traits>
#include <vector>

namespace hvn3 {
	namespace Graphics {

		enum class EasingFunction {
			None,
			EaseOutSin,
			Linear
		};

		template<typename T>
		struct TweenTraits {
			static T Interpolate(const T& from, const T& to, float time, hvn3::Graphics::EasingFunction func) {

				float amount = 1.0f;

				time = Math::Clamp(time, 0.0f, 1.0f);

				switch (func) {

				case EasingFunction::None:
					amount = amount > 0.0f ? 1.0f : 0.0f;
					break;
				case EasingFunction::EaseOutSin:
					amount = static_cast<float>(std::sin(Math::Pi / 2.0 * time));
					break;
				case EasingFunction::Linear:
					amount = time;
					break;

				}

				return static_cast<T>(from + (amount * (to - from)));

			}
		};
		template<>
		struct TweenTraits<Color> {
			static Color Interpolate(const Color& from, const Color& to, float time, hvn3::Graphics::EasingFunction func) {

				// At the moment, colors only use linear interpolation.

				switch (func) {

				case EasingFunction::None:
					return to;
					break;
				case EasingFunction::Linear:
					return Color::Merge(from, to, time);
					break;
				default:
					return to;

				}

			}
		};

		template <typename T>
		class Tween {

			typedef int duration_type;
			typedef std::function<void(Tween<T>&)> callback_type;

			struct tween_data {

				tween_data() {
					duration = 1;
				}

				T from;
				T to;
				duration_type duration;
				callback_type _do;
			};

			typedef std::vector<tween_data> data_collection_type;

		public:
			template<typename T = T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
			Tween(T from, T to) :
				Tween(from, to, Math::Abs(to - from)) {
			}
			template<typename T = T, typename std::enable_if<!std::is_integral<T>::value, int>::type = 0>
			Tween(T from, T to) :
				Tween(from, to, 10) {
			}
			Tween(T from, T to, duration_type duration) :
				Tween(from, to, duration, EasingFunction::Linear) {
			}
			Tween(T from, T to, duration_type duration, EasingFunction easing_function) {

				From(from).To(to).During(duration);

				_step = 0;
				_data_index = 0;
				_easing_function = easing_function;
				_reverse = false;

			}

			const T Value() const {

				assert(_data_index >= 0);
				assert(_data_index < _data.size());

				const tween_data& data = _data[_data_index];

				return TweenTraits<T>::Interpolate(data.from, data.to, Progress(), _easing_function);

			}

			bool Step() {
				return Step(1);
			}
			bool Step(duration_type amount) {

				assert(_data_index >= 0);
				assert(_data_index < _data.size());

				if (_reverse)
					amount = -amount;

				tween_data& data = _data[_data_index];

				// This is checked before adjusting the step so that it doesn't return true until the user has had a chance
				// to react to the final step. By returning a bool, this method can conveniently be called in loops.
				bool finished;

				if (_reverse) {

					// If stepping in reverse, we're finished when we hit the beginning of the tween.
					finished = (_step == 0 && _data_index == 0);

					if (!finished && data._do)
						data._do(*this);

				}
				else
					finished = (_step >= data.duration) && (_data_index + 1 >= _data.size());

				if (!_reverse && _step >= data.duration && _data_index + 1 < _data.size()) {

					_step = 0;
					++_data_index;

				}
				else if (_reverse && _step == 0 && _data_index > 0) {
					_step = _data[--_data_index].duration;
				}
				else {

					if (-amount > _step)
						_step = 0;
					else if (_step + amount > data.duration)
						_step = data.duration;
					else
						_step += amount;

				}

				if (!_reverse && !finished && data._do)
					data._do(*this);

				return !finished;

			}
			void Reverse() {
				_reverse = !_reverse;
			}
			void SeekToBegin() {
				_step = 0;
				_data_index = 0;
			}
			void SeekToEnd() {

				if (_data.size() > 0) {
					_data_index = _data.size() - 1;
					_step = _data[_data_index].duration;
				}
				else {
					_step = 0;
					_data_index = 0;
				}

			}

			float Progress() const {

				assert(_data_index >= 0);
				assert(_data_index < _data.size());

				if (_step >= _data[_data_index].duration)
					return 1.0f;

				return _step / static_cast<float>(_data[_data_index].duration);

			}

			Tween<T>& From(T value) {

				tween_data data;
				data.from = value;
				data.to = value;
				data.duration = 1;

				_data.push_back(data);

				return *this;

			}
			Tween<T>& To(T value) {

				assert(_data.size() > 0);

				_data.back().to = value;

				return *this;

			}
			Tween<T>& During(duration_type value) {

				assert(_data.size() > 0);

				_data.back().duration = value;

				return *this;

			}
			Tween<T>& Wait(duration_type value) {

				assert(_data.size() > 0);

				tween_data data;
				data.from = _data.back().to;
				data.to = _data.back().to;
				data.duration = value;

				_data.push_back(data);

				return *this;

			}
			Tween<T>& Do(callback_type&& value) {

				assert(_data.size() > 0);

				_data.back()._do = std::move(value);

				return *this;

			}
			Tween<T>& Then(callback_type&& value) {

				assert(_data.size() > 0);

				tween_data data;
				data.from = _data.back().to;
				data.to = _data.back().to;
				data.duration = 0;
				data._do = std::move(value);

				_data.push_back(data);

				return *this;

			}
			Tween<T>& Using(EasingFunction easing_function) {

				_easing_function = easing_function;

				return *this;

			}

		private:
			duration_type _step;
			bool _reverse;
			typename data_collection_type::size_type _data_index;
			data_collection_type _data;
			EasingFunction _easing_function;

		};

		namespace tween {

			template<typename T>
			Tween<T> From(T value) {
				return Tween<T>(value, value);
			}

		}

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