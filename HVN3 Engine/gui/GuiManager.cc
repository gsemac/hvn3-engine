#include "gui/ControlController.h"
#include "gui/GuiManager.h"
#include "io/Mouse.h"
#include "io/Keyboard.h"
#include <utility>

namespace hvn3 {

	namespace Gui {

		GuiManager::GuiManager(const RectangleF& dockable_region)
			: GuiManager(dockable_region, new Gui::ControlManager(this), new Gui::StyleManager()) {

			_owns_managers = true;

		}
		GuiManager::GuiManager(const RectangleF& dockable_region, Gui::ControlManager* control_manager, Gui::StyleManager* style_manager) :
			_dockable_region(dockable_region) {

			_control_manager = control_manager;
			_style_manager = style_manager;
			_owns_managers = false;

		}
		GuiManager::~GuiManager() {

			if (!_owns_managers)
				return;

			if (_style_manager)
				delete _style_manager;

			if (_control_manager)
				delete _control_manager;

			_style_manager = nullptr;
			_control_manager = nullptr;

		}

		void GuiManager::Clear() {

			ControlManager()->Clear();

		}

		StyleManager* GuiManager::StyleManager() {

			return _style_manager;

		}
		ControlManager* GuiManager::ControlManager() {

			return const_cast<Gui::ControlManager*>(static_cast<const GuiManager*>(this)->ControlManager());

		}
		const ControlManager* GuiManager::ControlManager() const {

			return _control_manager;

		}

		const RectangleF& GuiManager::DockableRegion() const {

			return _dockable_region;

		}

		void GuiManager::OnUpdate(UpdateEventArgs& e) {

			ControlManager()->OnUpdate(e);

		}
		void GuiManager::OnDraw(DrawEventArgs& e) {

			ControlManager()->OnDraw(e);

		}

	}

}