#pragma once

#include "../events/allegro_timer.h"
#include "core/app_base.h"

namespace hvn3 {

	class AllegroApp :
		public AppBase {

	public:
		AllegroApp();

	protected:
		void ConfigureServices(core::DIServiceContainer& services) override;

	private:
		events::AllegroTimer frameTimer;

	};

}