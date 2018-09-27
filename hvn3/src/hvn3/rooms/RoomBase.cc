#pragma once
#include "hvn3/backgrounds/IBackgroundManager.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/physics/IPhysicsManager.h"
#include "hvn3/rooms/RoomBase.h"
#include "hvn3/tilesets/TileManager.h"
#include "hvn3/views/IViewManager.h"

namespace hvn3 {

	RoomBase::RoomBase(RoomId id, const SizeI& size) :
		SizeableBase(size),
		_local([this](IManager* m) {
		ContextChangedEventArgs args(_context);
		m->OnContextChanged(args);
	}) {

		_id = id;
		_context = Context(nullptr, &_local);

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

	IObjectManager& RoomBase::Objects() {
		return _local.Get<OBJECT_MANAGER>();
	}
	IBackgroundManager& RoomBase::Backgrounds() {
		return _local.Get<BACKGROUND_MANAGER>();
	}
	IViewManager& RoomBase::Views() {
		return _local.Get<VIEW_MANAGER>();
	}
	ICollisionManager& RoomBase::Collisions() {
		return _local.Get<COLLISION_MANAGER>();
	}
	Physics::IPhysicsManager& RoomBase::Physics() {
		return _local.Get<PHYSICS_MANAGER>();
	}
	TileManager& RoomBase::Tiles() {
		return _local.Get<TILE_MANAGER>();
	}

	const IObjectManager& RoomBase::Objects() const {
		return _local.Get<OBJECT_MANAGER>();
	}
	const IBackgroundManager& RoomBase::Backgrounds() const {
		return _local.Get<BACKGROUND_MANAGER>();
	}
	const IViewManager& RoomBase::Views() const {
		return _local.Get<VIEW_MANAGER>();
	}
	const ICollisionManager& RoomBase::Collisions() const {
		return _local.Get<COLLISION_MANAGER>();
	}
	const Physics::IPhysicsManager& RoomBase::Physics() const {
		return _local.Get<PHYSICS_MANAGER>();
	}
	const TileManager& RoomBase::Tiles() const {
		return _local.Get<TILE_MANAGER>();
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

	void RoomBase::OnContextChanged(ContextChangedEventArgs& e) {

		_context = Context(e.Context(), &_local);

		ContextChangedEventArgs args(_context);

		// Update the contexts for all local managers.
		for (auto i = _local.begin(); i != _local.end(); ++i)
			i->second->OnContextChanged(args);

	}
	void RoomBase::OnEnter(RoomEnterEventArgs& e) {}
	void RoomBase::OnExit(RoomExitEventArgs& e) {}
	void RoomBase::OnCreate(RoomCreateEventArgs& e) {
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

	const ManagerRegistry& RoomBase::Local() const {
		return _local;
	}
	ManagerRegistry& RoomBase::Local() {
		return _local;
	}
	const Context& RoomBase::GetContext() const {
		return _context;
	}

}