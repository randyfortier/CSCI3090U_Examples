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

#define STB_IMAGE_IMPLEMENTATION
#include "apis/stb_image.h"

#include "ShaderProgram.h"
#include "ObjMesh.h"

int width, height;

GLuint programId;
GLuint vertexBuffer;
GLuint indexBuffer;
GLenum positionBufferId;
GLuint positions_vbo = 0;
GLuint textureCoords_vbo = 0;
GLuint normals_vbo = 0;
GLuint colours_vbo = 0;
GLuint textureId;

unsigned int numVertices;

bool rotating = true;

unsigned int loadTexture(char const * path);

static void createTexture(std::string filename) {
   int imageWidth, imageHeight;
   int numComponents;

   // load the image data into a bitmap
   unsigned char *bitmap = stbi_load(filename.c_str(),
                                     &imageWidth,
                                     &imageHeight,
                                     &numComponents, 4);

   // generate a texture name
   glGenTextures(1, &textureId);

   // make the texture active
   glBindTexture(GL_TEXTURE_2D, textureId);

   // make a texture mip map
   glGenerateTextureMipmap(textureId);
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

   // specify the functions to use when shrinking/enlarging the texture image
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

   // specify the tiling parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // send the data to OpenGL
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight,
                0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

   // bind the texture to unit 0
   glBindTexture(GL_TEXTURE_2D, textureId);
   glActiveTexture(GL_TEXTURE0);

   // free the bitmap data
   stbi_image_free(bitmap);
}

static void createGeometry(void) {
  ObjMesh mesh;
  mesh.load("meshes/apple.obj", true, true);

  numVertices = mesh.getNumIndexedVertices();
  Vector3* vertexPositions = mesh.getIndexedPositions();
  Vector2* vertexTextureCoords = mesh.getIndexedTextureCoords();
  Vector3* vertexNormals = mesh.getIndexedNormals();

  glGenBuffers(1, &positions_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertexPositions, GL_STATIC_DRAW);

  glGenBuffers(1, &textureCoords_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), vertexTextureCoords, GL_STATIC_DRAW);

  glGenBuffers(1, &normals_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertexNormals, GL_STATIC_DRAW);

  unsigned int* indexData = mesh.getTriangleIndices();
  int numTriangles = mesh.getNumTriangles();

  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numTriangles * 3, indexData, GL_STATIC_DRAW);
}

float angle = 0.0f;

static void update(void) {
   int milliseconds = glutGet(GLUT_ELAPSED_TIME);

   // we'll rotate our model by an ever-increasing angle so that we can see the texture
   if (rotating) {
      float degrees = (float)milliseconds / 10.0f;
      angle = degrees;
   }

   glutPostRedisplay();
}

static void render(void) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // activate our shader program
	glUseProgram(programId);

   // turn on depth buffering
   glEnable(GL_DEPTH_TEST);

   // projection matrix - perspective projection
   // FOV:           45°
   // Aspect ratio:  4:3 ratio
   // Z range:       between 0.1 and 100.0
   float aspectRatio = (float)width / (float)height;
   glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

   // projection matrix - orthographic (non-perspective) projection
   // Note:  These are in world coordinates
   // xMin:          -10
   // xMax:          +10
   // yMin:          -10
   // yMax:          +10
   // zMin:           0
   // zMax:          +100
   //glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);

   // view matrix - orient everything around our preferred view
   glm::mat4 view = glm::lookAt(
      glm::vec3(40,30,30), // eye/camera location
      glm::vec3(0,0,0),    // where to look
      glm::vec3(0,1,0)     // up
   );

   // model matrix: translate, scale, and rotate the model
   glm::vec3 rotationAxis(0,1,0);
   glm::mat4 model = glm::mat4(1.0f);
   model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0)); // rotate about the y-axis
   model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));

   // model-view-projection matrix
   glm::mat4 mvp = projection * view * model;
   GLuint mvpMatrixId = glGetUniformLocation(programId, "MVP");
   glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);

   // texture sampler - a reference to the texture we've previously created
   // send the texture id to the texture sampler
   GLuint textureUniformId = glGetUniformLocation(programId, "textureSampler");
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glUniform1i(textureUniformId, 0);

   // find the names (ids) of each vertex attribute
   GLint positionAttribId = glGetAttribLocation(programId, "position");
   GLint colourAttribId = glGetAttribLocation(programId, "colour");
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
   glDisableVertexAttribArray(colourAttribId);

	// make the draw buffer to display buffer (i.e. display what we have drawn)
	glutSwapBuffers();
}

static void reshape(int w, int h) {
  // efficiency note:  calculating the projection matrix here only when resizing
  //                   would save much computation in the render() method.
  glViewport(0, 0, w, h);

  width = w;
  height = h;
}

static void drag(int x, int y) {
}

static void mouse(int button, int state, int x, int y) {
}

static void keyboard(unsigned char key, int x, int y) {
   if (key == 'r') {
      rotating = !rotating;
   }
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(800, 600);
   glutCreateWindow("CSCI 3090u Texture Mapping in OpenGL");
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

   // load and prepare the texture
   createTexture("textures/planks.jpg");

   ShaderProgram program;
  	program.loadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
  	programId = program.getProgramId();

   glutMainLoop();

   return 0;
}
