#pragma once

#include "core/app_base.h"

#include <vector>

namespace hvn3::core {

	class GameApp :
		public AppBase {

	protected:
		void ConfigureServices(services::DIServiceContainer& services) override;

	};

}