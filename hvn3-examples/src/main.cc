#include <iostream>

#include "io/window.h"

using namespace hvn3;

int main() {

	io::Window window(640, 480, "hello world", io::WindowOptions::Resizable);

	window.GetCanvas().Clear(graphics::Color::FromHex("#abc"));

	window.Refresh();

	getchar();

	return 0;

}