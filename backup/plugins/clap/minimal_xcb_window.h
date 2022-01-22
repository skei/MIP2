#ifndef minimal_xcb_window_included
#define minimal_xcb_window_included
//----------------------------------------------------------------------

#include <xcb/xcb.h>
//#include <xcb/xcb_util.h>
//#include <xcb/xproto.h>

#include <string.h> // strlen

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MinimalXcbWindow {

//------------------------------
private:
//------------------------------

  xcb_connection_t* MConnection = nullptr;
  xcb_window_t      MWindow     = XCB_NONE;
  xcb_screen_t*     MScreen     = nullptr;
  xcb_gcontext_t    MScreenGC   = XCB_NONE;

  bool              MAttached   = false;

//------------------------------
protected:
//------------------------------

  uint32_t  MWidth   = 600;
  uint32_t  MHeight  = 400;

//------------------------------
public:
//------------------------------

  MinimalXcbWindow() {
  }

  //----------

  ~MinimalXcbWindow() {
    //if (MWindow != XCB_NONE) {
    if (MAttached) {
      xcb_free_gc(MConnection,MScreenGC);
      xcb_destroy_window(MConnection,MWindow);
      xcb_disconnect(MConnection);
    }
  }

//------------------------------
public:
//------------------------------

  bool attach(const char *display_name, unsigned long window) {

    //connect

    int screen_num;
    MConnection = xcb_connect(nullptr,&screen_num);
    xcb_screen_iterator_t iter;
    iter = xcb_setup_roots_iterator(xcb_get_setup(MConnection));
    for (; iter.rem; --screen_num, xcb_screen_next(&iter)) {
      if (screen_num == 0) {
        MScreen = iter.data;
        break;
      }
    }

    // create window

    uint32_t event_mask =
      XCB_EVENT_MASK_KEY_PRESS      |
      XCB_EVENT_MASK_KEY_RELEASE    |
      XCB_EVENT_MASK_BUTTON_PRESS   |
      XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_ENTER_WINDOW   |
      XCB_EVENT_MASK_LEAVE_WINDOW   |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_EXPOSURE       |
      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    uint32_t window_mask =
      XCB_CW_BACK_PIXMAP  |
      XCB_CW_BORDER_PIXEL |
      XCB_CW_EVENT_MASK   |
      XCB_CW_COLORMAP;
    uint32_t window_mask_values[4] = {
      XCB_NONE,
      0,
      event_mask,
      MScreen->default_colormap
    };

    MWindow = xcb_generate_id(MConnection);
    xcb_create_window(
      MConnection,                    // connection
      XCB_COPY_FROM_PARENT,           // depth (same as root)
      MWindow,                        // window Id
      window,                         // parent window
      0, 0,                           // x, y
      MWidth, MHeight,                // width, height
      0,                              // border_width
      XCB_WINDOW_CLASS_INPUT_OUTPUT,  // class
      MScreen->root_visual,           // visual
      window_mask,
      window_mask_values
    );

    xcb_flush(MConnection);

    // remove decorations

//    struct WMHints {
//      uint32_t flags;
//      uint32_t functions;
//      uint32_t decorations;
//      int32_t  inputMode;
//      uint32_t state;
//    };
//
//    static const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;
//
//    xcb_atom_t prop = mip_xcb_get_intern_atom(MConnection,"_MOTIF_WM_HINTS");
//    if (prop) {
//      WMHints hints;
//      hints.flags = MWM_HINTS_DECORATIONS;
//      hints.decorations = 0;
//      const unsigned char* ptr = (const unsigned char*)(&hints);
//      xcb_change_property(
//        MConnection,
//        XCB_PROP_MODE_REPLACE,
//        MWindow,
//        prop,     // hintsAtomReply->atom,
//        prop,     // XCB_ATOM_WM_HINTS,
//        32,
//        5,        // PROP_MOTIF_WM_HINTS_ELEMENTS
//        ptr
//      );
//    }

    // init GC

    MScreenGC = xcb_generate_id(MConnection);
    xcb_drawable_t draw = MScreen->root;
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t values[2];
    values[0] = MScreen->black_pixel;
    values[1] = MScreen->white_pixel;
    xcb_create_gc(MConnection, MScreenGC, draw, mask, values);

    MAttached = true;
    return true;
  }

  //----------

  void show() {
    xcb_map_window(MConnection,MWindow);
    //fill(0x808080ff);
    xcb_flush(MConnection);
  }

  //----------

  void hide() {
    xcb_unmap_window(MConnection,MWindow);
    xcb_flush(MConnection);
  }

//------------------------------
private:
//------------------------------

//  xcb_atom_t mip_xcb_get_intern_atom(xcb_connection_t *conn, const char *name) {
//    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn ,0, strlen(name), name);
//    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
//    return reply->atom;
//  }

//------------------------------
public:
//------------------------------

  void fill(uint32_t AColor) {
    fill(0,0,MWidth,MHeight,AColor);
  }

  //----------

  void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) {
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1];
    values[0] = AColor;
    xcb_change_gc(MConnection,MScreenGC,mask,values);
    xcb_rectangle_t rectangles[] = {{
      (int16_t)AXpos,
      (int16_t)AYpos,
      (uint16_t)AWidth,
      (uint16_t)AHeight,
    }};
    xcb_poly_fill_rectangle(MConnection,MWindow,MScreenGC,1,rectangles);
  }

};

//----------------------------------------------------------------------
#endif
