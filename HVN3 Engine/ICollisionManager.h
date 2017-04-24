#pragma once

class UpdateEventArgs;

class ICollisionManager {

public:
	virtual void Update(UpdateEventArgs& e) = 0;

};