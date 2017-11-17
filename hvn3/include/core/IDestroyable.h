#pragma once
#include "core/DestroyEventArgs.h"

namespace hvn3 {

	class IDestroyable {

	public:
		virtual void Destroy() = 0;
		virtual bool IsDestroyed() const = 0;

		virtual void OnDestroy(DestroyEventArgs& e) = 0;

	};

}