#pragma once

namespace hvn3 {

	class UpdateEventArgs;

	class IUpdatable {

	public:
		virtual void OnBeginUpdate(UpdateEventArgs& e);
		virtual void OnUpdate(UpdateEventArgs& e) = 0;
		virtual void OnEndUpdate(UpdateEventArgs& e);

	};

}