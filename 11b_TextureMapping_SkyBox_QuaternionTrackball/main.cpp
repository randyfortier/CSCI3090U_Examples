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
#include <glm/gtc/quaternion.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "apis/stb_image.h"

#include "ShaderProgram.h"
#include "trackball.hpp"
#include "ObjMesh.h"

#define NUM_SHADERS 4
#define SHADER_SWITCH_DELAY 5000

#define NUM_SKYBOXES 3
#define SKYBOX_SWITCH_DELAY 20000

int width = 1024;
int height = 768;

GLuint programIds[NUM_SHADERS];
int programIndex = 1;
int lastShaderTime = 0;
bool animateShaders = false;

GLuint skyboxProgramId;

GLuint vertexBuffer;
GLuint indexBuffer;
GLenum positionBufferId;
GLuint positions_vbo = 0;
GLuint textureCoords_vbo = 0;
GLuint normals_vbo = 0;
GLuint colours_vbo = 0;
GLuint skybox_vbo = 0;

unsigned int skyboxTextures[NUM_SKYBOXES];
unsigned int skyboxIndex;
int lastSkyboxTime = 0;
bool animateSkyboxes = false;

unsigned int numVertices;

float angle = 0.0f;
float lightOffsetY = 0.0f;

float theta = -0.4f;
float phi = -1.0f;
float cameraRadius = 100.0f;

bool animateLight = true;
bool rotateObject = true;

static void createTexture(std::string filename);
static unsigned int createCubemap(std::vector<std::string> filenames);

static void createSkybox(void) {
  float skyboxPositions[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
  };
  unsigned int numVertices = 36;

  glGenBuffers(1, &skybox_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 3, skyboxPositions, GL_STATIC_DRAW);
}

