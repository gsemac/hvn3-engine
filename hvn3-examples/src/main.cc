#include <iostream>

#include "events/event_listener_base.h"
#include "events/multi_event_bus.h"
#include "io/window.h"
#include "services/di_service_container.h"
#include "core/application.h"
#include "events/user_event_source.h"
#include "events/user_event.h"
#include "events/event_queue.h"

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

	events::UserEventSource eventSource;
	events::EventQueue eventQueue;

	eventQueue.RegisterEventSource(eventSource);

	auto event = events::make_event(MyEventData());

	std::cout << "pushing event\n";

	eventSource.PushEvent(event);

	events::Event ev;

	std::cout << std::boolalpha << eventQueue.IsEmpty() << std::endl;
	std::cout << std::boolalpha << eventQueue.GetNextEvent(ev) << std::endl;

	std::cout << (int)ev.Type() << std::endl;

	//hvn3::Application app;

	//app.Run(window);

	//services::DIServiceContainer services;

	//services.RegisterService(std::make_shared<int>(6));
	//services.RegisterService<MyService>();

	//getchar();

	//events::MultiEventBus bus;
	//MyEventListener listener;

	//bus.Subscribe(&listener);

	//bus.Dispatch(5);
	//bus.Dispatch(4);
	//bus.Dispatch(3);
	//bus.Dispatch(2.0f);
	//bus.Dispatch(1);

	//std::cout << "dispatcher.Count(): " << bus.Count() << '\n';

	getchar();

	return 0;

}