#include <iostream>

#include "core/application.h"
#include "core/window.h"
#include "events/event_listener_base.h"
#include "events/event_manager.h"
#include "events/event_queue.h"
#include "events/multi_event_bus.h"
#include "events/user_event.h"
#include "events/user_event_source.h"
#include "services/di_service_container.h"

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

	core::Window window(640, 480, "hello world", core::WindowOptions::Resizable);

	core::Application app;

	app.Run(window);

	getchar();

	return 0;

}