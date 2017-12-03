#pragma once
#include "hvn3/assets/AssetManager.h"
#include "hvn3/assets/FontAssetLoader.h"
#include "hvn3/assets/BitmapAssetLoader.h"
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/graphics/Color.h"

namespace hvn3 {

	class Font;

	namespace Gui {

		enum class FontResourceId : AssetId {
			Null,
			PrimaryFont
		};

		enum class BitmapResourceId : AssetId {
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
			AssetHandle<Font> GetFontResource(FontResourceId id);
			AssetHandle<Graphics::Bitmap> GetImageResource(BitmapResourceId id);

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

			AssetManager<FontAssetLoader> _fonts;
			AssetManager<BitmapAssetLoader> _bitmaps;

		};

	}

}