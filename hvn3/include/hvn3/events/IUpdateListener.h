#pragma once

#include "hvn3/core/ApplicationContext.h"

namespace hvn3 {

	class IUpdateListener {

	public:
		class UpdateEventArgs {

		public:
			UpdateEventArgs(const ApplicationContext& context, double deltaTime) :
				_context(context),
				_delta_time(deltaTime) {}

			// Returns the number of seconds elapsed since the last update.
			double Delta() const { return _delta_time; }

		private:
			ApplicationContext _context;
			double _delta_time;

		};

		virtual void OnBeginUpdate(UpdateEventArgs& e) = 0;
		virtual void OnUpdate(UpdateEventArgs& e) = 0;
		virtual void OnEndUpdate(UpdateEventArgs& e) = 0;
	};

}