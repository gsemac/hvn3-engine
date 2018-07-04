#pragma once
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/rooms/RoomBase.h"

namespace hvn3 {

	RoomBase::RoomBase(RoomId id, const SizeI& size) :
		SizeableBase(size) {

		_id = id;

		// Set the default background color.
		_background_color = Color::Silver;

		// The current View is 0 because no views have been added yet. When drawing occurs, this value corresponds to the view currently being drawn.
		_persistent = false;
		_set_up = false;

	}

	void RoomBase::OnUpdate(UpdateEventArgs& e) {}
	void RoomBase::OnDraw(DrawEventArgs& e) {

		// Save the current graphics state.
		Graphics::Transform original_tranform(e.Graphics().GetTransform());
		RectangleF original_clip(e.Graphics().Clip());

		// Render the room state.
		OnRender(e);

		// Restore the previous graphics state.
		e.Graphics().SetTransform(original_tranform);
		e.Graphics().SetClip(original_clip);

	}

	IObjectManager& RoomBase::GetObjects() {
		throw System::NotImplementedException();
	}
	IBackgroundManager& RoomBase::GetBackgrounds() {
		throw System::NotImplementedException();
	}
	IViewManager& RoomBase::GetViews() {
		throw System::NotImplementedException();
	}
	ICollisionManager<IObject>& RoomBase::GetCollisions() {
		throw System::NotImplementedException();
	}
	Physics::IPhysicsManager& RoomBase::GetPhysics() {
		throw System::NotImplementedException();
	}

	const IObjectManager& RoomBase::Objects() const {
		throw System::NotImplementedException();
	}
	const IBackgroundManager& RoomBase::Backgrounds() const {
		throw System::NotImplementedException();
	}
	const IViewManager& RoomBase::Views() const {
		throw System::NotImplementedException();
	}
	const ICollisionManager<IObject>& RoomBase::Collisions() const {
		throw System::NotImplementedException();
	}
	const Physics::IPhysicsManager& RoomBase::Physics() const {
		throw System::NotImplementedException();
	}

	void RoomBase::SetBackgroundColor(const Color& color) {
		_background_color = color;
	}
	const Color& RoomBase::BackgroundColor() const {
		return _background_color;
	}
	void RoomBase::SetBackground(const Background& value) {
		throw System::NotImplementedException();
	}

	const View& RoomBase::CurrentView() const {
		throw System::NullReferenceException();
	}
	RoomId RoomBase::Id() const {
		return _id;
	}
	RectangleF RoomBase::VisiblePort() const {
		return Bounds();
	}
	RectangleF RoomBase::VisibleRegion() const {
		return Bounds();
	}
	RectangleF RoomBase::Bounds() const {
		return RectangleF(0.0f, 0.0f, static_cast<float>(Width()), static_cast<float>(Height()));
	}
	bool RoomBase::Persistent() const {
		return _persistent;
	}
	void RoomBase::SetPersistent(bool value) {
		_persistent = value;
	}


	hvn3::Context RoomBase::Context() {
		return _context;
	}
	void RoomBase::SetContext(hvn3::Context context) {

		_context = context;

		OnContextChanged(ContextChangedEventArgs(context));

	}
	void RoomBase::OnContextChanged(ContextChangedEventArgs& e) {}
	void RoomBase::OnEnter(RoomEnterEventArgs& e) {}
	void RoomBase::OnExit(RoomExitEventArgs& e) {}
	void RoomBase::OnCreate() {
		_set_up = true;
	}
	bool RoomBase::IsReady() const {
		return _set_up;
	}
	void RoomBase::OnReset() {

		// Reset members to default values.
		_background_color = Color::Silver;

		_set_up = false;

	}
	void RoomBase::OnRender(DrawEventArgs& e) {
		e.Graphics().Clear(_background_color);
	}


	void RoomBase::_addManager(ManagerId id, std::shared_ptr<ManagerBase>& manager) {

		auto iter = _registered_managers.find(id);

		//// This has been commented-out so that the user can replace existing managers with new ones.
		//if (iter != _registered_managers.end())
		//	throw System::ArgumentException("A manager with this ID already exists.");

		_registered_managers[id] = std::move(manager);

	}
	ManagerBase* RoomBase::_getManager(ManagerId id) const {

		auto iter = _registered_managers.find(id);

		if (iter == _registered_managers.end())
			return nullptr;

		return iter->second.get();

	}

}