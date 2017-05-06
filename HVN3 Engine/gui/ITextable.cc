#include "ITextable.h"
#include "GuiManager.h"

namespace Gui {

	ITextable::ITextable(Control* control, const String& text) :
		_text(text),
		_font(nullptr),
		_control(control) {
	}

	const String& ITextable::Text() const {

		return _text;

	}
	void ITextable::SetText(const String& text) {

		_text = text;

		_control->Invalidate();

	}

	const ResourceHandle<::Font>& ITextable::Font() {
		
		if (!_font && _control->Manager())
			_font = _control->Manager()->StyleManager()->GetFontResource(Gui::GuiFontResourceId::PrimaryFont);

		return _font;

	}
	void ITextable::SetFont(ResourceHandle<::Font>& font) {

		_font = font;

		_control->Invalidate();

	}

}