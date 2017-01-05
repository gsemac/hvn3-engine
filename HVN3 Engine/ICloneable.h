#pragma once

class ICloneable {

public:
	virtual ICloneable* Clone() = 0;

};