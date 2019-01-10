#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#pragma once

class UVSphere {
private:
	unsigned int numVerticalSegments;
	unsigned int numHorizontalSegments;
	float radius;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;
	unsigned int numVertices;
	unsigned int numTriangles;
	unsigned int* triangleIndices;

public:
	UVSphere(float radius, unsigned int vertRes, unsigned int horizRes) : radius(radius), numVerticalSegments(vertRes), numHorizontalSegments(horizRes) {}

	glm::vec3* getPositions();
	glm::vec2* getTextureCoords();
	glm::vec3* getNormals();

	unsigned int getNumVertices();
	unsigned int getNumTriangles();

	unsigned int* getTriangleIndices();

	float getRadius();
	unsigned int getNumVerticalSegments();
	unsigned int getNumHorizontalSegments();

	void save(const std::string filename);
};
