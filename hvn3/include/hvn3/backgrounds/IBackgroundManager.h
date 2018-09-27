#pragma once
#include "hvn3/core/IManager.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/backgrounds/BackgroundDrawEventArgs.h"
#include "hvn3/backgrounds/Background.h"
#include <cstdint>

namespace hvn3 {

	class IBackgroundManager :
		public IManager {

	public:
		virtual size_t Add(const Background& background) = 0;
		virtual size_t Add(const Background& background, bool foreground) = 0;
		virtual void Remove(size_t index) = 0;
		virtual const Background& At(size_t index) const = 0;
		virtual size_t Count() const = 0;
		virtual void Clear() = 0;

		virtual void ForEach(const std::function<bool(Background&)>& func) = 0;
		virtual void ForEach(const std::function<bool(const Background&)>& func) const = 0;

		virtual void Update(UpdateEventArgs& e) = 0;
		virtual void Draw(BackgroundDrawEventArgs& e) = 0;

	};

	template<>
	struct ManagerIdTraits<BACKGROUND_MANAGER> {
		typedef IBackgroundManager type;
	};

}