#pragma once

#include "core/app_base.h"
#include "events/timer.h"

#include <vector>

namespace hvn3::core {

	class GameApp :
		public AppBase {

	public:
		GameApp();

	protected:
		void ConfigureServices(services::DIServiceContainer& services) override;

	private:
		events::Timer frameTimer;

	};

}