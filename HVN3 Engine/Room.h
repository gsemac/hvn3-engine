#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Point.h"
#include "IUpdatable.h"
#include "Background.h"
#include "Size.h"
#include "View.h"
#include "CollisionGrid.h"
#include "CollisionManager.h"
#include "Graphics.h"
#include "UpdateEventArgs.h"
#include "ObjectManager.h"
#include "RoomEnterEventArgs.h"
#include "RoomExitEventArgs.h"
#include "BackgroundManager.h"
#include "ViewManager.h"

class Object;
class Runner;

typedef int RoomId;

class Room : public IUpdatable, public IDrawable, public ISizeable {
	friend class RoomController;

public:
	Room(unsigned int width, unsigned int height);

	virtual void Update(UpdateEventArgs& e) override;
	virtual void Draw(DrawEventArgs& e) override;

	void SetBackgroundColor(const Color& color);
	const Color& BackgroundColor() const;
	const View* CurrentView();
	ObjectManager* ObjectManager();
	ViewManager* ViewManager();
	virtual ::RoomId Id() const;
	bool Persistent() const;
	void SetPersistent(bool value);

	template<typename T, typename ... Args>
	static std::unique_ptr<Room> Create(Args &&... args) {

		return std::make_unique<T>(std::forward<Args>(args)...);

	}

protected:
	virtual void OnRoomEnter(RoomEnterEventArgs& e);
	virtual void OnRoomExit(RoomExitEventArgs& e);
	virtual void SetUp();
	virtual void Reset();
	virtual void Render(DrawEventArgs& e);

private:
	::ObjectManager _obj_manager;
	::ViewManager _view_manager;
	size_t _rendering_view;
	Color _background_color;
	bool _set_up;
	bool _persistent;

};