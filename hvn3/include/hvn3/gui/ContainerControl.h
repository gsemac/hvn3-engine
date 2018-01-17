#pragma once
#include "hvn3/gui/Control.h"
#include "hvn3/gui/ControlManager.h"
#include "hvn3/gui/_GuiManager.h"

/*
This class is a replacement for the now deprecated IContainer class.
*/

namespace hvn3 {
	namespace Gui {

		class ContainerControl : public virtual Control {

			class ContainerControlControlManager : public ControlManager {

			public:
				ContainerControlControlManager(GuiManager* gui_manager, Control* parent, const RectangleF& dockable_region);
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
			ControlManager* _controls();
			Gui::GuiManager* _childControlManager();
			bool _hasActiveChildControl() const;
			const RectangleF& _childRegion() const;
			void _setChildRegion(const RectangleF& region);
			bool _mouseInChildRegion() const;
			void _updateAnchors(ResizeEventArgs& e);

		private:
			void _initializeStyleManager();

			gui_manager_type _gui_manager;
			control_manager_type _control_manager;
			RectangleF _child_region;

		};

	}
}