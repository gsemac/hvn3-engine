#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		WidgetBase::WidgetBase(float x, float y, float width, float height) :
			WidgetBase(PointF(x, y), SizeF(width, height)) {
		}
		WidgetBase::WidgetBase(const PointF& position, const SizeF& size) :
			_position(position),
			_size(size) {
		}

		void WidgetBase::HandleEvent(WidgetEventArgs& ev) {

			auto i = _callback_table.find(ev.Type());

			if (i == _callback_table.end())
				return;

			i->second(*this, ev);

		}
		void WidgetBase::SetEventHandler(GuiEvent ev, const event_handler_type& callback) {

			_callback_table[ev] = callback;

		}

		float WidgetBase::X() const {

			return _position.X();

		}
		float WidgetBase::Y() const {

			return _position.Y();

		}
		void WidgetBase::SetX(float x) {

			HandleEvent(MoveEventArgs(Position(), PointF(x, Y())));

			_position.SetX(x);

		}
		void WidgetBase::SetY(float y) {

			HandleEvent(MoveEventArgs(Position(), PointF(X(), y)));

			_position.SetY(y);

		}
		PointF WidgetBase::Position() const {

			return _position;

		}
		void WidgetBase::SetPosition(const PointF& position) {

			SetPosition(position.X(), position.Y());

		}
		void WidgetBase::SetPosition(float x, float y) {

			HandleEvent(MoveEventArgs(Position(), PointF(x, y)));

			_position.SetX(x);
			_position.SetY(y);

		}

		float WidgetBase::Width() const {

			return _size.Width();

		}
		float WidgetBase::Height() const {

			return _size.Height();

		}
		void WidgetBase::SetWidth(float value) {



			_size.SetWidth(value);

		}
		void WidgetBase::SetHeight(float value) {



			_size.SetHeight(value);

		}
		void WidgetBase::Resize(float width, float height) {



			_size.Resize(width, height);

		}
		SizeF WidgetBase::Size() const {

			return _size;

		}

	}
}