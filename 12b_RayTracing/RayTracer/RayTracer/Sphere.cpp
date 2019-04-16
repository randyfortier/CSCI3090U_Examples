#include <cmath>
#include <iostream>

#include "Sphere.h"
#include "Util.h"

float Sphere::intersectionPoint(Ray ray) {
	// Calculate intersection point
	Vector3 oc = ray.origin - this->centre;
	float a = ray.direction.dot(ray.direction);
	float b = 2 * ray.direction.dot(oc);
	float c = oc.dot(oc) - (this->radius * this->radius);

	float discr = b * b - 4 * a * c;

	if (discr < 0.0f) {
		// no intersection
		return NAN;
	} else {
		// intersection!
		return (-b - sqrt(discr)) / (2 * a);
	}
}

Colour Sphere::calculateShading(Vector3 lightPos, Colour lightColour, Ray ray, float t) {
	// Calculate colour at the intersection point
	Vector3 pointOnSurface = ray.origin + (ray.direction * t);

	// calculate a surface normal
	Vector3 surfaceNormal = pointOnSurface - this->centre;
	surfaceNormal.normalize();

	// calculate the light path vector
	Vector3 lightPathVector = lightPos - pointOnSurface;
	lightPathVector.normalize();

	// eye path vector
	Vector3 eyePathVector = ray.origin - pointOnSurface;
	eyePathVector.normalize();

	// diffuse
	float diffuseCoeff = clamp(surfaceNormal.dot(lightPathVector));
	Colour diffuseColour = this->material.colour * diffuseCoeff;

	// specular
	Vector3 reflect = lightPathVector + surfaceNormal * 
		             -2.0f * surfaceNormal.dot(lightPathVector);
	reflect.normalize();

	Vector3 h = lightPathVector + eyePathVector;
	h.normalize();
	float z = max(0.0f, h.dot(surfaceNormal));
	float specPow = (8 + this->material.shininess) / (8 * PI);
	Colour specularColour = lightColour * this->material.specularCoefficient * 
		                 specPow * pow(z, this->material.shininess);

	return diffuseColour + specularColour;
}
