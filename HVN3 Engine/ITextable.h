#pragma once
#include "Font.h"
#include "ResourceCollection.h"
#include "UTF8String.h"

namespace Gui {

	class Control;

	class ITextable {

	public:
		ITextable(Control* control, const String& text);

		virtual const String& Text() const;
		virtual void SetText(const String& text);

		virtual const ResourceHandle<::Font>& Font();
		virtual void SetFont(ResourceHandle<::Font>& font);

	private:
		Control* _control;
		String _text;
		ResourceHandle<::Font> _font;

	};

}