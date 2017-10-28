//#include "gui/ControlCollection.h"
//#include "exceptions/Exception.h"
//#include <utility>
//
//namespace Gui {
//
//	void ControlCollection::Add(control_type& element) {
//
//		_controls.push_back(std::move(element));
//
//	}
//	void ControlCollection::Remove(const control_type& element) {
//
//		_controls.remove(element);
//
//	}
//	void ControlCollection::Clear() {
//
//		_controls.clear();
//
//	}
//	ControlCollection::control_type& ControlCollection::At(size_t index) {
//
//		throw NotImplementedException();
//
//	}
//	size_t ControlCollection::Count() const {
//
//		return _controls.size();
//
//	}
//	bool ControlCollection::Contains(const control_type& element) {
//
//		throw NotImplementedException();
//
//	}
//
//}