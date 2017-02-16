#ifndef __IUPDATABLE_H
#define __IUPDATABLE_H

class UpdateEventArgs;

class IUpdatable {

public:
	virtual void BeginUpdate(UpdateEventArgs& e);
	virtual void Update(UpdateEventArgs& e) = 0;
	virtual void EndUpdate(UpdateEventArgs& e);

};

#endif