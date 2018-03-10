#pragma once
#include "hvn3/rooms/IRoom.h"
#include "hvn3/core/UniquePolymorphicCreateableBase.h"

#pragma warning(push)
#pragma warning(disable:4250)

namespace hvn3 {

	typedef int RoomId;

	template <typename object_manager_type>
	class RoomBase : public IRoom, public SizeableBase<int> {

	public:
		RoomBase(RoomId id, const SizeI& size) :
			SizeableBase(size) {

			_id = id;

			// Set the default background color.
			_background_color = Color::Silver;

			// The current View is 0 because no views have been added yet. When drawing occurs, this value corresponds to the view currently being drawn.
			_persistent = false;
			_set_up = false;

		}
		virtual ~RoomBase() = default;

		virtual void OnUpdate(UpdateEventArgs& e) override {

			_obj_manager->OnBeginUpdate(e);
			_obj_manager->OnUpdate(e);
			_obj_manager->OnEndUpdate(e);

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

		virtual IObjectManager& Objects() override {

			return *_obj_manager;

		}
		virtual IBackgroundManager& Backgrounds() override {

			throw System::NotImplementedException();

		}
		virtual IViewManager& Views() override {

			throw System::NotImplementedException();

		}
		virtual ICollisionManager<IObject>& Collisions() override {

			throw System::NotImplementedException();

		}
		virtual Physics::IPhysicsManager& Physics() override {

			throw System::NotImplementedException();

		}

		const IObjectManager& Objects() const override {

			return *_obj_manager;

		}
		virtual const IBackgroundManager& Backgrounds() const override {

			throw System::NotImplementedException();

		}
		virtual const IViewManager& Views() const override {

			throw System::NotImplementedException();

		}
		virtual const ICollisionManager<IObject>& Collisions() const override {

			throw System::NotImplementedException();

		}
		virtual const Physics::IPhysicsManager& Physics() const override {

			throw System::NotImplementedException();

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

		hvn3::Context Context() override {
			return _context;
		}
		void SetContext(hvn3::Context context) override {

			_context = context;
			
			if (!_obj_manager)
				_obj_manager = std::make_unique<object_manager_type>(_context);
		
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
			_obj_manager->ClearAll();

			// Reset members to default values.
			_background_color = Color::Silver;

			_set_up = false;

		}
		virtual void OnRender(DrawEventArgs& e) override {

			// Clear to background color.
			e.Graphics().Clear(_background_color);

			// Draw all objects.
			_obj_manager->OnDraw(e);

		}

	private:
		std::unique_ptr<object_manager_type> _obj_manager;
		RoomId _id;
		Color _background_color;
		bool _set_up;
		bool _persistent;
		hvn3::Context _context;

	};

}

#pragma warning(pop)