#pragma once
#include "IViewManager.h"
#include <vector>

namespace hvn3 {

	class RoomBase;

	class ViewManager : public IViewManager {

	public:
		// Inherited via IViewManager
		virtual size_t ViewAdd(const View & view) override;
		virtual void ViewRemove(size_t index) override;
		virtual View & ViewAt(size_t index) override;
		virtual size_t ViewCount() const override;
		virtual void Clear() override;
		virtual void Update(ViewUpdateEventArgs & e) override;

	private:
		std::vector<View> _views;

	};

}