
#include "mip.h"

// get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"

  #define NG_GL_SUPPORT
  //#define NG_GLX_SUPPORT
  //#define NG_2D_SUPPORT
  #define NG_IMPLEMENT
  #include "extern/nanogfx.h"

#pragma GCC diagnostic pop

//----------------------------------------------------------------------

#define WIDTH   640
#define HEIGHT  480

//----------------------------------------------------------------------

float rot = 0.0;
bool done = false;

void eventHandler(const nanoGFX::nGSurface& surface, const nanoGFX::nGEvent& ev, void* user_data) {
  switch (ev.type) {
    case nanoGFX::nGEvent::WINDOW_CREATE:
      //MIP_Print("WINDOW_CREATE\n");
      {
        glViewport(0,0,WIDTH,HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,0,WIDTH,HEIGHT,-1,1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glShadeModel(GL_SMOOTH);
        //glClearColor(0,0.4,0.2,1.0);
        glClearColor(0.2,0.1,0.1,1);
        glClearDepth(1.f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      }
      break;
    case nanoGFX::nGEvent::WINDOW_EXPOSE:
      //MIP_Print("WINDOW_CREATE\n");
      break;
    case nanoGFX::nGEvent::WINDOW_REPAINT:
      MIP_Print("WINDOW_REPAINT\n");
      {
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        glColor4f(1,1,1,1);
        float cx = WIDTH / 2;
        float cy = HEIGHT / 2;
        float s = 10;
        glTranslatef(cx,cy,0);
        glRotatef(rot,0,0,1);
        glTranslatef(-cx,-cy,0);
        rot += 1.1;
        glBegin(GL_QUADS);
        glVertex3f(cx-s,cy-s,0);
        glVertex3f(cx+s,cy-s,0);
        glVertex3f(cx+s,cy+s,0);
        glVertex3f(cx-s,cy+s,0);
        glEnd();
        glPopMatrix();
        int err = 0;
        if ((err = glGetError()) != 0) printf("glGetError(): %d", err);
      }
      break;
    case nanoGFX::nGEvent::WINDOW_DESTROY:
      //MIP_Print("WINDOW_DESTROY\n");
      done = true;
      break;
    default:
      //MIP_Print("(unknown)\n");
      break;
  }
}

//----------------------------------------------------------------------

int main() {
  nanoGFX::nGSurface nano;
  nano.setEventHandler(eventHandler,nullptr);
  nano.create(WIDTH,HEIGHT,nanoGFX::SURFACE_GL);
  while (!done) {
    nano.update();
    nanoGFX::nGTime::sleep(100);
  }
  //nano.destroy();
  return 0;
}



//----------------------------------------------------------------------
