#pragma once
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/WidgetRenderArgs.h"
#include "hvn3/io/IKeyboardListener.h"
#include "hvn3/io/IMouseListener.h"
#include <cstdint>
#include <memory>
#include <list>
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class IWidget;
		class IWidgetRenderer;

		class WidgetManager :
			public IDrawable,
			public IUpdatable,
			public virtual IKeyboardListener,
			public virtual IMouseListener {

			typedef int z_depth_type;

			struct WidgetData {
				WidgetData(std::unique_ptr<IWidget>& widget);
				IWidget& GetRef();
				std::unique_ptr<IWidget> widget;
				WidgetRenderArgs rendererArgs;
				z_depth_type z;
			};

			typedef std::shared_ptr<IWidgetRenderer> renderer_ptr_type;
			typedef std::list<WidgetData> widget_collection_type;

		public:
			WidgetManager();
			WidgetManager(IWidget* owner);
			WidgetManager(renderer_ptr_type& renderer);

			void AddChildManager(IWidget* parent, WidgetManager* manager);
			void RemoveChildManager(IWidget* parent);

			void Add(IWidget* widget);
			void Add(std::unique_ptr<IWidget>& widget);
			void Remove(IWidget* widget);
			void Move(IWidget* widget, WidgetManager* to_manager);
			const widget_collection_type& Widgets() const;
			widget_collection_type& GetWidgets();
			widget_collection_type::size_type Count() const;
			void BringToFront(IWidget* widget);
			void SendToBack(IWidget* widget);

			void SetRenderer(renderer_ptr_type& renderer);
			const IWidgetRenderer& Renderer();
			renderer_ptr_type& GetRenderer();

			const RectangleF& DockableRegion() const;
			void SetDockableRegion(const RectangleF& value);

			void OnDraw(DrawEventArgs& e) override;
			void OnUpdate(UpdateEventArgs& e) override;

			void OnKeyDown(KeyDownEventArgs& e) override;
			void OnKeyPressed(KeyPressedEventArgs& e) override;
			void OnKeyUp(KeyUpEventArgs& e) override;
			void OnKeyChar(KeyCharEventArgs& e) override;

			void OnMouseDown(MouseDownEventArgs& e) override;
			void OnMousePressed(MousePressedEventArgs& e) override;
			void OnMouseReleased(MouseReleasedEventArgs& e) override;
			void OnMouseMove(MouseMoveEventArgs& e) override;
			void OnMouseScroll(MouseScrollEventArgs& e) override;

		private:
			renderer_ptr_type _renderer;
			widget_collection_type _widgets;
			IWidget* _widget_hovered;
			IWidget* _widget_held;
			PointF _last_mouse_position;
			z_depth_type _smallest_z_depth;
			bool _resort_required;
			RectangleF _dockable_region;
			IWidget* _owner;
			std::unordered_map<IWidget*, WidgetManager*> _child_managers;
			bool _update_required_before_draw;

			static IWidget* _widget_focused; // Only one widget can be focused at a time among all managers.

			void _initialize();
			widget_collection_type::iterator _findWidget(IWidget* widget);
			void _renderChildWidgets(DrawEventArgs& e, IWidget* widget);
			void _applyDockStyle(IWidget* widget, RectangleF& region);
			void _setFocused(IWidget* widget);

			// Returns a pointer to the widget renderer assigned to this manager. 
			// IF no renderer has been assigned, assigns and returns the default renderer.
			renderer_ptr_type& _getRenderer();

		};

	}
}