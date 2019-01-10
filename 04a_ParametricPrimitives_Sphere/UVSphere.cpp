#include "UVSphere.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

void UVSphere::save(const std::string filename) {
   // TODO:  Generate the sphere vertices

   // TODO:  Handle the top special case

   // TODO:  Handle the bottom special case

   // TODO:  Handle the last column special case

   std::ofstream fileOut(filename.c_str());

   if (!fileOut.is_open()) {
      return;
   }

   for (unsigned int i = 0; i < positions.size(); i++) {
      fileOut << "v " << positions[i].x << " "  << positions[i].y << " "  << positions[i].z << std::endl;
   }

   for (unsigned int i = 0; i < textureCoords.size(); i++) {
      fileOut << "vt " << textureCoords[i].s << " "  << textureCoords[i].t << std::endl;
   }

   for (unsigned int i = 0; i < normals.size(); i++) {
      fileOut << "vn " << normals[i].x << " "  << normals[i].y << " "  << normals[i].z << std::endl;
   }

   for (unsigned int i = 0; i < vertexIndices.size(); i += 3) {
      fileOut << "f " << vertexIndices[i] << "/" << vertexIndices[i] << "/" << vertexIndices[i] << " ";
      fileOut << vertexIndices[i+1] << "/" << vertexIndices[i+1] << "/" << vertexIndices[i+1] << " ";
      fileOut << vertexIndices[i+2] << "/" << vertexIndices[i+2] << "/" << vertexIndices[i+2] << std::endl;
   }

   fileOut.close();
}

glm::vec3* UVSphere::getPositions() { return this->positions.data(); }
glm::vec2* UVSphere::getTextureCoords() { return this->textureCoords.data(); }
glm::vec3* UVSphere::getNormals() { return this->normals.data(); }
unsigned int UVSphere::getNumVertices() { return this->numVertices; }
unsigned int UVSphere::getNumTriangles() { return this->numTriangles; }
unsigned int* UVSphere::getTriangleIndices() { return this->triangleIndices; }
float UVSphere::getRadius() { return this->radius; }
unsigned int UVSphere::getNumVerticalSegments() { return this->numVerticalSegments; }
unsigned int UVSphere::getNumHorizontalSegments() { return this->numHorizontalSegments; }
