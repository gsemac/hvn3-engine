#pragma once

#include "io/idisplay_manager.h"
#include "io/allegro_display.h"
#include "core/service_provider.h"
#include "events/ievent_manager.h"

#include <memory>

namespace hvn3::io {

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
		mutable std::shared_ptr<AllegroDisplay> display;

		void InitializeDisplay() const;

	};

}