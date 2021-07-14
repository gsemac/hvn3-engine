#pragma once

#include "core/iapp.h"
#include "events/ievent_filter.h"

namespace hvn3 {

	class AppBase :
		public IApp {

	public:
		void Show(const io::DisplayOptions& displayOptions) override;

		int Run(const io::DisplayOptions& displayOptions) override;
		int Run() override;

	protected:
		class EventFilter :
			public events::IEventFilter {

		public:
			EventFilter(ServiceProvider& services);

			bool PreFilterEvent(events::Event& ev) const override;

		private:
			int displayId;

			bool IsMouseEvent(events::Event& ev) const;
			int GetDisplayId(events::Event& ev) const;

		};

		AppBase() = default;

		virtual void ConfigureServices(ServiceProvider& services);

	private:
		std::vector<ServiceProvider> services;

		void AddDisplay(const io::DisplayOptions& displayOptions);

		int DoEventLoop();

	};

}