#pragma once
#include "hvn3/utility/UTF8String.h"
#include "hvn3/gui/Control.h"
#include "hvn3/gui/StyleManager.h"

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

			virtual const AssetHandle<hvn3::Font> Font();
			virtual void SetFont(FontResourceId font);

		protected:
			String& RefText();

		private:
			String _text;
			Gui::FontResourceId _font;

		};

	}
}