#pragma once

#include "Primitive.h"
#include "Vector3.h"
#include "Material.h"

class Sphere : public Primitive {
public:
	Vector3 centre;
	float radius;
	Material material;
	float ambientCoefficient = 0.1f;

	Sphere(Vector3 centre, float radius, Material material) : centre(centre), radius(radius), material(material) {}

	virtual float intersectionPoint(Ray ray);
	virtual Colour calculateShading(Vector3 lightPos, Colour lightColour, Ray ray, float t);
};

