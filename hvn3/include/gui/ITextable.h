#pragma once
#include "resources/ResourceCollection.h"
#include "utility/UTF8String.h"
#include "gui/StyleManager.h"

namespace hvn3 {

	namespace Gui {

		class Control;

		class ITextable {

		public:
			ITextable(Control* control, const String& text);

			virtual const String& Text() const;
			virtual void SetText(const String& text);

			virtual const ResourceHandle<hvn3::Font> Font();
			virtual void SetFont(Gui::FontResourceId font);

		protected:
			String& RefText();

		private:
			Control* _control;
			String _text;
			Gui::FontResourceId _font;

		};

	}

}