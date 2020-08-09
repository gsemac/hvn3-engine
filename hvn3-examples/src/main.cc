#include <iostream>

#include "events/multi_event_listener_container.h"
#include "io/window.h"

using namespace hvn3;

class EventListener :
	public events::EventListenerBase<int> {

public:
	void OnEvent(int ev) override {

		std::cout << ev;

	}

};

int main() {

	io::Window window(640, 480, "hello world", io::WindowOptions::Resizable);

	EventListener listener;
	events::EventListenerContainer<int> listenerContainer;

	listenerContainer.Subscribe(&listener);

	listenerContainer.Dispatch(5);
	listenerContainer.Dispatch(4);
	listenerContainer.Dispatch(3);
	listenerContainer.Dispatch(2);
	listenerContainer.Dispatch(1);

	getchar();

	return 0;

}