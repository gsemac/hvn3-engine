#include <iostream>

#include "events/event_listener_base.h"
#include "events/multi_event_bus.h"
#include "io/window.h"

using namespace hvn3;

class MyEventListener :
	public events::EventListenerBase<int, float> {

public:
	void OnEvent(int ev) override {

		std::cout << ev;

	}
	void OnEvent(float ev) override {

		std::cout << "float";

	}

};

int main() {

	io::Window window(640, 480, "hello world", io::WindowOptions::Resizable);

	events::MultiEventBus bus;
	MyEventListener listener;

	bus.Subscribe(&listener);
	bus.Unsubscribe(&listener);

	bus.Dispatch(5);
	bus.Dispatch(4);
	bus.Dispatch(3);
	bus.Dispatch(2.0f);
	bus.Dispatch(1);

	std::cout << "dispatcher.Count(): " << bus.Count() << '\n';

	getchar();

	return 0;

}