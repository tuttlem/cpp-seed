
#include <GL/glut.h>

#include "camera.hpp"

const int windowWidth = 800;
const int windowHeight = 600;

float advanceSpeed = 0.0f;
float strafeSpeed = 0.0f;
int lastMouseX = 0, lastMouseY = 0;

camera cam;

void resize(int width, int height);
void render(void);
void update(void);
void normalKey(unsigned char key, int x, int y);
void normalKeyUp(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void specialKeyUp(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);

void drawPlane();

int main(int argc, char *argv[]) {

   // initialize glut & video
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowPosition(0, 0);
   glutInitWindowSize(windowWidth, windowHeight);

   // create the window
   glutCreateWindow("Template with Camera");

   // setup the environment
   glutIgnoreKeyRepeat(1);

   // setup the callbacks
   glutDisplayFunc(render);
   glutIdleFunc(render);
   glutReshapeFunc(resize);
   glutKeyboardFunc(normalKey);
   glutKeyboardUpFunc(normalKeyUp);
   glutSpecialFunc(specialKey);
   glutSpecialUpFunc(specialKeyUp);
   glutMouseFunc(mouse);
   glutPassiveMotionFunc(mouseMotion);

   // setup open gl
   glShadeModel(GL_SMOOTH);
   glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   // run the program
   glutMainLoop();

   return 0;
}

void resize(int width, int height) {

   // avoid div-by-zero
   if (height == 0) {
      height = 1;
   }

   // calculate the aspect ratio
   float ratio = width * 1.0 / height;

   // set the projection matrix
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0, 0, width, height);
   gluPerspective(45.0f, ratio, 0.1f, 1000.0f);

   // back to modelview
   glMatrixMode(GL_MODELVIEW);

}

void render(void) {
   update();

   // clear the buffers
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // reset the current matrix
   glLoadIdentity();

   // position everything with respect to the camera
   cam.place();

   drawPlane();

   // flip the buffers
   glutSwapBuffers();
}

void update(void) {

   cam.advance(advanceSpeed);
   cam.strafe(strafeSpeed);

}

void normalKey(unsigned char key, int x, int y) {
   switch (key) {
      case 119: /* w */
         advanceSpeed = -1.0f;
         break;

      case 97: /* a */
         strafeSpeed = -1.0f;
         break;

      case 115: /* s */
         advanceSpeed = 1.0f;
         break;

      case 100: /* d */
         strafeSpeed = 1.0f;
         break;

   }
}

void normalKeyUp(unsigned char key, int x, int y) {
   switch (key) {
      case 119: /* w */
      case 115: /* s */
         advanceSpeed = 0.0f;
         break;

      case 97: /* a */
      case 100: /* d */
         strafeSpeed = 0.0f;
         break;
   }
}

void specialKey(int key, int x, int y) {

}

void specialKeyUp(int key, int x, int y) {
}

void mouse(int button, int state, int x, int y) {

}

void mouseMotion(int x, int y) {
   int halfWidth = windowWidth >> 1;
   int halfHeight = windowHeight >> 1;

   float deltaX = (halfWidth - x) / 20.0f;
   float deltaY = (halfHeight - y) / 20.0f;

   if ((deltaX == 0.0f) && (deltaY == 0.0f)) {
      return ;
   }

   cam.yaw(deltaX);
   cam.pitch(deltaY);

   glutWarpPointer(halfWidth, halfHeight);
}

void drawPlane() {

   int planeSize = 1000;
   int halfSize = planeSize >> 1;

   glBegin(GL_POINTS);
   glColor3f(0.0f, 1.0f, 0.0f);

   for (int z = 0; z < planeSize; z ++) {
      float pz = (z - halfSize);
      float zc = ((float)z / (float)planeSize);
      for (int x = 0; x < planeSize; x ++) {
         float px = (x - halfSize);
         float xc = ((float)x / (float)planeSize);
         glColor3f(zc, xc, (zc + xc) / 2.0f);
         glVertex3f(px, -5.0f, pz);
      }
   }

   glEnd();
}

