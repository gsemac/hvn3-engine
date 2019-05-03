#pragma once

namespace hvn3 {

	class ApplicationContext;

	class IManager {

	public:
		typedef void interface;

		/*
		StartEventArgs and EndEventArgs both hold a reference to the ApplicationContext object instead of holding their own copy.
		This isn't ideal, but was done to avoid a circular dependency (ApplicationContext needs to know about Application, Application needs to know about StartEventArgs, ...).
		*/

		class StartEventArgs {

		public:
			StartEventArgs(ApplicationContext* context) :
				_context(context) {}
			ApplicationContext& Context() {
				return *_context;
			}

		private:
			ApplicationContext* _context;

		};

		class EndEventArgs {

		public:
			EndEventArgs(ApplicationContext* context) :
				_context(context) {}
			ApplicationContext& Context() {
				return *_context;
			}

		private:
			ApplicationContext* _context;

		};

		virtual ~IManager() = default;

		virtual void OnStart(StartEventArgs& e) = 0;
		virtual void OnEnd(EndEventArgs& e) = 0;

	};

}