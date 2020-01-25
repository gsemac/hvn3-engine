#pragma once

#include "hvn3/collision/ICollidable.h"
#include "hvn3/core/IPositionable2d.h"
#include "hvn3/ecs/EcsDefs.h"
#include "hvn3/ecs/Entity.h"
#include "hvn3/utility/TypeIndexer.h"

namespace hvn3 {

	class IObject {

	protected:
		typedef TypeIndexer<struct object_indexer_family> object_indexer; // used to assign indices to object types 

	public:
		class CreateEventArgs :
			public EventArgs {
		public:
			using EventArgs::EventArgs;
		};

		class DestroyEventArgs :
			public EventArgs {
		public:
			using EventArgs::EventArgs;
		};

		class AnimationEndEventArgs :
			public EventArgs {

		public:
			AnimationEndEventArgs(int endIndex);

			int EndIndex() const;

		private:
			int _end_index;

		};

		typedef object_indexer::index_type object_id;

		static const object_indexer::index_type NULL_OBJECT_ID;

		virtual ~IObject() = default;

		virtual object_id Id() const = 0;
		virtual ecs::Entity Entity() = 0;
		virtual bool IsActive() const = 0;
		virtual void SetActive(bool value) = 0;

		virtual void HandleEvent(CreateEventArgs& e) = 0;
		virtual void HandleEvent(DestroyEventArgs& e) = 0;

	};

}