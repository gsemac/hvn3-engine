#pragma once
#include "assets/ResourceCollection.h"
#include "graphics/Bitmap.h"
#include "graphics/Color.h"

namespace hvn3 {

	class Font;

	namespace Gui {

		enum class FontResourceId : ResourceId {
			Null,
			PrimaryFont
		};

		enum class BitmapResourceId : ResourceId {
			Null,
			ExitButton,
			ArrowR
		};

		class StyleManager {

		public:
			StyleManager();
			~StyleManager();

			const Color& PrimaryColor() const;
			const Color& SecondaryColor() const;
			const Color& TertiaryColor() const;
			const Color& TextColor() const;
			ResourceHandle<Font> GetFontResource(FontResourceId id);
			ResourceHandle<Graphics::Bitmap> GetImageResource(BitmapResourceId id);

			float DrawingScale() const;
			void SetDrawingScale(float scale);

		protected:
			bool LoadDefaultFontResource(FontResourceId id);
			bool LoadDefaultBitmapResource(BitmapResourceId id);

		private:
			float _draw_scale;

			Color _primary_color;
			Color _secondary_color;
			Color _tertiary_color;
			Color _text_color;

			ResourceCollection<Font> _fonts;
			ResourceCollection<Graphics::Bitmap> _bitmaps;

		};

	}

}