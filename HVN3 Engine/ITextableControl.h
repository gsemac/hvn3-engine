#pragma once
#include "Control.h"
#include "Font.h"

namespace GUI {

	class ITextableControl : public Control {

	public:
		ITextableControl(const Point& location, const Size& size, const char* text);
		ITextableControl(const Point& location, const Size& size, std::string& text);
		ITextableControl(const Point& location, const Size& size, const Utf8String& text);

		virtual const Utf8String& Text() const;
		virtual void SetText(const char* text);
		virtual void SetText(const std::string& text);
		virtual void SetText(const Utf8String& text);

		virtual const Font& Font() const;
		virtual void SetFont(const ::Font& font);

	private:
		Utf8String __text;
		const ::Font* __font;

	};

}