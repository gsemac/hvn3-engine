#pragma once

#include "hvn3/backgrounds/ibackground_manager.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/events/IEventManager.h"
#include "hvn3/services/di_service_container.h"

#include <vector>

namespace hvn3::backgrounds {

	class BackgroundManager :
		public IBackgroundManager,
		public EventListenerBase<events::DrawEvents, events::UpdateEvents> {

	public:
		HVN3_INJECT(BackgroundManager(IEventManager* eventManager));

		~BackgroundManager();

		void OnEvent(DrawEventArgs& e) override;
		void OnEvent(UpdateEventArgs& e) override;

		Background& AddBackground(const Background& background) override;

		void SetBackgroundColor(const Color& color) override;

		size_type Count() const override;

		void Clear() override;

	private:
		IEventManager* eventManager;
		std::vector<Background> backgrounds;
		Color backgroundColor;

	};

}