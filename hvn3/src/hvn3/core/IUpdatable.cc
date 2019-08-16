#include "hvn3/core/IUpdatable.h"
#include "hvn3/events/UpdateEventArgs.h"

namespace hvn3 {

	void IUpdatable::OnBeginUpdate(UpdateEventArgs& e) {}
	void IUpdatable::OnEndUpdate(UpdateEventArgs& e) {}

}