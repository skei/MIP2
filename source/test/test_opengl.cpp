
#include "mip.h"
#include "gui/opengl/mip_opengl_window.h"

//----------------------------------------------------------------------

int main() {
  MIP_OpenGLWindow* window = new MIP_OpenGLWindow(640,480);
  delete window;
  return 0;
}

//----------



#if 0

#include <GL/glx.h>
#include <X11/Xlib.h>

#include "mip.h"

#define _dbg(...) { fprintf( stderr,"=>[%s][%d][%s]",__FILE__,__LINE__,__FUNCTION__); fprintf( stderr,__VA_ARGS__); fprintf( stderr,"\n");}
#define _err(...) { fprintf( stderr,"[ERROR][%s][%d][%s]",__FILE__,__LINE__,__FUNCTION__); fprintf( stderr,__VA_ARGS__); fprintf( stderr,"\n");}

void init(int w, int h) {
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,w,h,0,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glShadeModel(GL_SMOOTH);
  glClearColor(0,0.4,0.2,1.0);
  glClearDepth(1.f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void draw() {
  float rot = 0.f;
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  glPushMatrix();
  glColor4f(1.f,0.7f,0.75f,0.5f);
  float cx = 640 / 2;
  float cy = 480 / 2;
  float s = 10;
  glTranslatef(cx,cy,0);
  glRotatef(rot,0,0,1);
  glTranslatef(-cx,-cy,0);
  rot+=1.1;
  glBegin(GL_QUADS);
  glVertex3f(cx-s,cy-s,0);
  glVertex3f(cx+s,cy-s,0);
  glVertex3f(cx+s,cy+s,0);
  glVertex3f(cx-s,cy+s,0);
  glEnd();
  glPopMatrix();
  int err = 0;
  if ((err = glGetError()) != 0) _err("glGetError(): %d", err);
  //nvgBeginPath(vg);
  //nvgRect(vg, 100,100, 120,30);
  //nvgCircle(vg, 120,120, 5);
  //nvgPathWinding(vg, NVG_HOLE);    // Mark circle as a hole.
  //nvgFillColor(vg, nvgRGBA(255,192,0,255));
  //nvgFill(vg);
}

//void eventHandler(const nanoGFX::nGSurface& surface, const nanoGFX::nGEvent& ev, void* user_data) {
//  if (ev.type == nanoGFX::nGEvent::WINDOW_CREATE) {
//    init(app.getWidth(), app.getHeight());
//  }
//  if (ev.type == nanoGFX::nGEvent::WINDOW_REPAINT) {
//    draw();
//  }
//  if (ev.type == nanoGFX::nGEvent::WINDOW_DESTROY || ev.type == nanoGFX::nGEvent::KEY_DOWN) {
//    done = true;
//  }
//}

#endif // 0
