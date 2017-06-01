#pragma once
#include "View.h"
#include "ViewUpdateEventArgs.h"

namespace hvn3 {

	class IViewManager {

	public:
		virtual size_t AddView(const View& view) = 0;
		virtual void RemoveView(size_t index) = 0;
		virtual View& ViewAt(size_t index) = 0;
		virtual size_t ViewCount() const = 0;
		virtual void Clear() = 0;

		virtual void Update(ViewUpdateEventArgs& e) = 0;

	};

}