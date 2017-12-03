#pragma once
#include "hvn3/rooms/IRoom.h"
#include "hvn3/core/UniquePolymorphicCreateableBase.h"

namespace hvn3 {

	typedef int RoomId;

	template <typename object_manager_type>
	class RoomBase : public IRoom {

	public:
		RoomBase(RoomId id, const SizeI& size) :
			IRoom(size) {

			_id = id;

			// Set the default background color.
			_background_color = Color::Silver;

			// The current View is 0 because no views have been added yet. When drawing occurs, this value corresponds to the view currently being drawn.
			_persistent = false;
			_set_up = false;

		}
		virtual ~RoomBase() = default;

		virtual void OnUpdate(UpdateEventArgs& e) override {

			_obj_manager.OnBeginUpdate(e);
			_obj_manager.OnUpdate(e);
			_obj_manager.OnEndUpdate(e);

		}
		virtual void OnDraw(DrawEventArgs& e) override {

			// Save the current graphics state.
			Graphics::Transform original_tranform(e.Graphics().GetTransform());
			RectangleF original_clip(e.Graphics().Clip());

			// Render the room state.
			OnRender(e);

			// Restore the previous graphics state.
			e.Graphics().SetTransform(original_tranform);
			e.Graphics().SetClip(original_clip);

		}
		virtual void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) override {}

		virtual const View* CurrentView() const override {

			return nullptr;

		}

		const IObjectManager* Objects() const override {

			return &_obj_manager;

		}
		virtual IObjectManager* Objects() override {

			return &_obj_manager;

		}
		virtual const IBackgroundManager* Backgrounds() const override {

			return nullptr;

		}
		virtual IBackgroundManager* Backgrounds() override {

			return nullptr;

		}
		virtual const IViewManager* Views() const override {

			return nullptr;

		}
		virtual IViewManager* Views() override {

			return nullptr;

		}
		virtual const ICollisionManager* Collisions() const override {

			return nullptr;

		}
		virtual ICollisionManager* Collisions() override {

			return nullptr;

		}
		virtual const Physics::IPhysicsManager<Object*>* Physics() const override {

			return nullptr;

		}
		virtual Physics::IPhysicsManager<Object*>* Physics() override {

			return nullptr;

		}

		RoomId Id() const override {

			return _id;

		}
		virtual RectangleF GetVisibleRegion() override {

			return RectangleF(0.0f, 0.0f, static_cast<float>(Width()), static_cast<float>(Height()));

		}
		virtual RectangleF Bounds() const override {

			return RectangleF(0.0f, 0.0f, static_cast<float>(Width()), static_cast<float>(Height()));

		}

		void SetBackgroundColor(const Color& color) override {

			_background_color = color;

		}
		const Color& BackgroundColor() const override {

			return _background_color;

		}

		bool Persistent() const override {

			return _persistent;

		}
		void SetPersistent(bool value) override {

			_persistent = value;

		}

	protected:
		virtual void OnRoomEnter(RoomEnterEventArgs& e) override {}
		virtual void OnRoomExit(RoomExitEventArgs& e) override {}
		virtual void OnSetUp() override {
		
			_set_up = true;

		}
		bool IsSetUp() const {

			return _set_up;

		}
		virtual void OnReset() override {

			// Clear all objects, but don't bother calling their destroy events.
			_obj_manager.ClearAll();

			// Reset members to default values.
			_background_color = Color::Silver;

			_set_up = false;

		}
		virtual void OnRender(DrawEventArgs& e) override {

			// Clear to background color.
			e.Graphics().Clear(_background_color);

			// Draw all objects.
			_obj_manager.OnDraw(e);

		}

	private:
		object_manager_type _obj_manager;

		RoomId _id;
		Color _background_color;
		bool _set_up;
		bool _persistent;

	};

}