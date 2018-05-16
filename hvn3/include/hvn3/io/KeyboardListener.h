#pragma once
#include "hvn3/io/IKeyboardListener.h"
#include <unordered_set>

namespace hvn3 {

	namespace System {
		class KeyboardMutator;
	}

	class KeyboardListener : public virtual IKeyboardListener {

		friend class System::KeyboardMutator;
		friend struct ListenerRegistry;

		typedef std::unordered_set<KeyboardListener*> listener_collection_type;

		struct ListenerRegistry {
			listener_collection_type Listeners;
			~ListenerRegistry();
		};

	public:
		KeyboardListener();
		virtual ~KeyboardListener();

		void OnKeyDown(KeyDownEventArgs& e) override;
		void OnKeyPressed(KeyPressedEventArgs& e) override;
		void OnKeyUp(KeyUpEventArgs& e) override;
		void OnKeyChar(KeyCharEventArgs& e) override;
		void OnKeyboardLost(KeyboardLostEventArgs& e) override;
		void OnKeyboardFound(KeyboardFoundEventArgs& e) override;

	protected:
		static listener_collection_type& _listeners();

	private:
		static ListenerRegistry _registry;

		bool _deregister_self;

	};

}