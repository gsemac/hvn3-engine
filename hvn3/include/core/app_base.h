#pragma once

#include "core/iapp.h"

namespace hvn3::core {

	class AppBase :
		public IApp {

	public:
		void Run(const DisplayOptions& displayOptions) override;

	protected:
		AppBase() = default;

		virtual void ConfigureServices(services::DIServiceContainer& services);

	private:
		std::vector<services::DIServiceContainer> services;

		void InitializeApp(const DisplayOptions& displayOptions);
		void Run();

	};

}