#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Shaders.h"
#include "teapot.h"

float eyex, eyey, eyez;

double theta, phi;
double r;

GLuint programId;

glm::mat4 projection;

static GLuint SurfaceKnotsVao;
static GLsizei VertCount;
static GLsizei KnotCount;
static float TessInner = 8.0f;
static float TessOuter = 8.0f;

float cx, cy, cz;
float radius;

void init() {
	float x, y, z;
	float minx, miny, minz;
	float maxx, maxy, maxz;
	int i;
	float dx, dy, dz;
	int positionLoc;

	const GLsizei Vec3Stride = 3 * sizeof(float);
	const GLsizei IndexStride = sizeof(unsigned short);
	VertCount = sizeof(TeapotPositions) / Vec3Stride;
	KnotCount = sizeof(TeapotKnots) / IndexStride;

	// Create the vertex array object
	glGenVertexArrays(1, &SurfaceKnotsVao);
	glBindVertexArray(SurfaceKnotsVao);

	// Determine the approximate size of the object
	maxx = maxy = maxz = -1000.0;
	minx = miny = minz = 1000.0;
	for (i = 0; i< VertCount; i++) {
		x = TeapotPositions[i][0];
		y = TeapotPositions[i][1];
		z = TeapotPositions[i][2];
		if (x < minx)
			minx = x;
		if (x > maxx)
			maxx = x;
		if (y < miny)
			miny = y;
		if (y > maxy)
			maxy = y;
		if (z < minz)
			minz = z;
		if (z > maxz)
			maxz = z;
	}
	cx = (minx + maxx) / 2.0;
	cy = (miny + maxy) / 2.0;
	cz = (minz + maxz) / 2.0;
	dx = maxx - minx;
	dy = maxy - miny;
	dz = maxz - minz;
	radius = sqrt(dx*dx + dy*dy + dz*dz);
	std::cout << "center: " << cx << ", " << cy << ", " << cz << std::endl;
	std::cout << "radius: " << radius << std::endl;

	// Add the control points to the vertex array object
	GLuint positions;

	glUseProgram(programId);
	glGenBuffers(1, &positions);
	glBindBuffer(GL_ARRAY_BUFFER, positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TeapotPositions), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TeapotPositions), TeapotPositions);

	positionLoc = glGetAttribLocation(programId, "Position");
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLoc);
	std::cout << "Position: " << positionLoc << std::endl;

	// Add the indices for the patches to the element array buffer
	GLuint knots;

	glGenBuffers(1, &knots);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, knots);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TeapotKnots), TeapotKnots, GL_STATIC_DRAW);
}


void changeSize(int w, int h) {
	float aspectRatio = 1.0 * w / h;

	glViewport(0, 0, w, h);

	projection = glm::perspective(45.0f, aspectRatio, 1.0f, 3000.0f);
}

void displayFunc(void) {
	glm::mat4 view;
	glm::mat4 modelViewPerspective;
	int matrixLoc;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programId);

	view = glm::lookAt(glm::vec3(eyex, eyey, eyez),
		glm::vec3(cx, cy, cz),
		glm::vec3(0.0f, 0.0f, 1.0f));

	modelViewPerspective = projection * view;

	matrixLoc = glGetUniformLocation(programId, "u_MVP");
	glUniformMatrix4fv(matrixLoc, 1, 0, glm::value_ptr(modelViewPerspective));

	glPatchParameteri(GL_PATCH_VERTICES, 16);

	float inner[] = { TessInner, TessOuter };
	float outer[] = { TessOuter, TessOuter, TessOuter, TessOuter };

	glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, &inner[0]);
	glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, &outer[0]);

	// Draw the patch, pass in the control points
	glBindVertexArray(SurfaceKnotsVao);
	glDrawElements(GL_PATCHES, KnotCount, GL_UNSIGNED_SHORT, 0);

	glutSwapBuffers();
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		phi -= 0.1;
		break;
	case 'd':
		phi += 0.1;
		break;
	case 'w':
		theta += 0.1;
		break;
	case 's':
		theta -= 0.1;
		break;
	}

	eyex = r*sin(theta)*cos(phi);
	eyey = r*sin(theta)*sin(phi);
	eyez = r*cos(theta);

	glutPostRedisplay();
}

int main(int argc, char **argv) {
	int fs;
	int vs;
	int tc;
	int te;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Tesselation Shaders");
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cerr << "Error starting GLEW: " << glewGetErrorString(error) << std::endl;
		exit(0);
	}

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keyboardFunc);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	programId = createShaderProgram("shaders/teapot_vert.glsl", "shaders/teapot_frag.glsl", "shaders/teapot_tessc.glsl", "shaders/teapot_tesse.glsl");
	dumpProgram(programId, "Tessellation shaders example");
	init();

	eyex = 0.0;
	eyez = 0.0;
	eyey = 1.0*radius;

	theta = 1.5;
	phi = 1.5;
	r = 1.0*radius;

	glutMainLoop();
}
