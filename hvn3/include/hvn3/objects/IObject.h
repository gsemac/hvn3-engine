#pragma once
#include "hvn3/core/ApplicationContext.h"
#include "hvn3/core/IContextReceiver.h"
#include "hvn3/collision/ICollidable.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/IPositionable2d.h"
#include "hvn3/objects/ObjectEventArgs.h"
#include "hvn3/objects/ObjectDefs.h"

namespace hvn3 {

	class IObject :
		public IContextReceiver,
		public IUpdatable,
		public IDrawable,
		public ICollidable,
		public IPositionable2d<float> {

	public:
		virtual ~IObject() = default;

		virtual void OnCreate(CreateEventArgs& e) = 0;
		virtual void OnDestroy(DestroyEventArgs& e) = 0;

		virtual ObjectId Id() const = 0;
		virtual int Depth() const = 0;
		virtual void SetDepth(int depth) = 0;
		virtual bool IsDestroyed() const = 0;
		virtual void Destroy() = 0;
		virtual bool IsActive() const = 0;
		virtual void SetActive(bool value) = 0;
		virtual ObjectFlags Flags() const = 0;
		virtual void SetFlags(ObjectFlags flags) = 0;

	};

}