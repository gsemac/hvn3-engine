#pragma once

#include "hvn3/core/ApplicationContext.h"
#include "hvn3/core/IApplicationContextReceiver.h"
#include "hvn3/collision/ICollidable.h"
#include "hvn3/core/IPositionable2d.h"
#include "hvn3/ecs/EcsDefs.h"
#include "hvn3/utility/TypeIndexer.h"

namespace hvn3 {

	class IObject :
		public IApplicationContextReceiver {

	protected:
		typedef TypeIndexer<struct object_indexer_family> object_indexer; // used to assign indices to object types 

		class CreateEventArgs :
			public EventArgs {};

		class DestroyEventArgs :
			public EventArgs {};

		class AnimationEndEventArgs :
			public EventArgs {

		public:
			AnimationEndEventArgs(int endIndex);

			int EndIndex() const;

		private:
			int _end_index;

		};

		virtual void OnCreate(CreateEventArgs& e) = 0;
		virtual void OnDestroy(DestroyEventArgs& e) = 0;

	public:
		typedef typename object_indexer::index_type object_id;

		virtual ~IObject() = default;

		virtual object_id Id() const = 0;
		virtual ecs::EntityId EntityId() const = 0;
		virtual bool IsDestroyed() const = 0;
		virtual void Destroy() = 0;
		virtual bool IsActive() const = 0;
		virtual void SetActive(bool value) = 0;

	};

}