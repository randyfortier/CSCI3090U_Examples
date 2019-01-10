#include "Colour.h"
#include "Util.h"

Colour Colour::operator*(const float rhs) {
	Colour result;

	if (rhs < 0.0f) {
		result.r = 0.0f;
		result.g = 0.0f;
		result.b = 0.0f;
		result.a = 1.0f;
	} else {
		result.r = clamp(this->r * rhs);
		result.g = clamp(this->g * rhs);
		result.b = clamp(this->b * rhs);
		result.a = clamp(this->a);        // don't scale alpha
	}

	return result;
}

Colour Colour::operator+(const Colour rhs) {
	Colour result;

	result.r = clamp(this->r + rhs.r);
	result.g = clamp(this->g + rhs.g);
	result.b = clamp(this->b + rhs.b);
	result.a = clamp(this->a);

	return result;
}

