#pragma once
#include "Console.h"
#ifdef _WIN32
#define OS_WINDOWS
#include <windows.h>
#endif

namespace hvn3 {

#ifdef OS_WINDOWS

	enum {
		COLOR_BLACK = 0x0,
		COLOR_DARK_BLUE = 0x1,
		COLOR_DARK_GREEN = 0x2,
		COLOR_DARK_CYAN = 0x3,
		COLOR_DARK_RED = 0x4,
		COLOR_DARK_MAGENTA = 0x5,
		COLOR_DARK_YELLOW = 0x6,
		COLOR_GRAY = 0x7,
		COLOR_DARK_GRAY = 0x8,
		COLOR_BLUE = 0x9,
		COLOR_GREEN = 0xA,
		COLOR_CYAN = 0xB,
		COLOR_RED = 0xC,
		COLOR_MAGENTA = 0xD,
		COLOR_YELLOW = 0xE,
		COLOR_WHITE = 0xF
	};

	bool _GetConsoleColor(short& color) {

		CONSOLE_SCREEN_BUFFER_INFO info;

		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
			return false;

		color = info.wAttributes;

		return true;

	}
	void _SetConsoleColor(short color) {

		HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(h_console, color);

	}
	ConsoleColor _ColorToConsoleColor(short color) {

		switch (color) {
		default:
		case COLOR_BLACK:
			return ConsoleColor::Black;
		case COLOR_DARK_BLUE:
			return ConsoleColor::DarkBlue;
		case COLOR_DARK_GREEN:
			return ConsoleColor::DarkGreen;
		case COLOR_DARK_CYAN:
			return ConsoleColor::DarkCyan;
		case COLOR_DARK_RED:
			return ConsoleColor::DarkRed;
		case COLOR_DARK_MAGENTA:
			return ConsoleColor::DarkMagenta;
		case COLOR_DARK_YELLOW:
			return ConsoleColor::DarkYellow;
		case COLOR_GRAY:
			return ConsoleColor::Gray;
		case COLOR_DARK_GRAY:
			return ConsoleColor::DarkGray;
		case COLOR_BLUE:
			return ConsoleColor::Blue;
		case COLOR_GREEN:
			return ConsoleColor::Green;
		case COLOR_CYAN:
			return ConsoleColor::Cyan;
		case COLOR_RED:
			return ConsoleColor::Red;
		case COLOR_MAGENTA:
			return ConsoleColor::Magenta;
		case COLOR_YELLOW:
			return ConsoleColor::Yellow;
		case COLOR_WHITE:
			return ConsoleColor::White;
		}

	}
	short _ConsoleColorToColor(ConsoleColor color) {

		switch (color) {
		default:
		case ConsoleColor::Black:
			return COLOR_BLACK;
		case ConsoleColor::Blue:
			return COLOR_BLUE;
		case ConsoleColor::Cyan:
			return COLOR_CYAN;
		case ConsoleColor::DarkBlue:
			return COLOR_DARK_BLUE;
		case ConsoleColor::DarkCyan:
			return COLOR_DARK_CYAN;
		case ConsoleColor::DarkGray:
			return COLOR_DARK_GRAY;
		case ConsoleColor::DarkGreen:
			return COLOR_DARK_GREEN;
		case ConsoleColor::DarkMagenta:
			return COLOR_DARK_MAGENTA;
		case ConsoleColor::DarkRed:
			return COLOR_DARK_RED;
		case ConsoleColor::DarkYellow:
			return COLOR_DARK_YELLOW;
		case ConsoleColor::Gray:
			return COLOR_GRAY;
		case ConsoleColor::Green:
			return COLOR_GREEN;
		case ConsoleColor::Magenta:
			return COLOR_MAGENTA;
		case ConsoleColor::Red:
			return COLOR_RED;
		case ConsoleColor::White:
			return COLOR_WHITE;
		case ConsoleColor::Yellow:
			return COLOR_YELLOW;
		}

	}

#endif

	void Console::Clear() {

#ifdef OS_WINDOWS
		system("cls");
#endif

	}
	void Console::SetBackgroundColor(ConsoleColor color) {

#ifdef OS_WINDOWS
		short current_color;
		_GetConsoleColor(current_color);
		_SetConsoleColor((_ConsoleColorToColor(color) << 4) | (current_color & 0x0F));
#endif

	}
	ConsoleColor Console::BackgroundColor() {

#ifdef OS_WINDOWS
		short current_color;
		_GetConsoleColor(current_color);
		return _ColorToConsoleColor(current_color >> 4);
#endif

	}
	void Console::SetForegroundColor(ConsoleColor color) {

#ifdef OS_WINDOWS
		short current_color;
		_GetConsoleColor(current_color);
		_SetConsoleColor(_ConsoleColorToColor(color) | (current_color & 0xF0));
#endif

	}
	ConsoleColor Console::ForegroundColor() {

#ifdef OS_WINDOWS
		short current_color;
		_GetConsoleColor(current_color);
		return _ColorToConsoleColor(current_color & 0x0F);
#endif

	}
	void Console::SetCursorPosition(int x, int y) {

#ifdef OS_WINDOWS
		COORD position = { x, y };
		HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(h_console, position);
#endif

	}

}