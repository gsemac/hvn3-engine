#pragma once
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/objects/ObjectEventArgs.h"
#include "hvn3/objects/ObjectTypeDefs.h"
#include "hvn3/utility/BitFlags.h"

namespace hvn3 {

	class ObjectBase : public virtual IUpdatable, public virtual IDrawable {

	public:
		ObjectBase();

		virtual void OnCreate(CreateEventArgs& e);
		virtual void OnDestroy(DestroyEventArgs& e);
		virtual void OnDraw(DrawEventArgs& e) override;
		virtual void OnUpdate(UpdateEventArgs& e) override;
		
		void Destroy();
		bool IsDestroyed() const;
		void Activate();
		void Deactivate();
		bool IsActive() const;

		ObjectFlags Flags() const;
		void SetFlags(ObjectFlags flags);

	private:
		bool _is_destroyed;
		bool _active;
		ObjectFlags _flags;

	};

}