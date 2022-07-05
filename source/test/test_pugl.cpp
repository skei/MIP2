

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "mip.h"

//#include "extern/pugl/gl.h"
#include "extern/pugl/pugl.h"

extern "C" {
  #include "extern/pugl/pugl_x11.c"
}

//#include "pugl/gl.h"
//#include "pugl/pugl.h"

//----------

static int   quit   = 0;
static float xAngle = 0.0f;
static float yAngle = 0.0f;
static float dist   = 10.0f;

static const float cubeVertices[] = {
  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,

   1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

   1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,

   1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

   1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,

  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,

   1.0f,  1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,

   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,

   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,

   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
   1.0f, -1.0f,  1.0f
};

/** Calculate a projection matrix for a given perspective. */

static
void perspective(float* m, float fov, float aspect, float zNear, float zFar) {
  const float h     = tan(fov);
  const float w     = h / aspect;
  const float depth = zNear - zFar;
  const float q     = (zFar + zNear) / depth;
  const float qn    = 2 * zFar * zNear / depth;
  m[0]  = w;  m[1]  = 0;  m[2]  = 0;   m[3]  = 0;
  m[4]  = 0;  m[5]  = h;  m[6]  = 0;   m[7]  = 0;
  m[8]  = 0;  m[9]  = 0;  m[10] = q;   m[11] = -1;
  m[12] = 0;  m[13] = 0;  m[14] = qn;  m[15] = 0;
}

static
void onReshape(PuglView* view, int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  float projection[16];
  perspective(projection, 1.8f, width / (float)height, 1.0, 100.0f);
  glLoadMatrixf(projection);
}

static
void onDisplay(PuglView* view) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, dist * -1);
  glRotatef(xAngle, 0.0f, 1.0f, 0.0f);
  glRotatef(yAngle, 1.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
  glColorPointer(3, GL_FLOAT, 0, cubeVertices);
  glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

static
void printModifiers(PuglView* view, unsigned mods) {
  MIP_Print( "Modifiers:%s%s%s%s\n",
    (mods & PUGL_MOD_SHIFT) ? " Shift"   : "",
    (mods & PUGL_MOD_CTRL)  ? " Ctrl"    : "",
    (mods & PUGL_MOD_ALT)   ? " Alt"     : "",
    (mods & PUGL_MOD_SUPER) ? " Super" : "");
}

static
void onEvent(PuglView* view, const PuglEvent* event) {
  switch (event->type) {
    case PUGL_NOTHING:
      break;
    case PUGL_CONFIGURE:
      MIP_Print( "Configure\n");
      onReshape(view, event->configure.width, event->configure.height);
      break;
    case PUGL_EXPOSE:
      MIP_Print( "Expose\n");
      onDisplay(view);
      break;
    case PUGL_CLOSE:
      quit = 1;
      break;
    case PUGL_KEY_PRESS:
      MIP_Print( "Key %u (char %u) press (%s)%s\n",
      event->key.keycode, event->key.character, event->key.utf8,
      event->key.filter ? " (filtered)" : "");
      if (event->key.character == 'q' || event->key.character == 'Q' || event->key.character == PUGL_CHAR_ESCAPE) {
        quit = 1;
      }
      break;
    case PUGL_KEY_RELEASE:
      MIP_Print( "Key %u (char %u) release (%s)%s\n",
      event->key.keycode, event->key.character, event->key.utf8,
      event->key.filter ? " (filtered)" : "");
      break;
    case PUGL_MOTION_NOTIFY:
      xAngle = -(int)event->motion.x % 360;
      yAngle = (int)event->motion.y % 360;
      puglPostRedisplay(view);
      break;
    case PUGL_BUTTON_PRESS:
    case PUGL_BUTTON_RELEASE:
      MIP_Print( "Mouse %d %s at %f,%f ",event->button.button,(event->type == PUGL_BUTTON_PRESS) ? "down" : "up",event->button.x,event->button.y);
      printModifiers(view, event->scroll.state);
      break;
    case PUGL_SCROLL:
      MIP_Print( "Scroll %f %f %f %f ",
      event->scroll.x, event->scroll.y, event->scroll.dx, event->scroll.dy);
      printModifiers(view, event->scroll.state);
      dist += event->scroll.dy;
      if (dist < 10.0f) {
        dist = 10.0f;
      }
      puglPostRedisplay(view);
      break;
    case PUGL_ENTER_NOTIFY:
      MIP_Print( "Entered\n");
      break;
    case PUGL_LEAVE_NOTIFY:
      MIP_Print( "Exited\n");
      break;
    case PUGL_FOCUS_IN:
      MIP_Print( "Focus in\n");
      break;
    case PUGL_FOCUS_OUT:
      MIP_Print( "Focus out\n");
      break;
  }
}

int main(int argc, char** argv) {
  bool ignoreKeyRepeat = false;
  bool resizable       = false;
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-h")) {
      MIP_Print("USAGE: %s [OPTIONS]...\n\n"
      "  -h  Display this help\n"
      "  -i  Ignore key repeat\n"
      "  -r  Resizable window\n", argv[0]);
      return 0;
    } else if (!strcmp(argv[i], "-i")) {
      ignoreKeyRepeat = true;
    } else if (!strcmp(argv[i], "-r")) {
      resizable = true;
    } else {
      MIP_Print( "Unknown option: %s\n", argv[i]);
    }
  }
  setlocale(LC_CTYPE, "");
  PuglView* view = puglInit(NULL, NULL);
  puglInitWindowClass(view, "PuglTest");
  puglInitWindowSize(view, 512, 512);
  puglInitWindowMinSize(view, 256, 256);
  puglInitResizable(view, resizable);
  puglIgnoreKeyRepeat(view, ignoreKeyRepeat);
  puglSetEventFunc(view, onEvent);
  puglCreateWindow(view, "Pugl Test");
  puglEnterContext(view);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  puglLeaveContext(view, false);
  puglShowWindow(view);

  while (!quit) {
    puglWaitForEvent(view);
    puglProcessEvents(view);
  }

  puglDestroy(view);
  return 0;
}
