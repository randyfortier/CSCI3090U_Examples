#include <iostream>
#include <string>
#include <vector>

#include <gl/glut.h>
#include <gl/GL.h>

#include <chrono>

#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "Vector3.h"
#include "Colour.h"
#include "ColourDefinitions.h"

int resolutionX = 500;
int resolutionY = 500;

Colour* raster;

std::vector<Sphere> scene;

Material yellowMaterial(COLOUR_YELLOW, 0.5f, 0.5f, 80.0f);
Vector3 sphere1Centre(0.5, 0.2, 0.25);
Sphere sphere1(sphere1Centre, 0.2, yellowMaterial);

Material redMaterial(COLOUR_RED, 0.2f, 0.8f, 200.0f);
Vector3 sphere2Centre(0.2, 0.15, 0.6);
Sphere sphere2(sphere2Centre, 0.15, redMaterial);

Material blueMaterial(COLOUR_BLUE, 0.8f, 0.2f, 5.0f);
Vector3 sphere3Centre(0.8, 0.1, 0.75);
Sphere sphere3(sphere3Centre, 0.1, blueMaterial);

Material lightGrayMaterial(COLOUR_LIGHT_GRAY, 0.5f, 0.5f, 100.0f);
Vector3 planeOrigin(0.0, 0.0, 0.0);
Vector3 planeNormal(0.0, 1.0, 0.0);
Plane ground(planeOrigin, planeNormal, lightGrayMaterial);

Vector3 eyeLocation(0.5, 0.5, -10.0);

Vector3 lightPosition(0.5, 10.0, -2.0);
Colour lightColour(1.0, 1.0, 1.0, 1.0);

std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
float originalLightZ = lightPosition.z;
float originalLightX = lightPosition.x;

void update(void) {
	std::chrono::high_resolution_clock::time_point frameTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedTime = frameTime - lastFrameTime;
	float deltaT = elapsedTime.count();

	lightPosition.z = originalLightZ + sinf(deltaT);
	lightPosition.x = originalLightX + sinf(deltaT);

	// clear the raster
	for (int y = 0; y < resolutionY; y++) {
		for (int x = 0; x < resolutionX; x++) {
			raster[y * resolutionX + x] = COLOUR_BLACK;
		}
	}

	float stepSizeX = 1.0f / (float)resolutionX;
	float stepSizeY = 1.0f / (float)resolutionY;

	// trace the rays
	for (int y = 0; y < resolutionY; y++) {
		for (int x = 0; x < resolutionX; x++) {
			// create a ray from the eyeLocation to this point
			Vector3 pixelLocation;
			pixelLocation.x = ((float)x + 0.5f) * stepSizeX;
			pixelLocation.y = ((float)y + 0.5f) * stepSizeY;
			pixelLocation.z = 0.0f;

			Vector3 rayDirection = pixelLocation - eyeLocation;
			rayDirection.normalize();

			Ray ray(eyeLocation, rayDirection);

			// find any intersections

			// ground plane
			float minT = ground.intersectionPoint(ray);

			// if we hit the plane, colour the pixel
			if (!isnan(minT)) {
				if (minT < 0.0f) {
					minT = NAN;
				} else {
					raster[y * resolutionX + x] = ground.calculateShading(lightPosition, lightColour, ray, minT);
				}
			}

			// spheres
			for (unsigned int i = 0; i < scene.size(); i++) {
				Sphere sphere = scene.at(i);
				float t = sphere.intersectionPoint(ray);
				if (!isnan(t) && t > 0.0f) {
					if (isnan(minT) || t < minT) {
						minT = t;
						raster[y * resolutionX + x] = sphere.calculateShading(lightPosition, lightColour, ray, t);
					}
				}
			}
		}
	}

	glutPostRedisplay();
}

void display(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	unsigned int numPixels = resolutionX * resolutionY;
	float* pixelData = new float[numPixels * 3];

	for (unsigned int i = 0; i < numPixels; i++) {
		pixelData[i * 3 + 0] = raster[i].r;
		pixelData[i * 3 + 1] = raster[i].g;
		pixelData[i * 3 + 2] = raster[i].b;
	}

	glDrawPixels(resolutionX, resolutionY, GL_RGB, GL_FLOAT, pixelData);

	glutSwapBuffers();
}

void saveAsPPM(std::string filename, int resolutionX, int resolutionY, Colour *raster);

int main(int argc, char *argv[]) {
	// Note:  This application uses GLUT, but does not use OpenGL, except to draw our generated pixels to a framebuffer

	// setup window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(resolutionX, resolutionY);
	glutCreateWindow("CSCI 3090U - Ray Tracer");

	// tell GLUT about our event-handler functions
	glutIdleFunc(update);
	glutDisplayFunc(display);

	scene.push_back(sphere1);
	scene.push_back(sphere2);
	scene.push_back(sphere3);

	raster = new Colour[resolutionX * resolutionY];

	glutMainLoop();

	delete raster;

	return 0;
}
