#pragma once
#include "hvn3/io/IMouseListener.h"
#include <unordered_set>

namespace hvn3 {

	namespace System {
		class MouseMutator;
	}

	class MouseListener : public virtual IMouseListener {
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

		void OnMouseDown(MouseDownEventArgs& e) override;
		void OnMousePressed(MousePressedEventArgs& e) override;
		void OnMouseReleased(MouseReleasedEventArgs& e) override;
		void OnMouseMove(MouseMoveEventArgs& e) override;
		void OnMouseScroll(MouseScrollEventArgs& e) override;

	protected:
		static listener_collection_type& _listeners();

	private:
		static ListenerRegistry _registry;

		bool _deregister_self;

	};

}