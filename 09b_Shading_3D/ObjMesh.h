#include <string>
#include <vector>

#pragma once

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Vector2 {
	float u;
	float v;
};

class ObjMesh {
private:
	unsigned int numVertices;
	unsigned int numTriangles;
	unsigned int numIndexedVertices;
	std::vector<unsigned int> triangleIndices;
	std::vector<Vector3> indexedPositions;
	std::vector<Vector2> indexedTextureCoords;
	std::vector<Vector3> indexedNormals;
	Vector3 centre;
	Vector3 dimensions;

public:
	ObjMesh();
	
	void load(const std::string filename, const bool autoCentre, const bool autoNormalize);

	Vector3* getIndexedPositions();
	Vector2* getIndexedTextureCoords();
	Vector3* getIndexedNormals();

	unsigned int getNumVertices();
	unsigned int getNumIndexedVertices();
	unsigned int getNumTriangles();

	unsigned int* getTriangleIndices();

	Vector3 getCentre();
	Vector3 getDimensions();
};