static void createGeometry(void) {
   ObjMesh mesh;
   mesh.load("meshes/dragon.obj", true, true);

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

static void update(void) {
   int milliseconds = glutGet(GLUT_ELAPSED_TIME);

   if (animateShaders) {
      int duration = milliseconds - lastShaderTime;

      if (duration > SHADER_SWITCH_DELAY) {
         programIndex++;

         if (programIndex >= NUM_SHADERS) {
            programIndex = 0;
         }

         lastShaderTime = milliseconds;
      }
   }

   if (animateSkyboxes) {
      int duration = milliseconds - lastSkyboxTime;

      if (duration > SKYBOX_SWITCH_DELAY) {
         skyboxIndex++;

         if (skyboxIndex >= NUM_SKYBOXES) {
            skyboxIndex = 0;
         }

         lastSkyboxTime = milliseconds;
      }
   }

   // rotate the shape about the y-axis so that we can see the shading
   if (rotateObject) {
      float degrees = (float)milliseconds / 80.0f;
      angle = degrees;
   }

   // move the light position over time along the x-axis, so we can see how it affects the shading
   if (animateLight) {
      float t = milliseconds / 1000.0f;
      lightOffsetY = sinf(t) * 100;
   }

   glutPostRedisplay();
}

glm::mat4 model;

static void render(void) {
   int programId = programIds[programIndex];
   int skyboxTexture = skyboxTextures[skyboxIndex];

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

   // view matrix - orient everything around our preferred view
   glm::mat4 view = glm::lookAt(
      eyePosition,
      glm::vec3(0,0,0),    // where to look
      glm::vec3(0,1,0)     // up
   );

   model = glm::mat4(1.0f);

   glm::mat4 vMatrix = glm::mat4(1.0f);
   vMatrix = glm::translate(vMatrix, glm::vec3(-eyePosition.x, -eyePosition.y, -eyePosition.z));
   vMatrix = glm::scale(vMatrix, glm::vec3(1.0, 1.0, 1.0));

   // draw the cube map sky box

   // provide the vertex positions to the shaders
   GLint skyboxPositionAttribId = glGetAttribLocation(skyboxProgramId, "position");
   glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
   glEnableVertexAttribArray(skyboxPositionAttribId);
   glVertexAttribPointer(skyboxPositionAttribId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

   // texture sampler - a reference to the texture we've previously created
   GLuint skyboxTextureId  = glGetUniformLocation(skyboxProgramId, "u_TextureSampler");
   glActiveTexture(GL_TEXTURE0);  // texture unit 0
   glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
   glUniform1i(skyboxTextureId, 0);

	glUseProgram(skyboxProgramId);

   glDepthMask(GL_FALSE);
   glDisable(GL_DEPTH_TEST);
   glFrontFace(GL_CCW);
   glDisable(GL_CULL_FACE);

   // set model-view matrix

   GLuint skyboxMVMatrixId = glGetUniformLocation(skyboxProgramId, "u_MVMatrix");
   glUniformMatrix4fv(skyboxMVMatrixId, 1, GL_FALSE, &view[0][0]);

   // set projection matrix
   GLuint skyboxProjMatrixId = glGetUniformLocation(skyboxProgramId, "u_PMatrix");
   glUniformMatrix4fv(skyboxProjMatrixId, 1, GL_FALSE, &projection[0][0]);

   glBindVertexArray(skyboxPositionAttribId);
   glDrawArrays(GL_TRIANGLES, 0, 36);

	// disable the attribute array
   glDisableVertexAttribArray(skyboxPositionAttribId);

   // draw the subject

   glUseProgram(programId);

   glDepthMask(GL_TRUE);
   glEnable(GL_DEPTH_TEST);

   // model matrix: translate, scale, and rotate the model
   model = glm::mat4(1.0f);
   model = glm::mat4_cast(rotation);
   model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

   // model-view-projection matrix
   glm::mat4 mvp = projection * view * model;
   GLuint mvpMatrixId = glGetUniformLocation(programId, "u_MVPMatrix");
   glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);

   // model matrix
   GLuint mMatrixId = glGetUniformLocation(programId, "u_ModelMatrix");
   glUniformMatrix4fv(mMatrixId, 1, GL_FALSE, &model[0][0]);

   GLuint textureId  = glGetUniformLocation(programId, "u_TextureSampler");
   glActiveTexture(GL_TEXTURE0);  // texture unit 0
   glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
   glUniform1i(textureId, 0);

   // the position of our camera/eye
   GLuint eyePosId = glGetUniformLocation(programId, "u_EyePosition");
   glUniform3f(eyePosId, eyePosition.x, eyePosition.y, eyePosition.z);

   // the position of our light
   GLuint lightPosId = glGetUniformLocation(programId, "u_LightPos");
   glUniform3f(lightPosId, 1, 8 + lightOffsetY, -2);

   // the colour of our object
   GLuint diffuseColourId = glGetUniformLocation(programId, "u_DiffuseColour");
   glUniform4f(diffuseColourId, 0.3, 0.2, 0.8, 1.0);

   // the shininess of the object's surface
   GLuint shininessId = glGetUniformLocation(programId, "u_Shininess");
   glUniform1f(shininessId, 25);

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
}

static void keyboard(unsigned char key, int x, int y) {
   switch (key) {
   case 'l':
      animateLight = !animateLight;
      break;
   case 'r':
      rotateObject = !rotateObject;
      break;
   case 's':
      animateShaders = !animateShaders;
      break;
   case 'b':
      animateSkyboxes = !animateSkyboxes;
      break;
   }
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(width, height);
   glutCreateWindow("CSCI 4110U - Texture Mapping - SkyBox");
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

   // determine the initial camera position
   eyePosition.x = cameraRadius * sin(theta) * cos(phi);
   eyePosition.y = cameraRadius * sin(theta) * sin(phi);
   eyePosition.z = cameraRadius * cos(theta);

   std::vector<std::string> filenames1;
   filenames1.push_back("textures/lposx.png");
   filenames1.push_back("textures/lnegx.png");
   filenames1.push_back("textures/lposy.png");
   filenames1.push_back("textures/lnegy.png");
   filenames1.push_back("textures/lposz.png");
   filenames1.push_back("textures/lnegz.png");

   std::vector<std::string> filenames2;
   filenames2.push_back("textures/cposx.png");
   filenames2.push_back("textures/cnegx.png");
   filenames2.push_back("textures/cposy.png");
   filenames2.push_back("textures/cnegy.png");
   filenames2.push_back("textures/cposz.png");
   filenames2.push_back("textures/cnegz.png");

   std::vector<std::string> filenames3;
   filenames3.push_back("textures/right.jpg");
   filenames3.push_back("textures/left.jpg");
   filenames3.push_back("textures/top.jpg");
   filenames3.push_back("textures/bottom.jpg");
   filenames3.push_back("textures/front.jpg");
   filenames3.push_back("textures/back.jpg");

   skyboxTextures[0] = createCubemap(filenames1);
   skyboxTextures[1] = createCubemap(filenames2);
   skyboxTextures[2] = createCubemap(filenames3);
   skyboxIndex = 2;
   glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

   // create the scene geometry
   createSkybox();
   createGeometry();

   // test out quaternions
   glm::quat rot(1.0f, 0.0f, 0.0f, 0.0f);
   float sqrtHalf = sqrt(0.5f);
   glm::quat rotx(sqrtHalf, sqrtHalf, 0.0f, 0.0f);  // 90 degrees about x
   glm::quat rotz(sqrtHalf, 0.0f, 0.0f, sqrtHalf);  // 90 degrees about z
   rot *= rotx;
   rot *= rotz;

   // load the GLSL shader programs

   ShaderProgram program[NUM_SHADERS];
   program[0].loadShaders("shaders/reflection_vertex.glsl", "shaders/reflection_fragment.glsl");
   program[1].loadShaders("shaders/copper_vertex.glsl", "shaders/copper_fragment.glsl");
   program[2].loadShaders("shaders/refraction_vertex.glsl", "shaders/refraction_fragment.glsl");
   program[3].loadShaders("shaders/combined_vertex.glsl", "shaders/combined_fragment.glsl");
   programIds[0] = program[0].getProgramId();
   programIds[1] = program[1].getProgramId();
   programIds[2] = program[2].getProgramId();
   programIds[3] = program[3].getProgramId();

   ShaderProgram skyboxProgram;
   skyboxProgram.loadShaders("shaders/skybox_vertex.glsl", "shaders/skybox_fragment.glsl");
  	skyboxProgramId = skyboxProgram.getProgramId();

   // output some basic help
   std::cout << "Controls:" << std::endl;
   std::cout << "\tLeft click + drag - rotate camera" << std::endl;
   std::cout << "\tRight click + drag - zoom camera" << std::endl;
   std::cout << "\tr - Enable/disable object auto-rotation" << std::endl;
   std::cout << "\ts - Enable/disable shader auto-switching" << std::endl;
   std::cout << "\tb - Enable/disable environment auto-switching" << std::endl;

   glutMainLoop();

   return 0;
}

static unsigned int createCubemap(std::vector<std::string> filenames) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, numChannels;
    for (unsigned int i = 0; i < filenames.size(); i++) {
        unsigned char *data = stbi_load(filenames[i].c_str(), &width, &height, &numChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << filenames[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glActiveTexture(GL_TEXTURE0);

    return textureID;
}

static void createTexture(std::string filename) {
  int width, height, numChannels;
  unsigned char *bitmap = stbi_load(filename.c_str(), &width, &height, &numChannels, 4);

  GLuint textureId;
  glGenTextures(1, &textureId);

  // make this texture active
  glBindTexture(GL_TEXTURE_2D, textureId);

  // specify the functions to use when shrinking/enlarging the texture image
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // send the data to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);

  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE0);

  // we don't need the bitmap data any longer
  stbi_image_free(bitmap);
}
