#pragma once

namespace hvn3 {

	class UpdateEventArgs;

	class IUpdatable {

	public:
		virtual void BeginUpdate(UpdateEventArgs& e);
		virtual void Update(UpdateEventArgs& e) = 0;
		virtual void EndUpdate(UpdateEventArgs& e);

	};

}