#pragma once
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/gui/ControlManager.h"
#include "hvn3/gui/StyleManager.h"
#include <list>
#include <memory>

namespace hvn3 {

	namespace Gui {

		class GuiManager : public IUpdatable, public IDrawable {

		public:
			GuiManager(const RectangleF& dockable_region);
			GuiManager(Gui::ControlManager* control_manager, Gui::StyleManager* style_manager);
			~GuiManager();

			void Clear();

			Gui::StyleManager* StyleManager();
			Gui::ControlManager* ControlManager();
			const Gui::ControlManager* ControlManager() const;
			
			void OnUpdate(UpdateEventArgs& e) override;
			void OnDraw(DrawEventArgs& e) override;

		private:
			bool _owns_managers;
			Gui::StyleManager* _style_manager;
			Gui::ControlManager* _control_manager;

		};

	}

}