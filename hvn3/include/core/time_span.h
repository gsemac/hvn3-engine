#pragma once

namespace hvn3 {

	class TimeSpan {

	public:
		double Seconds() const;

		static TimeSpan FromSeconds(double totalSeconds);

	private:
		TimeSpan(double totalSeconds);

		double totalSeconds;

	};

}