#include "StyleManager.h"
#include "Font.h"

namespace hvn3 {

	namespace Gui {

		StyleManager::StyleManager() :
			_primary_color(Color::FromArgb(35, 35, 35)),
			_secondary_color(Color::FromArgb(186, 186, 186)),
			_draw_scale(1.0f) {
		}
		StyleManager::~StyleManager() {

			_fonts.Clear();
			_bitmaps.Clear();

		}
		const Color& StyleManager::PrimaryColor() const {

			return _primary_color;

		}
		const Color& StyleManager::SecondaryColor() const {

			return _secondary_color;

		}
		ResourceHandle<Font> StyleManager::GetFontResource(FontResourceId id) {

			// Get a resource handle for the requested resource.
			ResourceHandle<Font> resource = _fonts.Find((ResourceId)id);

			// If the resource is null, load the default resource, and get a handle for it.
			if (!resource) {
				LoadDefaultFontResource(id);
				resource = _fonts[(ResourceId)id];
			}

			// Return the resource.
			return resource;

		}
		ResourceHandle<Drawing::Bitmap> StyleManager::GetImageResource(BitmapResourceId id) {

			// Get a resource handle for the requested resource.
			ResourceHandle<Drawing::Bitmap> resource = _bitmaps.Find((ResourceId)id);

			// If the resource is null, load the default resource, and get a handle for it.
			if (!resource) {
				LoadDefaultBitmapResource(id);
				resource = _bitmaps[(ResourceId)id];
			}

			// Return the resource.
			return resource;

		}

		float StyleManager::DrawingScale() const {

			return _draw_scale;

		}
		void StyleManager::SetDrawingScale(float scale) {

			_draw_scale = scale;

		}

		// Protected members

		bool StyleManager::LoadDefaultFontResource(FontResourceId id) {

			ResourceId resId = (ResourceId)id;

			switch (id) {

			case FontResourceId::PrimaryFont:
				_fonts.Add(resId, Resource::Create<Font>("data/System/Fonts/webly.ttf", 11, FontOptions::Monochrome));
				break;
			default:
				return false;
			}

			return true;

		}
		bool StyleManager::LoadDefaultBitmapResource(BitmapResourceId id) {

			ResourceId resId = (ResourceId)id;

			switch (id) {

			case BitmapResourceId::ExitButton:
				_bitmaps.Add(resId, Resource::Create<Drawing::Bitmap>("data/System/Icons/close.png"));
				break;
			default:
				return false;
			}

			return true;

		}

	}

}