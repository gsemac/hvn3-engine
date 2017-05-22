#pragma once

namespace hvn3 {

	class ICloneable {

	public:
		virtual ICloneable* Clone() = 0;

	};

}