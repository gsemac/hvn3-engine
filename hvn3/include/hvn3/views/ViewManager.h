#pragma once
#include "hvn3/views/IViewManager.h"
#include <vector>

namespace hvn3 {

	class ViewManager : public IViewManager {

	public:
		// Inherited via IViewManager
		virtual size_t Add(const View & view) override;
		virtual void RemoveAt(size_t index) override;
		virtual View& At(size_t index) override;
		virtual const View& At(size_t index) const override;
		virtual size_t Count() const override;
		virtual void Clear() override;
		virtual void Update(ViewUpdateEventArgs & e) override;

	private:
		std::vector<View> _views;

	};

}