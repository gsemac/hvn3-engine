#ifndef __OBJECT_H
#define __OBJECT_H
#include <initializer_list>
#include "IDrawable.h"
#include "IUpdatable.h"
#include "ICollidable.h"
#include "ISpriteable.h"
#include "Vector2d.h"

class ObjectBase : public IDrawable, public IUpdatable {

public:
	ObjectBase();
	virtual void Draw();
	virtual void Update();
	void Destroy();
	bool IsDestroyed();
	void Deactivate();
	bool IsActive();

private:
	bool __is_destroyed;
	bool __active;

};

class Object : public ObjectBase, public ICollidable, public ISpriteable {
	friend class CollisionGrid;

public:
	Vector2d Velocity;
	int Depth;

	Object();
	Object(float x, float y);
	virtual void Update(float dt) override;
	virtual void Draw() override;

private:
	float __image_index_timer;
	void IncrementImageIndex();

};

float DistanceToObject(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b);
float DistanceToObject(const Object* a, const Object* b);
float DistanceToObject(const Object& a, const Object& b);

#endif