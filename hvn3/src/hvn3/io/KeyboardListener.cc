#include "hvn3/io/KeyboardListener.h"

namespace hvn3 {
	
	KeyboardListener::KeyboardListener() {

		// Add self to the global collection of listeners.
		_registry.Listeners.insert(this);
		_deregister_self = true;

	}
	KeyboardListener::~KeyboardListener() {

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

	void KeyboardListener::OnKeyDown(KeyDownEventArgs& e) {}
	void KeyboardListener::OnKeyPressed(KeyPressedEventArgs& e) {}
	void KeyboardListener::OnKeyUp(KeyUpEventArgs& e) {}
	void KeyboardListener::OnKeyChar(KeyCharEventArgs& e) {}
	void KeyboardListener::OnKeyboardLost(KeyboardLostEventArgs& e) {}
	void KeyboardListener::OnKeyboardFound(KeyboardFoundEventArgs& e) {}


	KeyboardListener::listener_collection_type& KeyboardListener::_listeners() {

		return _registry.Listeners;

	}

	KeyboardListener::ListenerRegistry KeyboardListener::_registry;



	KeyboardListener::ListenerRegistry::~ListenerRegistry() {
		for (auto i = Listeners.begin(); i != Listeners.end(); ++i)
			(*i)->_deregister_self = false;
	}

}