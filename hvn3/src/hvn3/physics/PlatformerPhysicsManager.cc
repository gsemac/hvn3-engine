//#include "hvn3/collision/ICollider.h"
//#include "hvn3/collision/ICollisionManager.h"
//#include "hvn3/math/GeometryUtils.h"
//#include "hvn3/math/MathUtils.h"
//#include "hvn3/physics/PlatformerPhysicsManager.h"
//#include "hvn3/utility/Direction8.h"
//
//namespace hvn3 {
//
//	void PlatformerPhysicsManager::OnUpdate(UpdateEventArgs& e) {
//
//		// A local context is required for accessing the collision manager.
//		if (!_context.HasLocalContext())
//			return;
//
//		bool clear_required = false;
//
//		for (auto i = PhysicsBodies().begin(); i != PhysicsBodies().end(); ++i) {
//
//			if (i->use_count() <= 1) {
//				clear_required = true;
//				continue;
//			}
//
//			ICollider* collider = (*i)->GetCollisionBody();
//			Physics::IPhysicsBody* body = i->get();
//
//			float deltaf = static_cast<float>(e.Delta());
//			float xvel = body->LinearVelocity().X();
//			float yvel = body->LinearVelocity().Y();
//			float xdir = static_cast<float>(xvel < 0.0f ? DIRECTION_LEFT : DIRECTION_RIGHT);
//			float ydir = static_cast<float>(yvel < 0.0f ? DIRECTION_UP : DIRECTION_DOWN);
//
//			float _step_height = 0.0f;
//
//			// Note that at higher framerates, gravity will be applied more often, and therefore needs to be scaled immediately.
//			yvel += Gravity().Y() * deltaf;
//
//			// Update horizontal position.
//
//			if (xvel != 0.0f) {
//
//				float xmax = Math::Abs(xvel) * deltaf;
//				float xleft = xmax;
//				PointF pprev = collider->Position();
//				bool slope_success = true;
//
//				while (xleft > 0.0f) {
//
//					//// Check for downward slopes.
//					//while (!Math::IsZero(xleft)) {
//
//					//	PointF ptry = pprev + PointF(Math::Min(1.0f, xleft) * Math::Sign(xvel), 1.0f);
//
//					//	if (_object->Context().GetCollisions().PlaceFree(body, ptry, _platform_category_bits)) {
//
//					//		_object->SetPosition(ptry);
//					//		xleft -= Math::Geometry::PointDistance(ptry, pprev);
//
//					//		break;
//
//					//	}
//					//	else
//					//		slope_success = false;
//
//					//	if (!slope_success)
//					//		break;
//
//					//}
//
//					//if (xleft < 0.0f || Math::IsZero(xleft))
//					//	break;
//
//					// Handle normal horizontal movement.
//
//					_context.Get<COLLISION_MANAGER>().MoveContact(collider, xdir, xmax);
//
//					xleft -= Math::Abs(pprev.x - collider->X());
//					pprev = collider->Position();
//
//					if (xleft < 0.0f || Math::IsZero(xleft))
//						break;
//
//					// Check for upward slopes.
//
//					slope_success = false;
//
//					if (yvel >= 0.0f) {
//
//						for (float i = 0.0f; i <= _step_height; i += 1.0f) {
//
//							PointF ptry = pprev + PointF(Math::Min(1.0f, xleft) * Math::Sign(xvel), -i);
//
//							if (_context.Get<COLLISION_MANAGER>().PlaceFree(*collider, ptry)) {
//
//								collider->SetPosition(ptry);
//								xleft -= Math::Geometry::PointDistance(ptry, pprev);
//
//								slope_success = true;
//
//								break;
//
//							}
//
//						}
//
//					}
//
//					if (!slope_success) {
//						// Hit a wall.
//						xvel = 0.0f;
//						break;
//					}
//
//				}
//
//			}
//
//			// Update vertical position.
//
//			if (_context.Get<COLLISION_MANAGER>().MoveContact(collider, ydir, Math::Abs(yvel) * deltaf)) {
//				yvel = 0.0f;
//				//_setGrounded(true);
//			}
//			else {
//				//_setGrounded(false);
//			}
//
//			// Update the velocity for the next update.
//			body->SetLinearVelocity(Vector2d(xvel, yvel));
//
//		}
//
//		if (clear_required)
//			ClearDeadPhysicsBodies();
//
//	}
//	void PlatformerPhysicsManager::OnContextChanged(ContextChangedEventArgs& e) {
//
//		_context = e.Context();
//
//	}
//
//}