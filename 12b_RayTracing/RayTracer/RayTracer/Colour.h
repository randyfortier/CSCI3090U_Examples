#pragma once
class Colour {
public:
	float r;
	float g;
	float b;
	float a;

	Colour() : r(0.0), g(0.0), b(0.0), a(1.0) {}
	Colour(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	Colour operator*(const float rhs);
	Colour operator+(const Colour rhs);
};
