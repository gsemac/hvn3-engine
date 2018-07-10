#pragma once
#include "hvn3/views/IViewManager.h"
#include <vector>

namespace hvn3 {

	class ViewManager : 
		public IViewManager {

	public:
		virtual size_t Add(const View & view) override;
		virtual void RemoveAt(size_t index) override;
		virtual View& At(size_t index) override;
		virtual const View& At(size_t index) const override;
		virtual size_t Count() const override;
		virtual void Clear() override;
		virtual void Update(ViewUpdateEventArgs & e) override;

		// Both of the "ForEach" methods iterate over the views such that top-most views are accessed first.

		void ForEach(const std::function<void(View&)>& func) override;
		void ForEach(const std::function<void(const View&)>& func) const override;


	private:
		std::vector<View> _views;

	};

}