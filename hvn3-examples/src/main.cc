#include <iostream>

#include "events/event_listener_base.h"
#include "events/multi_event_bus.h"
#include "io/window.h"
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

int main() {

	io::Window window(640, 480, "hello world", io::WindowOptions::Resizable);

	services::DIServiceContainer services;

	services.RegisterService(std::make_shared<int>(6));
	services.RegisterService<MyService>();

	getchar();

	events::MultiEventBus bus;
	MyEventListener listener;

	bus.Subscribe(&listener);

	bus.Dispatch(5);
	bus.Dispatch(4);
	bus.Dispatch(3);
	bus.Dispatch(2.0f);
	bus.Dispatch(1);

	std::cout << "dispatcher.Count(): " << bus.Count() << '\n';

	getchar();

	return 0;

}