#include <iostream>

#include "events/event_listener_base.h"
#include "events/multi_event_dispatcher.h"
#include "io/window.h"

using namespace hvn3;

class EventListener :
	public events::EventListenerBase<int, float> {

public:
	~EventListener() {

		std::cout << "\ndestroyed!\n";

	}

	void OnEvent(int ev) override {

		std::cout << ev;

	}
	void OnEvent(float ev) override {

		std::cout << "float";

	}

};

int main() {

	io::Window window(640, 480, "hello world", io::WindowOptions::Resizable);

	events::MultiEventDispatcher dispatcher;
	EventListener listener;

	dispatcher.Subscribe(&listener);

	dispatcher.Dispatch(5);
	dispatcher.Dispatch(4);
	dispatcher.Dispatch(3);
	dispatcher.Dispatch(2.0f);
	dispatcher.Dispatch(1);

	std::cout << std::endl;

	std::cout << "dispatcher.Count(): " << dispatcher.Count() << '\n';

	getchar();

	return 0;

}