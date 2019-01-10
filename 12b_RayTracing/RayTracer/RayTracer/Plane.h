#pragma once

#include "Primitive.h"
#include "Material.h"

class Plane : public Primitive {
public:
	Vector3 origin;
	Vector3 surfaceNormal;
	Material material;
	float ambientCoefficient = 0.1f;

	Plane(Vector3 origin, Vector3 surfaceNormal, Material material) : origin(origin), surfaceNormal(surfaceNormal), material(material) {}

	virtual float intersectionPoint(Ray ray);
	virtual Colour calculateShading(Vector3 lightPos, Colour lightColour, Ray ray, float t);
};

