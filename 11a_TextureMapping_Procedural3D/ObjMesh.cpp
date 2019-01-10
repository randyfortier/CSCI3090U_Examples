#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>

#include "ObjMesh.h"

struct Triangle {
	int a, b, c;
};
typedef struct Triangle Triangle;

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
		return !isspace(ch);
	}));
}

static inline void rtrim(std::string &s) {
	s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
		return !isspace(ch);
	}).base(), s.end());
}

static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

ObjMesh::ObjMesh() {
	this->numVertices = 0;
	this->numTriangles = 0;
}

void ObjMesh::load(const std::string filename, const bool autoCentre = false, const bool autoNormalize = false) {
	std::cout << "Loading " << filename.c_str() << "..." << std::endl;

	std::ifstream fileIn(filename);

	if (!fileIn.is_open()) {
		return;
	}

	std::vector<Vector3> vertexPositions;
	std::vector<Vector2> vertexTextureCoords;
	std::vector<Vector3> vertexNormals;
	std::vector<unsigned int> positionIndices;
	std::vector<unsigned int> textureCoordIndices;
	std::vector<unsigned int> normalIndices;

	numTriangles = 0;

	// for auto-centering
	float totalX = 0.0f;
	float totalY = 0.0f;
	float totalZ = 0.0f;

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::min();

	std::string line;
	while (getline(fileIn, line)) {
		trim(line);
		if (line.size() > 0) {
			// skip empty lines
			std::stringstream lineIn(line);

			std::string typeIdentifier;
			lineIn >> typeIdentifier;

			if (typeIdentifier == "v") {
				// a vertex position
				float x, y, z;
				lineIn >> x >> y >> z;

				Vector3 v;
				v.x = x;
				v.y = y;
				v.z = z;

				// for auto-centering
				totalX += x;
				totalY += y;
				totalZ += z;

				// for auto-normalization
				if (x < minX) {
					minX = x;
				}
				if (x > maxX) {
					maxX = x;
				}
				if (y < minY) {
					minY = y;
				}
				if (y > maxY) {
					maxY = y;
				}
				if (z < minZ) {
					minZ = z;
				}
				if (z > maxZ) {
					maxZ = z;
				}

				vertexPositions.push_back(v);
			} else if (typeIdentifier == "vt") {
				// a vertex texture coordinate
				float u, v;
				lineIn >> u >> v;

				Vector2 t;
				t.u = u;
				t.v = v;

				vertexTextureCoords.push_back(t);
			} else if (typeIdentifier == "vn") {
				// a vertex normal
				float x, y, z;
				lineIn >> x >> y >> z;

				Vector3 n;
				n.x = x;
				n.y = y;
				n.z = z;

				vertexNormals.push_back(n);
			} else if (typeIdentifier == "f") {
				// a face
				unsigned int p1, t1, n1, p2, t2, n2, p3, t3, n3;
				sscanf(line.c_str(),
					     "f %d/%d/%d %d/%d/%d %d/%d/%d",
				       &p1, &t1, &n1,
				       &p2, &t2, &n2,
				       &p3, &t3, &n3);

				positionIndices.push_back(p1 - 1);
 				positionIndices.push_back(p2 - 1);
 				positionIndices.push_back(p3 - 1);

				textureCoordIndices.push_back(t1 - 1);
				textureCoordIndices.push_back(t2 - 1);
				textureCoordIndices.push_back(t3 - 1);

				normalIndices.push_back(n1 - 1);
				normalIndices.push_back(n2 - 1);
				normalIndices.push_back(n3 - 1);

				this->numTriangles++;
			}
		}
	}
	this->numIndexedVertices = this->numTriangles * 3;

	// for auto-centering
	this->centre.x = totalX / float(vertexPositions.size());
	this->centre.y = totalY / float(vertexPositions.size());
	this->centre.z = totalZ / float(vertexPositions.size());

	// for auto-normalization
	this->dimensions.x = maxX - minX;
	this->dimensions.y = maxY - minY;
	this->dimensions.z = maxZ - minZ;

	// re-centre this object, if requested
	if (autoCentre) {
		totalX = 0.0f;
		totalY = 0.0f;
		totalZ = 0.0f;
		for (unsigned int i = 0; i < vertexPositions.size(); i++) {
			vertexPositions[i].x -= this->centre.x;
			vertexPositions[i].y -= this->centre.y;
			vertexPositions[i].z -= this->centre.z;
		}

		// normalize these coordinates to keep the mesh between -1 and 1 in all axes, if requested
		if (autoNormalize) {
			float maxDimension = this->dimensions.x;
			if (this->dimensions.y > maxDimension) {
				maxDimension = this->dimensions.y;
			}
			if (this->dimensions.z > maxDimension) {
				maxDimension = this->dimensions.z;
			}
			for (unsigned int i = 0; i < vertexPositions.size(); i++) {
				vertexPositions[i].x /= maxDimension;
				vertexPositions[i].y /= maxDimension;
				vertexPositions[i].z /= maxDimension;
			}
		}
	}

	// collect the vertex positions, texture coordinates, and normals for each face
	std::vector<Vector3> indexedPositions;
	std::vector<Vector2> indexedTextureCoords;
	std::vector<Vector3> indexedNormals;
	std::vector<unsigned int> vertexIndices;

	for (unsigned int i = 0; i < this->numIndexedVertices; i++) {
		unsigned int positionIndex = positionIndices[i];
		unsigned int textureCoordIndex = textureCoordIndices[i];
		unsigned int normalIndex = normalIndices[i];

		indexedPositions.push_back(vertexPositions[positionIndex]);
		indexedTextureCoords.push_back(vertexTextureCoords[textureCoordIndex]);
		indexedNormals.push_back(vertexNormals[normalIndex]);

		vertexIndices.push_back(i);
	}

	this->indexedPositions = indexedPositions;
	this->indexedTextureCoords = indexedTextureCoords;
	this->indexedNormals = indexedNormals;
	this->triangleIndices = vertexIndices;
}

Vector3 ObjMesh::getCentre() {
	return this->centre;
}

Vector3 ObjMesh::getDimensions() {
	return this->dimensions;
}

unsigned int ObjMesh::getNumVertices() {
	return this->numVertices;
}

Vector3* ObjMesh::getIndexedPositions() {
	return this->indexedPositions.data();
}

Vector2* ObjMesh::getIndexedTextureCoords() {
	return this->indexedTextureCoords.data();
}

Vector3* ObjMesh::getIndexedNormals() {
	return this->indexedNormals.data();
}

unsigned int ObjMesh::getNumTriangles() {
	return this->numTriangles;
}

unsigned int ObjMesh::getNumIndexedVertices() {
	return this->numIndexedVertices;
}

unsigned int* ObjMesh::getTriangleIndices() {
	return this->triangleIndices.data();
}
