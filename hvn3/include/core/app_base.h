#pragma once

#include "core/iapp.h"
#include "events/ievent_filter.h"

namespace hvn3::core {

	class AppBase :
		public IApp {

	public:
		void Show(const DisplayOptions& displayOptions) override;
		int Run() override;
		int Run(const DisplayOptions& displayOptions) override;

	protected:
		class EventFilter :
			public events::IEventFilter {

		public:
			EventFilter(services::DIServiceContainer& services);

			bool PreFilterEvent(events::Event& ev) const override;

		private:
			int displayId;

			bool IsMouseEvent(events::Event& ev) const;
			int GetDisplayId(events::Event& ev) const;

		};

		AppBase() = default;

		virtual void ConfigureServices(services::DIServiceContainer& services);

	private:
		std::vector<services::DIServiceContainer> services;

		void AddWindow(const DisplayOptions& displayOptions);
		int DoEventLoop();

	};

}