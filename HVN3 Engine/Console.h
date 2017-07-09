#pragma once
#include <iostream>

namespace hvn3 {

	enum class ConsoleColor {
		Black,
		Blue,
		Cyan,
		DarkBlue,
		DarkCyan,
		DarkGray,
		DarkGreen,
		DarkMagenta,
		DarkRed,
		DarkYellow,
		Gray,
		Green,
		Magenta,
		Red,
		White,
		Yellow
	};

	class Console final {

	public:
		// Writes the text representation of the specified value to the standard output stream.
		template <typename T>
		static void Write(const T& value) {

			std::cout << value;

		}
		// Writes the specified string value, followed by the current line terminator, to the standard output stream.
		template<typename T>
		static void WriteLine(const T& value) {

			std::cout << value << '\n';

		}
		// Writes the current line terminator to the standard output stream.
		static void WriteLine();
		// Clears the console buffer and corresponding console window of display information.
		static void Clear();
		// Sets the background color of the console.
		static void SetBackgroundColor(ConsoleColor color);
		// Gets the background color of the console.
		static ConsoleColor BackgroundColor();
		// Sets the foreground color of the console.
		static void SetForegroundColor(ConsoleColor color);
		// Gets the foreground color of the console.
		static ConsoleColor ForegroundColor();
		// Sets the position of the cursor.
		static void SetCursorPosition(int x, int y);

	private:
		Console() = default;

	};

}