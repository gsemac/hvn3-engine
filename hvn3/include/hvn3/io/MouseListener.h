#pragma once
#include "hvn3/io/MouseEventArgs.h"
#include <unordered_set>

namespace hvn3 {

	namespace System {
		class MouseMutator;
	}

	class MouseListener {
		friend class System::MouseMutator;
		friend struct ListenerRegistry;

		typedef std::unordered_set<MouseListener*> listener_collection_type;

		struct ListenerRegistry {
			listener_collection_type Listeners;
			~ListenerRegistry();
		};

	public:
		MouseListener();
		virtual ~MouseListener();

		virtual void OnMouseDown(MouseDownEventArgs& e);
		virtual void OnMousePressed(MousePressedEventArgs& e);
		virtual void OnMouseUp(MouseUpEventArgs& e);
		virtual void OnMouseMove(MouseMoveEventArgs& e);
		virtual void OnMouseScroll(MouseScrollEventArgs& e);

	protected:
		static listener_collection_type& _listeners();

	private:
		static ListenerRegistry _registry;

		bool _deregister_self;

	};

}