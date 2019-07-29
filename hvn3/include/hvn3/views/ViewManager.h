#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/graphics/GraphicsDefs.h"
#include "hvn3/views/IViewManager.h"

#include <vector>

namespace hvn3 {

	class ViewManager :
		public ManagerBase<IViewManager>,
		public EventListenerBase<events::UpdateEvents> {

	public:
		ViewManager();

		index_type AddView(const View & view) override;
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
		std::vector<View> _views;

	};

}