#include "gui/ControlController.h"
#include "gui/GuiManager.h"
#include "io/Mouse.h"
#include "io/Keyboard.h"
#include <utility>

namespace Gui {

	GuiManager::GuiManager()
		: GuiManager(new Gui::ControlManager(this), new Gui::StyleManager()) {

		_owns_managers = true;

	}
	GuiManager::GuiManager(Gui::ControlManager* control_manager, Gui::StyleManager* style_manager) {

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

	void GuiManager::Update(UpdateEventArgs& e) {

		ControlManager()->Update(e);

	}
	void GuiManager::Draw(DrawEventArgs& e) {

		ControlManager()->Draw(e);

	}

}