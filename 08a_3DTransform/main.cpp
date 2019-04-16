#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "ShaderProgram.h"
#include "ObjMesh.h"

#define ANGLE_INCREMENT 2.5f
#define SCALE_FACTOR 1.1f
#define FOV_INCREMENT 2.0f

int width, height;

GLuint programId;
GLuint vertexBuffer;
GLuint indexBuffer;
GLenum positionBufferId;
GLuint positions_vbo = 0;
GLuint textureCoords_vbo = 0;
GLuint normals_vbo = 0;
GLuint colours_vbo = 0;

unsigned int numVertices;

glm::mat4 orthographicProjection;
glm::mat4 perspectiveProjection;
glm::mat4 projection;
glm::mat4 view;

float fov = 45.0f;
bool perspective = true;

glm::vec3 eyePosition(2, 1, 1);

float scale = 1.0f;

float xAngle = 0.0f;
float yAngle = 0.0f;
float zAngle = 0.0f;

static void createGeometry(void) {
  ObjMesh mesh;
  mesh.load("meshes/teapot.obj", true, true);

  numVertices = mesh.getNumIndexedVertices();
  glm::vec3* vertexPositions = mesh.getIndexedPositions();
  glm::vec2* vertexTextureCoords = mesh.getIndexedTextureCoords();
  glm::vec3* vertexNormals = mesh.getIndexedNormals();

  glGenBuffers(1, &positions_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), vertexPositions, GL_STATIC_DRAW);

  glGenBuffers(1, &textureCoords_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), vertexTextureCoords, GL_STATIC_DRAW);

  glGenBuffers(1, &normals_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), vertexNormals, GL_STATIC_DRAW);

  unsigned int* indexData = mesh.getTriangleIndices();
  int numTriangles = mesh.getNumTriangles();

  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numTriangles * 3, indexData, GL_STATIC_DRAW);
}

static void update(void) {
    int milliseconds = glutGet(GLUT_ELAPSED_TIME);

    glutPostRedisplay();
}


static void render(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // activate our shader program
  glUseProgram(programId);

  // turn on depth buffering
  glEnable(GL_DEPTH_TEST);

  // model matrix: translate, scale, and rotate the model
  glm::vec3 rotationAxis(0,1,0);
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(xAngle), glm::vec3(1, 0, 0)); // rotate about the x-axis
  model = glm::rotate(model, glm::radians(yAngle), glm::vec3(0, 1, 0)); // rotate about the y-axis
  model = glm::rotate(model, glm::radians(zAngle), glm::vec3(0, 0, 1)); // rotate about the z-axis
  model = glm::scale(model, glm::vec3(scale, scale, scale));

  // model-view matrix
  glm::mat4 mv = view * model;
  GLuint mvMatrixId = glGetUniformLocation(programId, "uMV");
  glUniformMatrix4fv(mvMatrixId, 1, GL_FALSE, &mv[0][0]);

  // model-view-projection matrix
  glm::mat4 mvp = projection * view * model;
  GLuint mvpMatrixId = glGetUniformLocation(programId, "uMVP");
  glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);

  // find the names (ids) of each vertex attribute
  GLint positionAttribId = glGetAttribLocation(programId, "position");
  GLint textureCoordsAttribId = glGetAttribLocation(programId, "textureCoords");
  GLint normalAttribId = glGetAttribLocation(programId, "normal");

  // provide the vertex positions to the shaders
  glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
  glEnableVertexAttribArray(positionAttribId);
  glVertexAttribPointer(positionAttribId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // provide the vertex texture coordinates to the shaders
  glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vbo);
  glEnableVertexAttribArray(textureCoordsAttribId);
  glVertexAttribPointer(textureCoordsAttribId, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  // provide the vertex normals to the shaders
  glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
  glEnableVertexAttribArray(normalAttribId);
  glVertexAttribPointer(normalAttribId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // draw the triangles
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, (void*)0);

	// disable the attribute arrays
  glDisableVertexAttribArray(positionAttribId);
  glDisableVertexAttribArray(textureCoordsAttribId);
  glDisableVertexAttribArray(normalAttribId);

  // make the draw buffer to display buffer (i.e. display what we have drawn)
  glutSwapBuffers();
}

static void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    width = w;
    height = h;

    // projection matrix - perspective projection
    // FOV:           45°
    // Aspect ratio:  4:3 ratio
    // Z range:       between 0.1 and 1000.0
    float aspectRatio = (float)width / (float)height;
    perspectiveProjection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);

    // projection matrix - orthographic (non-perspective) projection
    // Note:  These are in world coordinates
    float size = 1.3f;
    orthographicProjection = glm::ortho(-size, size, -size / aspectRatio, size / aspectRatio, 0.0f, 100.0f);

    if (perspective) {
      projection = perspectiveProjection;
   } else {
      projection = orthographicProjection;
   }

    // view matrix - orient everything around our preferred view
    view = glm::lookAt(
        eyePosition,
        glm::vec3(0,0,0),    // where to look
        glm::vec3(0,1,0)     // up
    );
}

static void drag(int x, int y) {
}

static void mouse(int button, int state, int x, int y) {
}

static void keyboard(unsigned char key, int x, int y) {
   if (key == 'w') {
      xAngle += ANGLE_INCREMENT;
   } else if (key == 's') {
      xAngle -= ANGLE_INCREMENT;
   } else if (key == 'a') {
      yAngle += ANGLE_INCREMENT;
   } else if (key == 'd') {
      yAngle -= ANGLE_INCREMENT;
   } else if (key == 'q') {
      zAngle += ANGLE_INCREMENT;
   } else if (key == 'e') {
      zAngle -= ANGLE_INCREMENT;
   } else if (key == '-') {
      scale /= SCALE_FACTOR;
   } else if (key == '+') {
      scale *= SCALE_FACTOR;
   } else if (key == 'p') {
      perspective = !perspective;
      reshape(width, height);
   } else if (key == 'm') {
      fov += FOV_INCREMENT;
      reshape(width, height);
   } else if (key == 'n') {
      fov -= FOV_INCREMENT;
      reshape(width, height);
   }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("CSCI 3090u Shading in OpenGL");
    glutIdleFunc(&update);
    glutDisplayFunc(&render);
    glutReshapeFunc(&reshape);
    glutMotionFunc(&drag);
    glutMouseFunc(&mouse);
    glutKeyboardFunc(&keyboard);

    glewInit();
    if (!GLEW_VERSION_2_0) {
        std::cerr << "OpenGL 2.0 not available" << std::endl;
        return 1;
    }
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

    createGeometry();
    ShaderProgram program;
    program.loadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
  	programId = program.getProgramId();

    glutMainLoop();

    return 0;
}
