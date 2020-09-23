#pragma once
#include <unordered_set>

namespace hvn3 {
	namespace System {

		class InstanceRegistry {

			typedef InstanceRegistry instance_type;
			typedef std::unordered_set<instance_type*> registry_type;

			friend struct Registry;
				
			struct Registry {
				registry_type Instances;
				~Registry() {
					for (auto i = Instances.begin(); i != Instances.end(); ++i)
						(*i)->_deregister_self = false;
				}
			};

		public:
			InstanceRegistry(InstanceRegistry* instance) {
				
				_deregister_self = true;

				_registry.Instances.insert(instance);

			}

		private:
			bool _deregister_self;

			static Registry _registry;

		};

		InstanceRegistry::Registry InstanceRegistry::_registry;

	}
}