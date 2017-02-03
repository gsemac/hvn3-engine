#ifndef __IUPDATABLE_H
#define __IUPDATABLE_H

class UpdateEventArgs;

class IUpdatable {

public:
	virtual void Update(UpdateEventArgs e) = 0;

};

#endif