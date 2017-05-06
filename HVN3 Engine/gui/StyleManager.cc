#include "StyleManager.h"
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
	ResourceHandle<Drawing::Bitmap> GuiStyleManager::GetImageResource(GuiBitmapResourceId id) {

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