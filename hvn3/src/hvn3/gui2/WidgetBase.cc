#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		WidgetBase::WidgetBase(float x, float y, float width, float height) :
			WidgetBase(PointF(x, y), SizeF(width, height)) {
		}
		WidgetBase::WidgetBase(const PointF& position, const SizeF& size) :
			_position(position),
			_size(size) {

			_parent_manager = nullptr;

		}

		void WidgetBase::HandleEvent(WidgetEventArgs& ev) {

			auto i = _callback_table.find(ev.Type());

			if (i == _callback_table.end())
				return;

			i->second(*this, ev);

		}
		void WidgetBase::SetEventHandler(GuiEvent ev, const EventHandler& callback) {

			_callback_table[ev] = callback;

		}

		const std::string& WidgetBase::Name() const {

			return _name;

		}
		void WidgetBase::SetName(const std::string& value) {

			_name = value;

		}
		const PointF& WidgetBase::Position() const {

			return _position;

		}
		void WidgetBase::SetPosition(const PointF& value) {

			_position = value;

		}
		const SizeF& WidgetBase::Size() const {

			return _size;

		}
		void WidgetBase::SetSize(const SizeF& value) {

			_size = value;

		}



		WidgetManager* WidgetBase::Manager() {

			return _parent_manager;

		}
		void WidgetBase::SetManager(WidgetManager* value) {

			_parent_manager = value;

			if (value != nullptr)
				_child_control_manager.SetRenderer(_parent_manager->GetRenderer());

		}

	}
}