#pragma once
#include "ResourceCollection.h"
#include "UTF8String.h"
#include "gui/StyleManager.h"

namespace Gui {

	class Control;

	class ITextable {

	public:
		ITextable(Control* control, const String& text);

		virtual const String& Text() const;
		virtual void SetText(const String& text);

		virtual const ResourceHandle<::Font> Font();
		virtual void SetFont(Gui::FontResourceId font);

	protected:
		String& RefText();

	private:
		Control* _control;
		String _text;
		Gui::FontResourceId _font;

	};

}