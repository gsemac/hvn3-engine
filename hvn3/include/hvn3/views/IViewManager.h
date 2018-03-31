#pragma once
#include "hvn3/views/View.h"
#include "hvn3/views/ViewUpdateEventArgs.h"

namespace hvn3 {

	class IViewManager {

	public:
		virtual size_t Add(const View& view) = 0;
		virtual void RemoveAt(size_t index) = 0;
		virtual View& At(size_t index) = 0;
		virtual const View& At(size_t index) const = 0;
		virtual size_t Count() const = 0;
		virtual void Clear() = 0;

		virtual void Update(ViewUpdateEventArgs& e) = 0;

	};

}