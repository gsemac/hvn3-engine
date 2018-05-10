#pragma once
#include "hvn3/gui2/WidgetBase.h"
#include "hvn3/math/MathUtils.h"

namespace hvn3 {
	namespace Gui {

		class Scrollbar : public WidgetBase {

			class Thumb : public WidgetBase {

			public:
				Thumb(Scrollbar* scrollbar) :
					_scrollbar(scrollbar) {

					SetIdentifier("thumb");
					_offset = 0.0f;

				}

				void OnUpdate(WidgetUpdateEventArgs& e) override {

					if (_scrollbar == nullptr)
						return;



					// Set the thumb according to the state of the scrollbar.

					if (_scrollbar->Orientation() == Orientation::Horizontal) {

						SetHeight(_scrollbar->Height());
						SetPosition(_scrollbar->X() + ((_scrollbar->Width() - Width()) * _scrollbar->ValuePercent()), _scrollbar->Y());

					}
					else {

						SetWidth(_scrollbar->Width());
						SetPosition(_scrollbar->X(), _scrollbar->Y() + ((_scrollbar->Height() - Height()) * _scrollbar->ValuePercent()));

					}

				}

			private:
				Scrollbar* _scrollbar;
				float _offset;

			};

		public:
			Scrollbar(float length, float max, Orientation orientation) :
				WidgetBase(0.0f, 0.0f, 20.0f, length) {

				SetIdentifier("scrollbar");
				_value = 0.0f;
				_max_scroll = max;
				_orientation = orientation;
				_thumb = nullptr;

			}

			float Max() const {
				return _max_scroll;
			}
			void SetMax(float value) {
				_max_scroll = value;
			}
			float Value() const {
				return _value;
			}
			void SetValue(float value) {
				_value = value;
			}
			float ValuePercent() const {
				return (_value / _max_scroll);
			}
			float SetValuePercent(float value) {

				value = Math::Clamp(value, 0.0f, 1.0f);

				_value = _max_scroll * value;

			}
			Orientation Orientation() const {
				return _orientation;
			}

			void OnManagerChanged(WidgetManagerChangedEventArgs& e) override {

				// If the new manager is null, do nothing but destroy the thumb (if it exists).
				if (GetManager() == nullptr && _thumb != nullptr) {

					if (e.OldManager() != nullptr)
						e.OldManager()->Remove(_thumb);

					_thumb = nullptr;

					return;

				}

				// If the thumb already exists, move it to the new manager.
				if (_thumb != nullptr) {
					e.OldManager()->Move(_thumb, GetManager());
					return;
				}

				// If the thumb does not already exist, we need to create it and add it to the new manager.
				if (GetManager() != nullptr) {
					_thumb = new Thumb(this);
					GetManager()->Add(_thumb);
				}

			}

		private:
			float _value;
			float _max_scroll;
			Gui::Orientation _orientation;
			Thumb* _thumb;

		};

	}
}