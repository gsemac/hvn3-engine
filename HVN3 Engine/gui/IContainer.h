#pragma once
#include "gui/GuiManager.h"

namespace hvn3 {

	namespace Gui {

		class Control;

		class IContainer {

			class ContainerControlManager : public ControlManager {

			public:
				ContainerControlManager(GuiManager* gui_manager, Control* parent);
				Handle<Control> AddControl(ControlPtr& control) override;

			private:
				Control* _control;

			};

		public:
			IContainer(Control* control);
			Gui::ControlManager* Controls();

		protected:
			Gui::GuiManager* ChildControlManager();
			bool HasActiveChildControl() const;
			const RectangleF& ChildRegion() const;
			void SetChildRegion(const RectangleF& region);
			bool MouseInChildRegion() const;
			void UpdateAnchors(ResizeEventArgs& e);

		private:
			Gui::GuiManager _manager;
			ContainerControlManager _control_manager;
			Control* _control;
			RectangleF _child_region;

			void SetUpStyleManager();

		};

	}

}