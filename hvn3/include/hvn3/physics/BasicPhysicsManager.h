#pragma once
#include "hvn3/core/IContextReceiver.h"
#include "hvn3/physics/PhysicsBodyBase.h"
#include "hvn3/physics/PhysicsManagerBase.h"
#include <unordered_map>
#include <vector>

namespace hvn3 {
	namespace Physics {

		class BasicPhysicsManager : 
			public PhysicsManagerBase<PhysicsBodyBase> {

			struct Contact {
				IPhysicsBody* body;
				Vector2d velocity;
			};

		public:
			BasicPhysicsManager();

			IPhysicsBody* CreateBody(ICollider* body) override;
			void OnUpdate(UpdateEventArgs& e) override;
			void Clear() override;

		protected:
			void OnContextChanged(ContextChangedEventArgs& e) override;

		private:
			std::unordered_map<ICollider*, IPhysicsBody*> _body_lookup_table;
			std::vector<Contact> _contacts;
			Context _context;

			IPhysicsBody* _lookupBody(const ICollider* key);

		};

	}
}