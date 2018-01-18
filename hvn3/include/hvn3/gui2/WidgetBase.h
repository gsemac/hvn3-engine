#pragma once
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/WidgetManager.h"
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class WidgetBase : public IWidget {

			typedef std::unordered_map<GuiEvent, EventHandler> callback_table_type;

		public:
			WidgetBase(float x, float y, float width, float height);
			WidgetBase(const PointF& position, const SizeF& size);

			void HandleEvent(WidgetEventArgs& ev) override;
			void SetEventHandler(GuiEvent ev, const EventHandler& callback) override;

			const std::string& Name() const override;
			void SetName(const std::string& value) override;
			const PointF& Position() const override;
			void SetPosition(const PointF& value) override;
			const SizeF& Size() const override;
			float SetSize(const SizeF& value) override;

		protected:
			WidgetManager* Manager() override;
			void SetManager(WidgetManager* value) override;

		private:
			std::string _name;
			PointF _position;
			SizeF _size;
			WidgetManager* _parent_manager;
			WidgetManager _child_control_manager;
			callback_table_type _callback_table;

		};

	}
}