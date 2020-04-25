#pragma once
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/gui/GuiDefs.h"
#include "hvn3/utility/BitFlags.h"
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class WidgetStyle {

		public:
			struct BackgroundPositionProperty;

		private:
			template<WidgetProperty T>
			struct WidgetPropertyTraits {
				typedef void type;
			};
			template<>
			struct WidgetPropertyTraits<WidgetProperty::BackgroundImage> {
				typedef Graphics::Bitmap type;
			};
			template<>
			struct WidgetPropertyTraits<WidgetProperty::BackgroundPosition> {
				typedef BackgroundPositionProperty type;
			};

		public:
			enum class PositionFlags {
				None = 0,
				Left = 1,
				Right = 2,
				Top = 4,
				Bottom = 8,
				Center = 16
			};

			struct BackgroundPositionProperty {
				BackgroundPositionProperty();
				PositionFlags flags;
				PointF position;
			};

			WidgetStyle() = default;

			template<WidgetProperty T>
			void SetProperty(const typename WidgetPropertyTraits<T>::type&) {}
			template<>
			void SetProperty<WidgetProperty::BackgroundImage>(const typename WidgetPropertyTraits<WidgetProperty::BackgroundImage>::type& value) {
				_bitmaps[WidgetProperty::BackgroundImage] = value;
			}
			template<>
			void SetProperty<WidgetProperty::BackgroundPosition>(const typename WidgetPropertyTraits<WidgetProperty::BackgroundPosition>::type& value) {
				_background_position = value;
			}

			template<WidgetProperty T>
			const typename WidgetPropertyTraits<T>::type* GetProperty() const {
		
				const void* ptr = nullptr;

				switch (T) {
				case WidgetProperty::BackgroundImage:
					ptr = _findBitmap(WidgetProperty::BackgroundImage);
					break;
				case WidgetProperty::BackgroundPosition:
					ptr = &_background_position;
					break;
				}

				return static_cast<const typename WidgetPropertyTraits<T>::type*>(ptr);

			}	

		private:
			const Graphics::Bitmap* _findBitmap(WidgetProperty p) const;

			std::unordered_map<WidgetProperty, Graphics::Bitmap> _bitmaps;
			std::unordered_map<WidgetProperty, PointF> _points_f;
			BackgroundPositionProperty _background_position;

		};

		HVN3_ENABLE_BITFLAGS(WidgetStyle::PositionFlags)

	}
}