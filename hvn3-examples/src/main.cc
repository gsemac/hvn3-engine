#include <iostream>

#include "events/event_listener_base.h"
#include "events/multi_event_bus.h"
#include "graphics/window.h"
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
	
	graphics::Window window(640, 480, "hello world", graphics::WindowOptions::Resizable);

	hvn3::Application app;

	app.Run(window);

	getchar();

	return 0;

}