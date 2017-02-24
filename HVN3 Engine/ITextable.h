#pragma once
#include "UTF8String.h"

class ITextable {

public:
	ITextable(const char* text);
	ITextable(const std::string& text);
	ITextable(const Utf8String& text);

	virtual const Utf8String& Text() const;
	virtual void SetText(const char* text);
	virtual void SetText(const std::string& text);
	virtual void SetText(const Utf8String& text);

private:
	Utf8String __text;

};