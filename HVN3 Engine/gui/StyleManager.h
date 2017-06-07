#pragma once
#include "ResourceCollection.h"
#include "Bitmap.h"
#include "Color.h"

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
			ResourceHandle<Font> GetFontResource(FontResourceId id);
			ResourceHandle<Drawing::Bitmap> GetImageResource(BitmapResourceId id);

			float DrawingScale() const;
			void SetDrawingScale(float scale);

		protected:
			bool LoadDefaultFontResource(FontResourceId id);
			bool LoadDefaultBitmapResource(BitmapResourceId id);

		private:
			float _draw_scale;
			Color _primary_color;
			Color _secondary_color;
			ResourceCollection<Font> _fonts;
			ResourceCollection<Drawing::Bitmap> _bitmaps;

		};

	}

}