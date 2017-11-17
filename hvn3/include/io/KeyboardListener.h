#pragma once
#include "io/KeyboardEventArgs.h"
#include <unordered_set>

namespace hvn3 {

	namespace System {
		class KeyboardController;
	}

	class KeyboardListener {

		friend class System::KeyboardController;
		friend struct ListenerRegistry;

		typedef std::unordered_set<KeyboardListener*> listener_collection_type;

		struct ListenerRegistry {
			listener_collection_type Listeners;
			~ListenerRegistry();
		};

	public:
		KeyboardListener();
		virtual ~KeyboardListener();

		virtual void OnKeyDown(KeyDownEventArgs& e);
		virtual void OnKeyPressed(KeyPressedEventArgs& e);
		virtual void OnKeyUp(KeyUpEventArgs& e);
		virtual void OnKeyChar(KeyCharEventArgs& e);

	protected:
		static listener_collection_type& _listeners();

	private:
		static ListenerRegistry _registry;

		bool _deregister_self;

	};

}