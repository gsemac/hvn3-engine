#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/utility/StringUtils.h"
#include <string>
#include <sstream>
#include <type_traits>
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		template <typename T>
		struct WidgetStyleProperty {
			typedef T type;
			WidgetStyleProperty() {
				_initMembers();
			}
			WidgetStyleProperty(bool inherit) {
				_initMembers();
				this->inherit = inherit;
			}
			WidgetStyleProperty(const T& value) :
				value(value) {
				_initMembers();
			}
			WidgetStyleProperty(T&& value) :
				value(std::move(value)) {
				_initMembers() :
			}
			WidgetStyleProperty(T value, bool inherit) :
				value(value) {
				_initMembers();
				this->inherit = inherit;
			}
			T value;
			bool inherit;
			float transitionDuration;
		private:
			void _initMembers() {
				inherit = false;
				transitionDuration = 0.0f;
			}
		};

		class WidgetStyle {

		public:
			struct WidgetStyleBackgroundProperty {
				enum {
					LEFT,
					RIGHT,
					TOP,
					BOTTOM
				};
				WidgetStyleBackgroundProperty();
				WidgetStyleBackgroundProperty(const std::string& str);
				Color left;
				Color right;
				Color top;
				Color bottom;
				bool setColors[4];
			};

			WidgetStyle();

			std::string GetPropertyString(const std::string& name);

			template <typename T>
			WidgetStyleProperty<T> GetProperty(const std::string& name) {
				auto it = _properties.find(name);
				if (it == _properties.end())
					return WidgetStyleProperty<T>();
				if (_isInherit(it->second))
					return WidgetStyleProperty<T>(true);
				return WidgetStyleProperty<T>(std::move(StringUtils::ConvertFromString<T>(it->second)));
			}
			template <>
			WidgetStyleProperty<WidgetStyleBackgroundProperty>GetProperty(const std::string& name) {
				return WidgetStyleProperty<WidgetStyleBackgroundProperty>(_background);
			}

			template <typename T>
			WidgetStyleProperty<T> GetPropertyOrDefault(const std::string& name, const T& def) {
				auto it = _properties.find(name);
				if (it == _properties.end())
					return WidgetStyleProperty<T>(def);
				if (_isInherit(it->second))
					return WidgetStyleProperty<T>(def, true);
				return WidgetStyleProperty<T>(std::move(StringUtils::ConvertFromString<T>(it->second)));
			}

			void SetProperty(const std::string& name, const std::string& value);

		private:
			WidgetStyleBackgroundProperty _background;
			std::unordered_map<std::string, std::string> _properties;

			bool _isInherit(const std::string& value);

		};

	}
}