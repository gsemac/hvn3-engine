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
			GuiManager();
			GuiManager(Gui::ControlManager* control_manager, Gui::StyleManager* style_manager);
			~GuiManager();

			void Clear();

			Gui::StyleManager* StyleManager();
			Gui::ControlManager* ControlManager();
			const Gui::ControlManager* ControlManager() const;

			void Update(UpdateEventArgs& e) override;
			void Draw(DrawEventArgs& e) override;

		private:
			bool _owns_managers;
			Gui::StyleManager* _style_manager;
			Gui::ControlManager* _control_manager;

		};

	}

}