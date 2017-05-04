#pragma once
#include "UpdateEventArgs.h"

class IExtension {

public:
	virtual void Initialize() = 0;
	virtual void Update(UpdateEventArgs& e) = 0;
	virtual void Shutdown() = 0;

private:

};