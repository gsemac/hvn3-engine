#pragma once
#include "hvn3/views/IViewManager.h"
#include <vector>

namespace hvn3 {

	class ViewManager : public IViewManager {

	public:
		// Inherited via IViewManager
		virtual size_t AddView(const View & view) override;
		virtual void RemoveView(size_t index) override;
		virtual View& ViewAt(size_t index) override;
		virtual const View& ViewAt(size_t index) const override;
		virtual size_t ViewCount() const override;
		virtual void Clear() override;
		virtual void Update(ViewUpdateEventArgs & e) override;

	private:
		std::vector<View> _views;

	};

}