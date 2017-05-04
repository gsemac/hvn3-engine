#include "GuiStyleManager.h"
#include "Font.h"

namespace Gui {

	GuiStyleManager::GuiStyleManager() :
		_primary_color(Color::FromArgb(35, 35, 35)),
		_secondary_color(Color::FromArgb(186, 186, 186)) {
	}
	GuiStyleManager::~GuiStyleManager() {

		_fonts.Clear();
		_bitmaps.Clear();

	}
	const Color& GuiStyleManager::PrimaryColor() const {

		return _primary_color;

	}
	const Color& GuiStyleManager::SecondaryColor() const {

		return _secondary_color;

	}
	ResourceHandle<Font> GuiStyleManager::GetFontResource(GuiFontResourceId id) {

		// Get a resource handle for the requested font.
		ResourceHandle<Font> font = _fonts.Find((ResourceId)id);
		
		// If the resource is null, load the default resource, and get a handle for it.
		if (!font) {
			LoadDefaultFontResource(id);
			font = _fonts[(ResourceId)id];
		}

		// Return the resource.
		return font;

	}
	ResourceHandle<Drawing::Bitmap> GuiStyleManager::GetImageResource(GuiBitmapResourceId id) {

		return _bitmaps[(ResourceId)id];

	}

	// Protected members

	bool GuiStyleManager::LoadDefaultFontResource(GuiFontResourceId id) {

		ResourceId resId = (ResourceId)id;

		switch (id) {

		case GuiFontResourceId::PrimaryFont:
			_fonts.Add(resId, Resource::Create<Font>("data/System/Fonts/webly.ttf", 11, FontOptions::Monochrome));
			break;
		default:
			return false;
		}

		return true;

	}
	bool GuiStyleManager::LoadDefaultBitmapResource(GuiBitmapResourceId id) {

		ResourceId resId = (ResourceId)id;

		switch (id) {

		case GuiBitmapResourceId::ExitButton:
			_bitmaps.Add(resId, Resource::Create<Drawing::Bitmap>("data/System/Icons/close.png"));
			break;
		default:
			return false;
		}

		return true;

	}

}