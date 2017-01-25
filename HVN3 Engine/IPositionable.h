#ifndef __IPOSITIONABLE_H
#define __IPOSITIONABLE_H

class IPositionable {

public:
	IPositionable();
	IPositionable(float x, float y);
	float X() const;
	float Y() const;
	virtual void SetX(float x);
	virtual void SetY(float y);
	void SetXY(float x, float y);
	void TranslateX(float x_offset);
	void TranslateY(float y_offset);
	void Translate(float x_offset, float y_offset);

	//void Move(float x, float y);

private:
	float __x, __y;

};

#endif