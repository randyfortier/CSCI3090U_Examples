#pragma once

#include "Vector3.h"
#include "Colour.h"

class Ray {
public:
	Vector3 origin;
	Vector3 direction;

	Ray(Vector3 origin, Vector3 direction) : origin(origin), direction(direction) {}

	Colour getDirectionAsColour();
};

