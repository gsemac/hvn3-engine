#pragma once
#include "ISizeable.h"
#include "IUpdatable.h"
#include "IDrawable.h"
#include "ObjectManager.h"
#include "RoomEnterEventArgs.h"
#include "RoomExitEventArgs.h"
#include "Color.h"

typedef int RoomId;

class ObjectManager;

class RoomBase : public IUpdatable, public IDrawable, public ISizeable {
	friend class RoomController;

public:
	RoomBase(unsigned int width, unsigned int height);
	RoomBase(unsigned int width, unsigned int height, std::unique_ptr<IObjectManager>& object_manager);

	virtual void Update(UpdateEventArgs& e) override;
	virtual void Draw(DrawEventArgs& e) override;

	void SetBackgroundColor(const Color& color);
	const Color& BackgroundColor() const;
	IObjectManager* ObjectManager();
	virtual ::RoomId Id() const;
	bool Persistent() const;
	void SetPersistent(bool value);

	template<typename T, typename ... Args>
	static std::unique_ptr<RoomBase> Create(Args &&... args) {

		return std::make_unique<T>(std::forward<Args>(args)...);

	}

protected:
	virtual void OnRoomEnter(RoomEnterEventArgs& e);
	virtual void OnRoomExit(RoomExitEventArgs& e);
	virtual void SetUp();
	virtual void Reset();
	virtual void Render(DrawEventArgs& e);

private:
	std::unique_ptr<IObjectManager> _obj_manager;

	Color _background_color;
	bool _set_up;
	bool _persistent;

};