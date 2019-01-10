#pragma once

#include "Colour.h"
#include "ColourDefinitions.h"

class Material {
public:
	Colour colour;
	float shininess;
	float diffuseCoefficient;
	float specularCoefficient;

	Material() : colour(COLOUR_WHITE), shininess(0.0f), diffuseCoefficient(1.0f), specularCoefficient(0.0f) {}
	Material(Colour colour) : colour(colour), shininess(0.0f), diffuseCoefficient(1.0f), specularCoefficient(0.0f) {}
	Material(Colour colour, float diffuseCoefficient, float specularCoefficient, float shininess) : colour(colour), shininess(shininess), diffuseCoefficient(diffuseCoefficient), specularCoefficient(specularCoefficient) {}
};

