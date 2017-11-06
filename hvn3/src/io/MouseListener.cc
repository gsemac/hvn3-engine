#include "io/MouseListener.h"

namespace hvn3 {

	MouseListener::MouseListener() {

		// Add self to the global collection of listeners.
		_registry.Listeners.insert(this);
		_deregister_self = true;

	}
	MouseListener::~MouseListener() {

		/*
		Be careful! Because the registry is static, in some scenarios, it's possible for it to be deinitialized before all instances of the listener class.
		If this happens, we need to make sure that they do not attempt to erase themselves from the deinitalized collection.
		The destructor of the registry object sets a flag in all listeners that can be checked for to see if the registry is still valid.

		This is just one approach to solving the problem-- Another previous approach was to make sure that all listener instances are deinitialized before
		reaching the end of the main function.
		*/

		if (_deregister_self)
			_registry.Listeners.erase(this);

	}

	void MouseListener::OnMouseDown(MouseDownEventArgs& e) {}
	void MouseListener::OnMousePressed(MousePressedEventArgs& e) {}
	void MouseListener::OnMouseUp(MouseUpEventArgs& e) {}
	void MouseListener::OnMouseMove(MouseMoveEventArgs& e) {}
	void MouseListener::OnMouseScroll(MouseScrollEventArgs& e) {}



	MouseListener::listener_collection_type& MouseListener::_listeners() {

		return _registry.Listeners;

	}

	MouseListener::ListenerRegistry MouseListener::_registry;



	MouseListener::ListenerRegistry::~ListenerRegistry() {
		for (auto i = Listeners.begin(); i != Listeners.end(); ++i)
			(*i)->_deregister_self = false;
	}

}