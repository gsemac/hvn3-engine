#pragma once

#include "../events/allegro_timer.h"
#include "core/app_base.h"

namespace hvn3 {

	class AllegroApp :
		public AppBase {

	public:
		AllegroApp();

	protected:
		void ConfigureServices(ServiceProvider& services) override;

	private:
		events::AllegroTimer frameTimer;

	};

}