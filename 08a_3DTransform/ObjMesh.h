#include <string>
#include <vector>

#include <glm/glm.hpp>

#pragma once

class ObjMesh {
private:
	unsigned int numVertices;
	unsigned int numTriangles;
	unsigned int numIndexedVertices;
	std::vector<unsigned int> triangleIndices;
	std::vector<glm::vec3> indexedPositions;
	std::vector<glm::vec2> indexedTextureCoords;
	std::vector<glm::vec3> indexedNormals;
	glm::vec3 centre;
	glm::vec3 dimensions;

public:
	ObjMesh();

	void load(const std::string filename, const bool autoCentre, const bool autoNormalize);

	glm::vec3* getIndexedPositions();
	glm::vec2* getIndexedTextureCoords();
	glm::vec3* getIndexedNormals();

	unsigned int getNumVertices();
	unsigned int getNumIndexedVertices();
	unsigned int getNumTriangles();

	unsigned int* getTriangleIndices();

	glm::vec3 getCentre();
	glm::vec3 getDimensions();
};
