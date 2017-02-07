#ifndef __OBJECT_H
#define __OBJECT_H
#include <initializer_list>
//#include "IDrawable.h"
#include "IUpdatable.h"
#include "ICollidable.h"
#include "ISpriteable.h"
#include "Vector2d.h"

class DrawEventArgs;

class ObjectBase : public IUpdatable, public IDrawable {

public:
	ObjectBase();
	virtual void Draw(DrawEventArgs e) override;
	virtual void Update(UpdateEventArgs e) override;
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
	friend class Room;

public:
	Object();
	Object(float x, float y);
	virtual void Update(UpdateEventArgs e) override;
	virtual void Draw(DrawEventArgs e) override;

	int Depth() const;
	void SetDepth(int depth);
	Vector2d& Velocity();

	::Room* Room();

	// Returns true if the object meets any objects that it is able to collide with at the given position.
	bool PlaceFree(float x, float y, bool notme = true);

	// Moves the object a set distance in a given direction (in degrees) until it comes in contact with an object in its collision filter.
	void MoveContact(float direction, int max_distance);

private:
	float __image_index_timer;
	Vector2d __velocity;
	int __depth;
	::Room* __room;

	void IncrementImageIndex();

};

float DistanceToObject(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b);
float DistanceToObject(const Object* a, const Object* b);
float DistanceToObject(const Object& a, const Object& b);

#endif