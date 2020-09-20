#include <iostream>

#include "events/event_listener_base.h"
#include "events/multi_event_bus.h"
#include "io/window.h"
#include "services/di_service_container.h"
#include "core/application.h"
#include "events/user_event_source.h"
#include "events/user_event.h"
#include "events/event_queue.h"
#include "events/event_manager.h"

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

class MyService {

public:
	HVN3_INJECT(MyService(int intService)) {

		std::cout << "int service value is " << intService << '\n';

	}

};

class MyEventData {

public:
	MyEventData() {

		std::cout << "Event data constructed\n";

	}
	MyEventData(const MyEventData&) {

		std::cout << "Event data copy constructed\n";

	}
	~MyEventData() {

		std::cout << "Event data destructed\n";

	}

};

int main() {

	io::Window window(640, 480, "hello world", io::WindowOptions::Resizable);

	events::MultiEventBus bus;

	bus.Subscribe<int>(
		[](int ev) {

			std::cout << "got event: " << ev << std::endl;

		}
	);

	bus.Dispatch(5);

	//events::EventManager eventManager;

	//eventManager.GetEventQueue().RegisterEventSource(window.GetEventSource());

	//events::Event ev;

	//while (eventManager.GetEventQueue().WaitForEvent(ev))
	//	std::cout << (int)ev.Type();

	hvn3::Application app;

	app.Run(window);

	getchar();

	return 0;

}