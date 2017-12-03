#include "gui/StyleManager.h"
#include "fonts/Font.h"

namespace hvn3 {

	namespace Gui {

		StyleManager::StyleManager() :
			_primary_color(Color::FromArgb(35, 35, 35)), // 240, 240, 240
			_secondary_color(Color::FromArgb(186, 186, 186)), // 160, 160, 160
			_tertiary_color(Color::FromArgb(96, 96, 96)),
			_text_color(Color::Black),
			_draw_scale(1.0f) {
		}
		StyleManager::~StyleManager() {

			_fonts.RemoveAll();
			_bitmaps.RemoveAll();

		}
		const Color& StyleManager::PrimaryColor() const {

			return _primary_color;

		}
		const Color& StyleManager::SecondaryColor() const {

			return _secondary_color;

		}
		const Color& StyleManager::TertiaryColor() const {

			return _tertiary_color;

		}
		const Color& StyleManager::TextColor() const {

			return _text_color;

		}
		AssetHandle<Font> StyleManager::GetFontResource(FontResourceId id) {

			// Get a resource handle for the requested resource.
			AssetHandle<Font> resource = _fonts.Get(static_cast<AssetId>(id));

			// If the resource is null, load the default resource, and get a handle for it.
			if (!resource) {
				LoadDefaultFontResource(id);
				resource = _fonts[static_cast<AssetId>(id)];
			}

			// Return the resource.
			return resource;

		}
		AssetHandle<Graphics::Bitmap> StyleManager::GetImageResource(BitmapResourceId id) {

			// Get a resource handle for the requested resource.
			AssetHandle<Graphics::Bitmap> resource = _bitmaps.Get(static_cast<AssetId>(id));

			// If the resource is null, load the default resource, and get a handle for it.
			if (!resource) {
				LoadDefaultBitmapResource(id);
				resource = _bitmaps[static_cast<AssetId>(id)];
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

			AssetId resId = static_cast<AssetId>(id);

			switch (id) {

			case FontResourceId::PrimaryFont:
				_fonts.Add(resId, FontAssetArgs("data/System/Fonts/webly.ttf", 11, FontOptions::Monochrome));
				break;
			default:
				return false;
			}

			return true;

		}
		bool StyleManager::LoadDefaultBitmapResource(BitmapResourceId id) {

			AssetId resId = static_cast<AssetId>(id);

			switch (id) {

			case BitmapResourceId::ExitButton:
				_bitmaps.Add(resId, "data/System/Icons/close.png");
				break;
			case BitmapResourceId::ArrowR:
				_bitmaps.Add(resId, "data/System/Icons/arrow_r.png");
				break;
			default:
				return false;
			}

			return true;

		}

	}

}