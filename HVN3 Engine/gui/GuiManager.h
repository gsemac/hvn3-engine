#pragma once
#include "IUpdatable.h"
#include "IDrawable.h"
#include "gui/ControlManager.h"
#include "gui/StyleManager.h"
#include <list>
#include <memory>

namespace hvn3 {

	namespace Gui {

		class GuiManager : public IUpdatable, public IDrawable {

		public:
			GuiManager(const RectangleF& dockable_region);
			GuiManager(const RectangleF& dockable_region, Gui::ControlManager* control_manager, Gui::StyleManager* style_manager);
			~GuiManager();

			void Clear();

			Gui::StyleManager* StyleManager();
			Gui::ControlManager* ControlManager();
			const Gui::ControlManager* ControlManager() const;
			
			// Returns the region use for docking child controls.
			const RectangleF& DockableRegion() const;
			// Sets the region used for docking child controls.
			void SetDockableRegion(const RectangleF& region);
			// Temporarily sets a new dockable region.
			void ResizeDockableRegion(const RectangleF& region);
			// Restores the dockable region, undoing any changes made by ResizeDockableRegion.
			void ResetDockableRegion();

			void OnUpdate(UpdateEventArgs& e) override;
			void OnDraw(DrawEventArgs& e) override;

		private:
			bool _owns_managers;
			Gui::StyleManager* _style_manager;
			Gui::ControlManager* _control_manager;
			RectangleF _dockable_region;
			RectangleF _temp_dockable_region;

		};

	}

}