#pragma once
#include "core/IUpdatable.h"
#include "core/IDrawable.h"
#include "core/UniqueCreateableBase.h"
#include "graphics/DisplaySizeChangedEventArgs.h"
#include "graphics/Color.h"
#include "physics/IPhysicsManager.h"
#include "math/Rectangle.h"
#include "rooms/RoomEnterEventArgs.h"
#include "rooms/RoomExitEventArgs.h"
#include "collision/ICollisionManager.h"

namespace hvn3 {

	typedef int RoomId;

	class View;
	class IObjectManager;
	class IBackgroundManager;
	class IViewManager;
	class Object;
	
	namespace System {
		class RoomController;
	}

	class IRoom : public IUpdatable, public IDrawable, public SizeableBase<int> {
		friend class System::RoomController;

	public:
		IRoom(const SizeI& size);
		virtual ~IRoom() = default;

		virtual void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) = 0;

		virtual void SetBackgroundColor(const Color& color) = 0;
		virtual const Color& BackgroundColor() const = 0;

		virtual const View* CurrentView() const = 0;

		virtual const IObjectManager* Objects() const = 0;
		virtual IObjectManager* Objects() = 0;
		virtual const IBackgroundManager* Backgrounds() const = 0;
		virtual IBackgroundManager* Backgrounds() = 0;
		virtual const IViewManager* Views() const = 0;
		virtual IViewManager* Views() = 0;
		virtual const ICollisionManager* Collisions() const = 0;
		virtual ICollisionManager* Collisions() = 0;
		virtual const Physics::IPhysicsManager<Object*>* Physics() const = 0;
		virtual Physics::IPhysicsManager<Object*>* Physics() = 0;

		virtual RoomId Id() const = 0;
		virtual bool Persistent() const = 0;
		virtual void SetPersistent(bool value) = 0;
		virtual RectangleF GetVisibleRegion() = 0;
		virtual RectangleF Bounds() const = 0;

		virtual void Restart() = 0;

	protected:
		virtual void OnRoomEnter(RoomEnterEventArgs& e) = 0;
		virtual void OnRoomExit(RoomExitEventArgs& e) = 0;
		virtual void OnSetUp() = 0;
		virtual bool IsSetUp() const = 0;
		virtual void OnReset() = 0;
		virtual void OnRender(DrawEventArgs& e) = 0;

	};

	typedef std::unique_ptr<IRoom> RoomPtr;

}