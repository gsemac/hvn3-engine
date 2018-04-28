#pragma once
#include "hvn3/exceptions/Exception.h"
#include "hvn3/rooms/IRoom.h"

#pragma warning(push)
// Disables warning about SizeableBase providing implementation for ISizeable.
#pragma warning(disable:4250) 

namespace hvn3 {
	//typedef ObjectManager object_manager_type;
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

		void OnUpdate(UpdateEventArgs& e) override {

			_obj_manager->OnBeginUpdate(e);
			_obj_manager->OnUpdate(e);
			_obj_manager->OnEndUpdate(e);

		}
		void OnDraw(DrawEventArgs& e) override {

			// Save the current graphics state.
			Graphics::Transform original_tranform(e.Graphics().GetTransform());
			RectangleF original_clip(e.Graphics().Clip());

			// Render the room state.
			OnRender(e);

			// Restore the previous graphics state.
			e.Graphics().SetTransform(original_tranform);
			e.Graphics().SetClip(original_clip);

		}

		IObjectManager& GetObjects() override {

			return *_obj_manager;

		}
		IBackgroundManager& GetBackgrounds() override {
			throw System::NotImplementedException();
		}
		IViewManager& GetViews() override {
			throw System::NotImplementedException();
		}
		ICollisionManager<IObject>& GetCollisions() override {
			throw System::NotImplementedException();
		}
		Physics::IPhysicsManager& GetPhysics() override {
			throw System::NotImplementedException();
		}

		const IObjectManager& Objects() const override {
			return *_obj_manager;
		}
		const IBackgroundManager& Backgrounds() const override {
			throw System::NotImplementedException();
		}
		const IViewManager& Views() const override {
			throw System::NotImplementedException();
		}
		const ICollisionManager<IObject>& Collisions() const override {
			throw System::NotImplementedException();
		}
		const Physics::IPhysicsManager& Physics() const override {
			throw System::NotImplementedException();
		}

		void SetBackgroundColor(const Color& color) override {

			_background_color = color;

		}
		const Color& BackgroundColor() const override {

			return _background_color;

		}
		void SetBackground(const Background& value) override {
			throw System::NotImplementedException();
		}

		const View& CurrentView() const override {
			throw System::NullReferenceException();
		}
		RoomId Id() const override {

			return _id;

		}
		RectangleF VisiblePort() const override {
			return Bounds();
		}
		RectangleF VisibleRegion() const override {
			return Bounds();
		}
		RectangleF Bounds() const override {
			return RectangleF(0.0f, 0.0f, static_cast<float>(Width()), static_cast<float>(Height()));
		}
		bool Persistent() const override {

			return _persistent;

		}
		void SetPersistent(bool value) override {

			_persistent = value;

		}

		void SetContext(hvn3::Context context) override {

			_context = context;

			if (!_obj_manager)
				_obj_manager = std::make_unique<object_manager_type>(_context);

		}
		void OnContextChanged(ContextChangedEventArgs& e) override {}

		System::ManagerBase& GetManagerById(System::ManagerId id) override {
			throw System::NotImplementedException();
		}

	protected:
		void OnRoomEnter(RoomEnterEventArgs& e) override {}
		void OnRoomExit(RoomExitEventArgs& e) override {}
		void OnSetUp() override {

			_set_up = true;

		}
		bool IsSetUp() const {

			return _set_up;

		}
		void OnReset() override {

			// Clear all objects, but don't bother calling their destroy events.
			_obj_manager->Clear();

			// Reset members to default values.
			_background_color = Color::Silver;

			_set_up = false;

		}
		void OnRender(DrawEventArgs& e) override {

			// Clear to background color.
			e.Graphics().Clear(_background_color);

			// Draw all objects.
			_obj_manager->OnDraw(e);

		}

		hvn3::Context Context() override {
			return _context;
		}

	private:
		void _addManager(System::ManagerId id, System::ManagerBase* manager) override {
			throw System::NotImplementedException();
		}

		std::unique_ptr<object_manager_type> _obj_manager;
		RoomId _id;
		Color _background_color;
		bool _set_up;
		bool _persistent;
		hvn3::Context _context;

	};

}

#pragma warning(pop)