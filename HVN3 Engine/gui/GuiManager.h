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
			GuiManager(const Rectangle& dockable_region);
			GuiManager(const Rectangle& dockable_region, Gui::ControlManager* control_manager, Gui::StyleManager* style_manager);
			~GuiManager();

			void Clear();

			Gui::StyleManager* StyleManager();
			Gui::ControlManager* ControlManager();
			const Gui::ControlManager* ControlManager() const;

			const Rectangle& DockableRegion() const;

			void Update(UpdateEventArgs& e) override;
			void Draw(DrawEventArgs& e) override;

		private:
			bool _owns_managers;
			Gui::StyleManager* _style_manager;
			Gui::ControlManager* _control_manager;
			Rectangle _dockable_region;

		};

	}

}