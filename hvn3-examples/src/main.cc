#include <iostream>

#include "events/event_queue.h"
#include "io/window.h"

using namespace hvn3;

int main() {

	io::Window window(640, 480, "hello world", io::WindowOptions::Resizable);

	events::EventQueue eventQueue;

	eventQueue.RegisterEventSource(window.GetEventSource());

	window.GetCanvas().Clear(graphics::Color::FromArgb(0, 255, 0));
	window.GetCanvas().PushClip(0, 0, 200, 200);
	window.GetCanvas().Clear(graphics::Color::FromHex("#fff"));
	window.GetCanvas().PushClip(0, 0, 50, 50);
	window.GetCanvas().Clear(graphics::Color::FromHex("#abc"));

	window.GetCanvas().ClearClip();

	graphics::Transform trans;

	trans.Rotate(20.0f);

	window.GetCanvas().PushTransform(trans);

	window.GetCanvas().DrawRectangle(0, 0, 200, 300, graphics::Color::Random(), 5.0f);

	window.Refresh();

	while (eventQueue.IsEmpty());

	return 0;

}