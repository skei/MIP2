
#include "mip.h"

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h> /* for XGetXCBConnection, link with libX11-xcb */
#include <xcb/xcb.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>

#define NANOVG_GL2_IMPLEMENTATION	// Use GL2 implementation.
#include "extern/nanovg/nanovg.h"
#include "extern/nanovg/nanovg_gl.h"
  // get rid of "warning: ‘visibility’ attribute ignored [-Wattributes]"
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include "extern/nanovg/nanovg.c"
  #pragma GCC diagnostic pop

//----------------------------------------------------------------------

  Display*          display;
  xcb_connection_t* connection;
  xcb_screen_t*     screen;
  xcb_colormap_t    colormap;
  xcb_window_t      window;
  GLXFBConfig       fb_config;
  GLXContext        context;
  GLXDrawable       drawable;
  GLXWindow         glxwindow;
  int               default_screen;
  int               visualID;

//----------------------------------------------------------------------

bool init() {
  /* Open Xlib Display */
  display = XOpenDisplay(0);
  if (!display) {
    fprintf(stderr, "Can't open display\n");
    return false;
  }
  default_screen = DefaultScreen(display);
  /* Get the XCB connection from the display */
  connection = XGetXCBConnection(display);
  if (!connection) {
    XCloseDisplay(display);
    fprintf(stderr, "Can't get xcb connection from display\n");
    return false;
  }
  /* Acquire event queue ownership */
  XSetEventQueueOwner(display,XCBOwnsEventQueue);
  /* Find XCB screen */
  screen = 0;
  xcb_screen_iterator_t screen_iter =
  xcb_setup_roots_iterator(xcb_get_setup(connection));
  for (int screen_num = default_screen;
    screen_iter.rem && screen_num > 0;
    --screen_num, xcb_screen_next(&screen_iter));
  screen = screen_iter.data;
  /* Initialize window and OpenGL context, run main loop and deinitialize */
//  int retval = setup_and_run(display, connection, default_screen, screen);
//  /* Cleanup */
//  XCloseDisplay(display);
//  return retval;
  return true;
}

//------------------------------

bool setup() {
  visualID = 0;
  /* Query framebuffer configurations */
  GLXFBConfig *fb_configs = 0;
  int num_fb_configs = 0;
  fb_configs = glXGetFBConfigs(display, default_screen, &num_fb_configs);
  if (!fb_configs || num_fb_configs == 0) {
    fprintf(stderr, "glXGetFBConfigs failed\n");
    return false;
  }
  // Select first framebuffer config and query visualID
  fb_config = fb_configs[0];
  glXGetFBConfigAttrib(display, fb_config, GLX_VISUAL_ID , &visualID);
  // Create OpenGL context
  context = glXCreateNewContext(display, fb_config, GLX_RGBA_TYPE, 0, True);
  if (!context) {
    fprintf(stderr, "glXCreateNewContext failed\n");
    return -1;
  }
  // Create XID's for colormap and window
  colormap = xcb_generate_id(connection);
  window = xcb_generate_id(connection);
  // Create colormap
  xcb_create_colormap(connection,XCB_COLORMAP_ALLOC_NONE,colormap,screen->root,visualID);
  //* Create window
  uint32_t eventmask = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
  uint32_t valuelist[] = { eventmask, colormap, 0 };
  uint32_t valuemask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
  xcb_create_window(connection,XCB_COPY_FROM_PARENT,window,screen->root,0,0,150,150,0,XCB_WINDOW_CLASS_INPUT_OUTPUT,visualID,valuemask,valuelist);
  // NOTE: window must be mapped before glXMakeContextCurrent
  xcb_map_window(connection, window);
  // Create GLX Window
  drawable = 0;
  glxwindow = glXCreateWindow(display,fb_config,window,0);
  if (!window) {
    xcb_destroy_window(connection, window);
    glXDestroyContext(display, context);
    fprintf(stderr, "glXDestroyContext failed\n");
    return false;
  }
  drawable = glxwindow;
  /* make OpenGL context current */
  if (!glXMakeContextCurrent(display, drawable, drawable, context)) {
    xcb_destroy_window(connection, window);
    glXDestroyContext(display, context);
    fprintf(stderr, "glXMakeContextCurrent failed\n");
    return false;
  }
  return true;
}

//------------------------------

int run() {
  //return main_loop(display, connection, window, drawable);
  //int main_loop(Display* display, xcb_connection_t* connection, xcb_window_t window, GLXDrawable drawable) {
  int running = 1;
  while (running) {
    /* Wait for event */
    xcb_generic_event_t *event = xcb_wait_for_event(connection);
    if (!event) {
      fprintf(stderr, "i/o error in xcb_wait_for_event");
      return -1;
    }
    switch (event->response_type & ~0x80) {
      case XCB_KEY_PRESS:
        /* Quit on key press */
        running = 0;
        break;
      case XCB_EXPOSE:
        /* Handle expose event, draw and swap buffers */
        //draw();
        glClearColor(0.2, 0.4, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glXSwapBuffers(display, drawable);
        break;
      default:
        break;
    }
    free(event);
  }
  return 0;
  //}

}

//------------------------------

void cleanup() {
  glXDestroyWindow(display, glxwindow);
  xcb_destroy_window(connection, window);
  glXDestroyContext(display, context);
}

//----------------------------------------------------------------------



//----------------------------------------------------------------------

int main() {
  init();
  setup();

  // crashes..
  //struct NVGcontext* vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

  run(),
  cleanup();
  return 0;
}

//----------------------------------------------------------------------

//----------------------------------------------------------------------


//----------------------------------------------------------------------
// setup_and_run
//----------------------------------------------------------------------


//void cleanup() {
//  glXDestroyWindow(display, glxwindow);
//  xcb_destroy_window(connection, window);
//  glXDestroyContext(display, context);
//}


//----------------------------------------------------------------------



