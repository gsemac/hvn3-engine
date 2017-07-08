#pragma once
#include "IRoom.h"
#include "UniquePolymorphicCreateableBase.h"
#include "ObjectManager.h"

namespace hvn3 {

	typedef int RoomId;

	class RoomBase : public IRoom, public UniquePolymorphicCreateableBase<RoomBase> {
		friend class RoomController;

	public:
		RoomBase(RoomId id, const SizeI& size);
		virtual ~RoomBase() = default;

		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;
		virtual void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) override;

		virtual const View* CurrentView() const override;

		const IObjectManager* Objects() const override;
		virtual IObjectManager* Objects() override;
		virtual const IBackgroundManager* Backgrounds() const override;
		virtual IBackgroundManager* Backgrounds() override;
		virtual const IViewManager* Views() const override;
		virtual IViewManager* Views() override;
		virtual const ICollisionManager<Object*>* Collisions() const override;
		virtual ICollisionManager<Object*>* Collisions() override;
		virtual const Physics::IPhysicsManager<Object*>* Physics() const override;
		virtual Physics::IPhysicsManager<Object*>* Physics() override;

		RoomId Id() const override;
		virtual RectangleF GetVisibleRegion() override;

		void SetBackgroundColor(const Color& color) override;
		const Color& BackgroundColor() const override;

		bool Persistent() const override;
		void SetPersistent(bool value) override;

	protected:
		virtual void OnRoomEnter(RoomEnterEventArgs& e) override;
		virtual void OnRoomExit(RoomExitEventArgs& e) override;
		virtual void OnSetUp() override;
		virtual void OnReset() override;
		virtual void OnRender(DrawEventArgs& e) override;

	private:
		ObjectManager _obj_manager;

		RoomId _id;
		Color _background_color;
		bool _set_up;
		bool _persistent;

	};

}