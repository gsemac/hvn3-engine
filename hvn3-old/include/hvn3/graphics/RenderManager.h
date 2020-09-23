#pragma once

#include "hvn3/events/DrawEventArgs.h"
#include "hvn3/core/ManagerBase.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/events/IEventManager.h"
#include "hvn3/events/EventListenerRegistry.h"
#include "hvn3/rooms/ISceneManager.h"
#include "hvn3/services/di_service_container.h"
#include "hvn3/views/IViewManager.h"

namespace hvn3 {

	class RenderManager {

	public:
		HVN3_INJECT(RenderManager(IEventManager& eventManager, IViewManager* viewManager)) :
			eventManager(&eventManager),
			viewManager(viewManager),
			_scaling_mode(ScalingMode::MaintainAspectRatio) {
		}

		ScalingMode ScalingMode() const {

			return _scaling_mode;

		}
		void SetScalingMode(enum class ScalingMode scalingMode) {

			_scaling_mode = scalingMode;

		}

		void Render(Graphics::Graphics& canvas) {

			// Apply the currently-selected scaling mode before rendering the scene.

			_applyScalingMode(canvas);

			if (!viewManager || viewManager->Count() <= 0) {

				// If the user is not using views, just render everything as-is.

				eventManager->GetListenerRegistry().Dispatch<DrawEventArgs>(canvas);

			}
			else {

				// Render all views.

				Graphics::Transform original_tranform(canvas.GetTransform());
				RectangleF original_clip(canvas.Clip());

				viewManager->ForEach([&](const View& view) {

					if (!view.Enabled())
						return;

					// Set the clipping region according to the view port.

					PointF p1 = view.Port().TopLeft();
					PointF p2 = view.Port().BottomRight();

					p1 = original_tranform.TransformPoint(p1);
					p2 = original_tranform.TransformPoint(p2);

					RectangleF clip(p1, p2);

					canvas.SetClip(clip);

					// Set transform according to view state.		

					Graphics::Transform transform = view.Transform();
					transform.Compose(original_tranform);

					canvas.SetTransform(transform);

					// Dispatch the draw event to all listeners.
					eventManager->GetListenerRegistry().Dispatch<DrawEventArgs>(canvas);

					});

			}

		}

	private:
		IEventManager* eventManager;
		IViewManager* viewManager;
		enum class ScalingMode _scaling_mode;

		void _applyScalingMode(Graphics::Graphics& canvas) {

			// If no scaling mode is set, do nothing.

			if (_scaling_mode == ScalingMode::None)
				return;

			// To decide how to scale, we need to determine the port comprised of all views.

			if (!viewManager || viewManager->Count() <= 0)
				return;

			RectangleF region = viewManager->Viewport();

			// Set the transform and clipping area according to the scaling mode.

			Graphics::Transform transform;

			RectangleF clip = canvas.Clip();
			enum class ScalingMode mode = _scaling_mode;

			if (mode == ScalingMode::Fit)
				mode = (region.Width() > canvas.Width() || region.Height() > canvas.Height()) ? ScalingMode::MaintainAspectRatio : ScalingMode::Fixed;

			// Apply scaling.

			Scale s(canvas.Width() / region.Width(), canvas.Height() / region.Height());

			if (mode == ScalingMode::Full) {

				// Scale to fit the entire canvas.
				transform.Scale(s);

			}
			else if (mode == ScalingMode::MaintainAspectRatio) {

				// Scale to fit the entire region in the canvas while maintaining the aspect ratio.

				float f = Math::Min(s.XScale(), s.YScale());
				float w = region.Width() * f;
				float h = region.Height() * f;

				float pos_x = (w < canvas.Width()) ? (canvas.Width() / 2.0f - w / 2.0f) : 0.0f;
				float pos_y = (h < canvas.Height()) ? (canvas.Height() / 2.0f - h / 2.0f) : 0.0f;

				transform.Scale(f);
				transform.Translate(pos_x, pos_y);

				clip = RectangleF(pos_x, pos_y, Math::Min(w, static_cast<float>(canvas.Width())), Math::Min(h, static_cast<float>(canvas.Height())));

			}
			else if (mode == ScalingMode::Fixed) {

				// Fixed scaling does not scale at all, but centers the content if it is smaller than the canvas.

				float pos_x = (region.Width() < canvas.Width()) ? (canvas.Width() / 2.0f - region.Width() / 2.0f) : 0.0f;
				float pos_y = (region.Height() < canvas.Height()) ? (canvas.Height() / 2.0f - region.Height() / 2.0f) : 0.0f;

				transform.Translate(pos_x, pos_y);
				clip = RectangleF(pos_x, pos_y, Math::Min(region.Width(), (float)canvas.Width()), Math::Min(region.Height(), (float)canvas.Height()));

			}

			canvas.SetTransform(transform);
			canvas.SetClip(clip);

		}

	};

}