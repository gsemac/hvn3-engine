#pragma once
#include "EventArgs.h"
#include "IUpdatable.h"

namespace hvn3 {
	
	class Object;

	class InstanceAddedEventArgs : public System::EventArgs {
	public:
		InstanceAddedEventArgs(Object* instance);
		Object* Instance();
	private:
		Object* _instance;
	};

	class InstanceRemovedEventArgs : public System::EventArgs {
	public:
		InstanceRemovedEventArgs(Object* instance);
		Object* Instance();
	private:
		Object* _instance;
	};

	class InstancesClearedEventArgs : public System::EventArgs {
	public:
		InstancesClearedEventArgs() = default;
	};

	class IObjectManagerListener {

	public:
		virtual void OnInstanceAdded(InstanceAddedEventArgs& e) = 0;
		virtual void OnInstanceRemoved(InstanceRemovedEventArgs& e) = 0;
		virtual void OnInstancesCleared(InstancesClearedEventArgs& e) = 0;
		virtual void OnObjectManagerUpdate(UpdateEventArgs& e) = 0;

	};

}