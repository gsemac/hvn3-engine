#pragma once
#include "ResourceCollection.h"
#include "Bitmap.h"
#include "Color.h"

class Font;

namespace Gui {
	
	enum class GuiFontResourceId : ResourceId {
		PrimaryFont
	};

	enum class GuiBitmapResourceId : ResourceId {
		ExitButton
	};

	class GuiStyleManager {

	public:
		GuiStyleManager();
		~GuiStyleManager();

		const Color& PrimaryColor() const;
		const Color& SecondaryColor() const;
		ResourceHandle<Font> GetFontResource(GuiFontResourceId id);
		ResourceHandle<Drawing::Bitmap> GetImageResource(GuiBitmapResourceId id);

	protected:
		bool LoadDefaultFontResource(GuiFontResourceId id);
		bool LoadDefaultBitmapResource(GuiBitmapResourceId id);

	private:
		Color _primary_color;
		Color _secondary_color;
		ResourceCollection<Font> _fonts;
		ResourceCollection<Drawing::Bitmap> _bitmaps;
	
	};

}