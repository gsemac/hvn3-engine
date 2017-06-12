#pragma once
#include "gui/Control.h"
#include "gui/ControlManager.h"
#include "gui/GuiManager.h"

/*
This class is a replacement for the now deprecated IContainer class.
*/

namespace hvn3 {
	namespace Gui {

		class ContainerControl : public virtual Control {

			class ContainerControlControlManager : public ControlManager {

			public:
				ContainerControlControlManager(GuiManager* gui_manager, Control* parent);
				Handle<Control> AddControl(ControlPtr& control) override;

			private:
				Control* _control;

			};

			typedef GuiManager gui_manager_type;
			typedef ContainerControlControlManager control_manager_type;

		public:
			ContainerControl();

			virtual void OnPaint(PaintEventArgs& e) override;
			virtual void OnUpdate(UpdateEventArgs& e) override;

		protected:
			ControlManager* _Controls();
			Gui::GuiManager* _ChildControlManager();
			bool _HasActiveChildControl() const;
			const RectangleF& _ChildRegion() const;
			void _SetChildRegion(const RectangleF& region);
			bool _MouseInChildRegion() const;
			void _UpdateAnchors(ResizeEventArgs& e);

		private:
			void _InitializeStyleManager();

			gui_manager_type _gui_manager;
			control_manager_type _control_manager;
			RectangleF _child_region;

		};

	}
}