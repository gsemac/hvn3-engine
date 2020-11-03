#pragma once

#include "core/idisplay_manager.h"
#include "core/window.h"
#include "events/ievent_manager.h"
#include "services/di_service_container.h"

#include <memory>

namespace hvn3::core {

	class DisplayManager :
		public IDisplayManager {

	public:
		HVN3_INJECT(DisplayManager(events::IEventManager& eventManager));

		IDisplay& GetDisplay() override;
		const IDisplay& GetDisplay() const override;
		void SetDisplayOptions(const DisplayOptions& displayOptions) override;

	private:
		events::IEventManager& eventManager;
		DisplayOptions displayOptions;
		mutable std::shared_ptr<Window> display;

		void InitializeDisplay() const;

	};

}