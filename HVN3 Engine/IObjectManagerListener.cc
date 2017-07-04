#include "IObjectManagerListener.h"

namespace hvn3 {

	InstanceAddedEventArgs::InstanceAddedEventArgs(Object* instance) :
		_instance(instance) { }
	Object* InstanceAddedEventArgs::Instance() {
		return _instance;
	}

	InstanceRemovedEventArgs::InstanceRemovedEventArgs(Object* instance) :
		_instance(instance) {
	}
	Object* InstanceRemovedEventArgs::Instance() {
		return _instance;
	}

}