#include "gui/TextableControl.h"
#include "gui/GuiManager.h"

namespace hvn3 {

	namespace Gui {

		TextableControl::TextableControl(const String& text) :
			_text(text),
			_font(FontResourceId::PrimaryFont) {
		}

		const String& TextableControl::Text() const {

			return _text;

		}
		void TextableControl::SetText(const String& text) {

			_text = text;

			Invalidate();

		}

		const AssetHandle<hvn3::Font> TextableControl::Font() {

			if (Manager() && Manager()->StyleManager())
				return Manager()->StyleManager()->GetFontResource(_font);

			std::cout << Manager()->StyleManager();

			return nullptr;

		}
		void TextableControl::SetFont(Gui::FontResourceId font) {

			_font = font;

			Invalidate();

		}

		// Protected methods

		String& TextableControl::RefText() {

			return _text;

		}

	}

}