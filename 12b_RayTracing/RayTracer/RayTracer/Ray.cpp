#include "Ray.h"

Colour Ray::getDirectionAsColour() {
	Colour colour;

	colour.r = direction.x + 0.5f;
	colour.g = direction.y + 0.5f;
	colour.b = direction.z + 0.5f;
	colour.a = 1.0;

	return colour;
}
