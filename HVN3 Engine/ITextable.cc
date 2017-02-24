#include "ITextable.h"

ITextable::ITextable(const char* text) :
	__text(text) {}
ITextable::ITextable(const std::string& text) :
	ITextable(text.c_str()) {}
ITextable::ITextable(const Utf8String& text) :
	__text(text) {}

const Utf8String& ITextable::Text() const {

	return __text;

}
void ITextable::SetText(const char* text) {

	__text = text;

}
void ITextable::SetText(const std::string& text) {

	__text = text;

}
void ITextable::SetText(const Utf8String& text) {

	__text = text;

}