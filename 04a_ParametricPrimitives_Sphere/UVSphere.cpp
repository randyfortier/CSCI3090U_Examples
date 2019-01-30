#include "UVSphere.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>

void UVSphere::save(const std::string filename) {
   // TODO:  Generate the sphere vertices
   float deltaTheta = (M_PI * 2.0) / this->numVerticalSegments;
   float deltaPhi   = (M_PI * 2.0) / this->numHorizontalSegments;

   std::vector<glm::vec3> positions;
   std::vector<glm::vec2> textureCoords;
   std::vector<glm::vec3> normals;
   std::vector<unsigned int> vertexIndices;

   glm::vec3 centre(0.0, 0.0, 0.0);
   for (unsigned int latitudeIndex = 0; latitudeIndex < this->numHorizontalSegments; latitudeIndex++) {
      float phi = -1 * M_PI + (float)latitudeIndex * deltaPhi;
      for (unsigned int longitudeIndex = 0; longitudeIndex < this->numVerticalSegments; longitudeIndex++) {
         float theta = (float)longitudeIndex * deltaTheta;

         // positions - parametric equations for a sphere
         float x = this->radius * cos(theta) * sin(phi);
         float y = this->radius * cos(phi);
         float z = this->radius * sin(theta) * sin(phi);

         glm::vec3 pos(x, y, z);
         positions.push_back(pos);

         // texture coordinates
         float u = theta / (2.0 * M_PI);
         float v = (phi + M_PI) / (2.0 * M_PI);
         glm::vec2 uv(u, v);
         textureCoords.push_back(uv);

         // surface normals
         glm::vec3 normal = glm::normalize(pos - centre);
         normals.push_back(normal);

         // generate the triangles
         if ((latitudeIndex > 0) && (longitudeIndex > 0)) {
            unsigned int a = (latitudeIndex * this->numHorizontalSegments) + longitudeIndex;
            unsigned int b = (latitudeIndex * this->numHorizontalSegments) + longitudeIndex - 1;
            unsigned int c = ((latitudeIndex - 1) * this->numHorizontalSegments) + longitudeIndex;
            unsigned int d = ((latitudeIndex - 1) * this->numHorizontalSegments) + longitudeIndex - 1;

            vertexIndices.push_back(a);
            vertexIndices.push_back(b);
            vertexIndices.push_back(c);

            vertexIndices.push_back(d);
            vertexIndices.push_back(c);
            vertexIndices.push_back(b);

         }
      }
   }

   // TODO:  Handle the top special case
   unsigned int c = positions.size();
   glm::vec3 topCentre(0.0, this->radius, 0.0);
   positions.push_back(topCentre);

   // calculate texture coordinates, using the spherical coordinates
   glm::vec2 topuv(0.5, 1.0);
   textureCoords.push_back(topuv);

   // calculate the surface normals
   glm::vec3 topNormal = glm::normalize(topCentre - centre);
   normals.push_back(topNormal);

   unsigned int row = this->numVerticalSegments - 1;
   for (unsigned int col = 1; col < this->numHorizontalSegments; col++) {
     // form a triangle between col, col-1, and the top centre
     unsigned int a = (row * this->numHorizontalSegments) + col;
     unsigned int b = (row * this->numHorizontalSegments) + (col-1);

     vertexIndices.push_back(a);
     vertexIndices.push_back(b);
     vertexIndices.push_back(c);
   }

   // TODO:  Handle the bottom special case
   c = positions.size();
   glm::vec3 bottomCentre(0.0, -1 * this->radius, 0.0);
   positions.push_back(bottomCentre);

   // calculate texture coordinates, using the spherical coordinates
   glm::vec2 bottomuv(0.5, 0.0);
   textureCoords.push_back(bottomuv);

   // calculate the surface normal
   glm::vec3 bottomNormal = glm::normalize(bottomCentre - centre);
   normals.push_back(bottomNormal);

   row = 0;
   for (unsigned int col = 1; col < this->numHorizontalSegments; col++) {
     // form a triangle between col, col-1, and the bottom centre
     unsigned int a = col;
     unsigned int b = col - 1;

     vertexIndices.push_back(c);
     vertexIndices.push_back(b);
     vertexIndices.push_back(a);
   }

   // TODO:  Handle the last column special case
   unsigned int lastCol = this->numHorizontalSegments - 1;
   for (row = 1; row < this->numVerticalSegments; row++) {
     unsigned int a = ((row-1) * this->numHorizontalSegments);
     unsigned int b = ((row-1) * this->numHorizontalSegments) + lastCol;
     unsigned int c = (row * this->numHorizontalSegments);
     unsigned int d = (row * this->numHorizontalSegments) + lastCol;

     vertexIndices.push_back(b);
     vertexIndices.push_back(d);
     vertexIndices.push_back(a);

     vertexIndices.push_back(d);
     vertexIndices.push_back(b);
     vertexIndices.push_back(c);
   }

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
