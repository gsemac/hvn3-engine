#pragma once
#include "utility/UTF8String.h"
#include "gui/Control.h"
#include "gui/StyleManager.h"

/*
This class is meant to replace the now deprecated ITextable.
*/

namespace hvn3 {
	namespace Gui {

		class TextableControl : public virtual Control {

		public:
			TextableControl(const String& text);

			virtual const String& Text() const;
			virtual void SetText(const String& text);

			virtual const ResourceHandle<hvn3::Font> Font();
			virtual void SetFont(FontResourceId font);

		protected:
			String& RefText();

		private:
			String _text;
			Gui::FontResourceId _font;

		};

	}
}