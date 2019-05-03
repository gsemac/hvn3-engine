#pragma once

#include "hvn3/core/IManager.h"
#include "hvn3/views/View.h"

namespace hvn3 {

	class IViewManager {

	public:
		typedef size_t index_type;
		typedef size_t size_type;

		virtual index_type AddView(const View& view) = 0;
		virtual void RemoveView(index_type index) = 0;
		virtual View& ViewAt(index_type index) = 0;
		virtual const View& ViewAt(index_type index) const = 0;
		virtual size_type Count() const = 0;
		virtual void Clear() = 0;

		virtual void ForEach(const std::function<void(View&)>& func) = 0;
		virtual void ForEach(const std::function<void(const View&)>& func) const = 0;

	};

}