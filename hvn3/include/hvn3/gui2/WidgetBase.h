#pragma once
#include "hvn3/gui2/IWidget.h"
#include "hvn3/core/ISizeable.h"
#include "hvn3/core/IPositionable2d.h"
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class WidgetBase : public IWidget, public IPositionable2d<float>, public ISizeable<float> {

			typedef std::unordered_map<GuiEvent, event_handler_type> callback_table_type;

		public:
			WidgetBase(float x, float y, float width, float height);
			WidgetBase(const PointF& position, const SizeF& size);

			void HandleEvent(WidgetEventArgs& ev) override;
			void SetEventHandler(GuiEvent ev, const event_handler_type& callback) override;

			float X() const override;
			float Y() const override;
			void SetX(float x) override;
			void SetY(float y) override;
			PointF Position() const;
			void SetPosition(const PointF& position);
			void SetPosition(float x, float y);

			virtual float Width() const override;
			virtual float Height() const override;
			virtual void SetWidth(float value) override;
			virtual void SetHeight(float value) override;
			virtual void Resize(float width, float height) override;
			virtual SizeF Size() const override;

		private:
			PointF _position;
			SizeF _size;
			callback_table_type _callback_table;

		};

	}
}