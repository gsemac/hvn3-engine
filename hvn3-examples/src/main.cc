#include <iostream>

#include "io/window.h"

using namespace hvn3;

int main() {

	io::Window window(640, 480, "hello world", io::WindowOptions::Resizable);

	window.GetCanvas().Clear(graphics::Color::FromArgb(0, 255, 0));
	window.GetCanvas().PushClip(0, 0, 200, 200);
	window.GetCanvas().Clear(graphics::Color::FromHex("#fff"));
	window.GetCanvas().PushClip(0, 0, 50, 50);
	window.GetCanvas().Clear(graphics::Color::FromHex("#abc"));

	window.Refresh();

	getchar();

	return 0;

}