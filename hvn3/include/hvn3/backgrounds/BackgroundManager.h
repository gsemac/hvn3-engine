#pragma once

#include "hvn3/backgrounds/IBackgroundManager.h"
#include "hvn3/core/ManagerBase.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/events/IEventManager.h"

#include <vector>

namespace hvn3 {

	class BackgroundManager :
		public IBackgroundManager,
		public EventListenerBase<events::DrawEvents, events::UpdateEvents> {

	public:
		BackgroundManager(IEventManager* eventManager);

		~BackgroundManager();

		void OnEvent(DrawEventArgs& e) override;
		void OnEvent(UpdateEventArgs& e) override;

		index_type AddBackground(const Background& background) override;
		index_type AddBackground(const Background& background, bool foreground) override;
		void RemoveBackgroundAt(index_type index) override;
		const Background& BackgroundAt(index_type index) const override;

		void SetBackgroundColor(const Color& color) override;

		size_type Count() const override;
		void Clear() override;

	private:
		IEventManager* eventManager;
		std::vector<Background> backgrounds;
		Color backgroundColor;

	};

}