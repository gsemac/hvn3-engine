#include "DrawEventArgs.h"

DrawEventArgs::DrawEventArgs(Drawing::Graphics& graphics) : __graphics(graphics) {}
Drawing::Graphics& DrawEventArgs::Graphics() {

	return __graphics;

}