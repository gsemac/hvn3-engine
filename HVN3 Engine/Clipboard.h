#ifndef __CLIPBOARD_H
#define __CLIPBOARD_H
#include <allegro5\allegro.h>
#include <string>

class String;

class Clipboard {

private:

public:
	static void SetText(const char* text);
	static void SetText(const std::string& text);
	static void SetText(const String& text);
	static std::string GetText();
	static bool HasText();

};

#endif