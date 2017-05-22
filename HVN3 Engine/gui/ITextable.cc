#include "gui/ITextable.h"
#include "gui/GuiManager.h"
#include "gui/Control.h"

namespace hvn3 {

	namespace Gui {

		ITextable::ITextable(Control* control, const String& text) :
			_text(text),
			_font(FontResourceId::PrimaryFont),
			_control(control) {
		}

		const String& ITextable::Text() const {

			return _text;

		}
		void ITextable::SetText(const String& text) {

			_text = text;

			_control->Invalidate();

		}

		const ResourceHandle<hvn3::Font> ITextable::Font() {

			if (_control->Manager() && _control->Manager()->StyleManager())
				return _control->Manager()->StyleManager()->GetFontResource(_font);

			return nullptr;

		}
		void ITextable::SetFont(Gui::FontResourceId font) {

			_font = font;

			_control->Invalidate();

		}

		// Protected methods

		String& ITextable::RefText() {

			return _text;

		}

	}

}