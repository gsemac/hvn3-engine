#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/ecs/ComponentManager.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/events/EventManager.h"
#include "hvn3/graphics/GraphicsDefs.h"
#include "hvn3/services/di_service_container.h"
#include "hvn3/views/IViewManager.h"

#include <vector>

namespace hvn3 {

	class ViewManager :
		public IViewManager,
		public EventListenerBase<events::UpdateEvents> {

	public:
		HVN3_INJECT(ViewManager(IEventManager& eventManager, ecs::ComponentManager* componentManager));

		index_type AddView(const View& view) override;
		void RemoveView(index_type index) override;
		View& ViewAt(index_type index) override;
		const View& ViewAt(index_type index) const override;
		size_type Count() const override;
		void Clear() override;

		RectangleF Viewport() const override;

		void ForEach(const std::function<void(View&)>& callback) override;
		void ForEach(const std::function<void(const View&)>& callback) const override;

		void OnEvent(UpdateEventArgs& e) override;

	private:
		IEventManager* eventManager;
		ecs::ComponentManager* componentManager;
		std::vector<View> _views;

	};

}