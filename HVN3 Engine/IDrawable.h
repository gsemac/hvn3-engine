#ifndef __IDRAWABLE_H
#define __IDRAWABLE_H

class DrawEventArgs;

class IDrawable {
	
public:
	virtual void Draw(DrawEventArgs e) = 0;
	
};

#endif