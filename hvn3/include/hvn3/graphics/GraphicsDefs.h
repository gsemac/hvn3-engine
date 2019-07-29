#pragma once
#include "hvn3/utility/BitFlags.h"

namespace hvn3 {

	enum class DisplayFlags {
		OpenGL = 4, // ALLEGRO_OPENGL
		Direct3D = 8, // ALLEGRO_DIRECT3D_INTERNAL
		Resizable = 16, // ALLEGRO_RESIZABLE
		NoBorder = 32, // ALLEGRO_NOFRAME
		OpenGL3 = 128, // ALLEGRO_OPENGL_3_0
		OpenGLForwardCompatible = 256, // ALLEGRO_OPENGL_FORWARD_COMPATIBLE
		FullscreenWindow = 512, // ALLEGRO_FULLSCREEN_WINDOW
		AntiAlias = 1024
	};
	ENABLE_BITFLAG_OPERATORS(DisplayFlags)

		enum class DisplayOption {
		VsyncEnabled
	};

	enum class ScalingMode {
		// No scaling is performed.
		None,
		//Display contents are not scaled, but are centered if the contents are smaller than the display area.
		Fixed,
		// Display contents are scaled down if larger than the display area, but centered if the contents are smaller than the display area.
		Fit,
		// Display contents are stretched to fit the exact size of the display area.
		Full,
		// Display contents are scaled to fit the display area while maintaining the aspect ratio.
		MaintainAspectRatio
	};

	namespace Graphics {

		enum class BlendOperation {
			Normal,
			Add,
			Subtract,
			Max,
			Invert,
			SourceMinusDestination,
			DestinationMinusSource
		};

		enum class BlendMode {
			Zero,
			One,
			Alpha,
			InverseAlpha,
			SourceColor,
			DestinationColor,
			InverseSourceColor,
			InverseDestinationColor,
			ConstColor,
			InverseConstColor
		};

		enum class DashStyle {
			Dash,
			Solid
		};

	}
}