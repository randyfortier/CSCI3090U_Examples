#include <cmath>
#include <iostream>

#include "Plane.h"
#include "Util.h"

float Plane::intersectionPoint(Ray ray) {
	// Calculate intersection point
	float denominator = ray.direction.dot(this->surfaceNormal);
	if (abs(denominator) > 0.000001f) {
		// intersection!
		Vector3 oc = this->origin - ray.origin;
		float numerator = oc.dot(this->surfaceNormal);
		return numerator / denominator;
	} else {
		// none!
		return -1.0f;
	}
}

Colour Plane::calculateShading(Vector3 lightPos, Colour lightColour, Ray ray, float t) {
	// Calculate colour at the intersection point
	return this->material.colour;
}
