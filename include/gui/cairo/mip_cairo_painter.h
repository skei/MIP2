#ifndef mip_cairo_painter_included
#define mip_cairo_painter_included
//----------------------------------------------------------------------

/*
  Most surface types allow accessing the surface without using Cairo functions.
  If you do this, keep in mind that it is mandatory that you call
  cairo_surface_flush() before reading from or writing to the surface and that
  you must use cairo_surface_mark_dirty() after modifying it.
*/

//----------

/*

  https://github.com/steinbergmedia/vstgui/issues/80
  https://lists.cairographics.org/archives/cairo/2017-December/028496.html

  When you have a cairo surface using your xcb_connection*, do the following:
    MDevice = cairo_device_reference( cairo_surface_get_device(MSurface) );

  Then, *before* you do xcb_disconnect(connection), do:
    cairo_device_finish(device);
    cairo_device_destroy(device);
*/

//----------

#include "mip.h"
#include "gui/base/mip_base_painter.h"
#include "gui/cairo/mip_cairo.h"
#include "gui/cairo/mip_cairo_utils.h"

//----------------------------------------------------------------------

const
cairo_font_slant_t mip_font_slant[3] = {
  CAIRO_FONT_SLANT_NORMAL,
  CAIRO_FONT_SLANT_ITALIC,
  CAIRO_FONT_SLANT_OBLIQUE
};

//----------

const
cairo_font_weight_t mip_font_weight[2] = {
  CAIRO_FONT_WEIGHT_NORMAL,
  CAIRO_FONT_WEIGHT_BOLD
};

//----------

const
cairo_line_cap_t mip_line_cap[3] = {
  CAIRO_LINE_CAP_BUTT,
  CAIRO_LINE_CAP_ROUND,
  CAIRO_LINE_CAP_SQUARE
};

const
cairo_line_join_t mip_line_join[3] = {
  CAIRO_LINE_JOIN_MITER,
  CAIRO_LINE_JOIN_ROUND,
  CAIRO_LINE_JOIN_BEVEL
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_CairoPainter
: public MIP_BasePainter {

//------------------------------
private:
//------------------------------

  bool              MCreatedSurface = false;
  cairo_surface_t*  MSurface        = nullptr;
  cairo_t*          MCairo          = nullptr;
  //cairo_device_t*   MCairoDevice    = nullptr;
  float             MFontSize       = 0.0;
  MIP_Drawable*     MTargetDrawable = nullptr;
  MIP_Bitmap*       MTargetBitmap   = nullptr;
//------------------------------
public:
//------------------------------

  /*
    cairo_create()
    returns a newly allocated cairo_t with a reference count of 1. The initial
    reference count should be released with cairo_destroy() when you are done
    using the cairo_t. This function never returns NULL. If memory cannot be
    allocated, a special cairo_t object will be returned on which cairo_status()
    returns CAIRO_STATUS_NO_MEMORY. If you attempt to target a surface which
    does not support writing (such as cairo_mime_surface_t) then a
    CAIRO_STATUS_WRITE_ERROR will be raised. You can use this object normally,
    but no drawing will be done.

    If drawable is a Window, then the function cairo_xcb_surface_set_size()
    must be called whenever the size of the window changes.

    This function references target , so you can immediately call
    cairo_surface_destroy() on it if you don't need to maintain a separate
    reference to it.
  */

  MIP_CairoPainter(MIP_Drawable* ADrawable) {
    MTargetDrawable = ADrawable;
    if (ADrawable->isCairo()) {
      MSurface = ADrawable->getCairoSurface();
      MIP_CHECK_CAIRO_SURFACE_ERROR(MSurface);
      //MCreatedSurface = true;
      MCreatedSurface = false;
      MCairo = cairo_create(MSurface);
      MIP_CHECK_CAIRO_ERROR(MCairo);
      //cairo_set_line_width(MCairo,1);
      setFontSize(12);
    }
    else {
      //MIP_Print("! ADrawable->isCairo()\n");
    }
  }

  //----------

  MIP_CairoPainter(MIP_Bitmap* ABitmap) {
    MTargetBitmap = ABitmap;
    MSurface = ABitmap->createCairoSurface();
    MIP_CHECK_CAIRO_SURFACE_ERROR(MSurface);
    MCreatedSurface = true;
    MCairo = cairo_create(MSurface);
    MIP_CHECK_CAIRO_ERROR(MCairo);
    //cairo_set_line_width(MCairo,1);
    setFontSize(12);
  }

  //----------

  /*
    Decreases the reference count on cr by one. If the result is zero, then cr
    and all associated resources are freed.
  */

  virtual ~MIP_CairoPainter() {
    #ifdef MIP_USE_CAIRO
    if (MCreatedSurface) cairo_surface_destroy(MSurface);
    #endif
  }

//------------------------------
public: // cairo surface
//------------------------------

  cairo_surface_t*  getSurface()        { return MSurface; }
  cairo_t*          getCairo()          { return MCairo; }

  MIP_Drawable*     getTargetDrawable() { return MTargetDrawable; }
  MIP_Bitmap*       getTargetBitmap()   { return MTargetBitmap; }

//------------------------------
public:
//------------------------------

  //MIP_Drawable* getTarget() override {
  //  MIP_Print("TODO!\n");
  //  return nullptr;
  //}

  //void destroy() override {
  //  cairo_device_finish(MDevice);
  //  cairo_device_destroy(MDevice);
  //}

//------------------------------
public: // surface
//------------------------------

  void setScale(float AXscale, float AYscale) override {
    //cairo_save(MCairo);
    cairo_identity_matrix(MCairo);
    cairo_scale(MCairo,AXscale,AYscale);
  }

  //----------

  void resetScale() override {
    cairo_identity_matrix(MCairo);
  }

  //----------

  /*
    Informs cairo of the new size of the XCB drawable underlying the surface.
    For a surface created for a window (rather than a pixmap), this function
    must be called each time the size of the window changes. (For a subwindow,
    you are normally resizing the window yourself, but for a toplevel window,
    it is necessary to listen for ConfigureNotify events.)

    A pixmap can never change size, so it is never necessary to call this
    function on a surface created for a pixmap.
  */

  void resize(uint32_t AWidth, uint32_t AHeight) override {
    //MIP_Print("w %i h %i\n",AWidth,AHeight);
    cairo_xcb_surface_set_size(MSurface,AWidth,AHeight);
  }

  //----------

  /*
    Do any pending drawing for the surface and also restore any temporary
    modifications cairo has made to the surface's state. This function must be
    called before switching from drawing on the surface with cairo to drawing
    on it directly with native APIs, or accessing its memory outside of Cairo.
    If the surface doesn't support direct access, then this function does
    nothing.
  */

  void flush() override {
    //MIP_PRINT;
    cairo_surface_flush(MSurface);
  }

  //----------

  /*
    Tells cairo that drawing has been done to surface using means other than
    cairo, and that cairo should reread any cached areas. Note that you must
    call cairo_surface_flush() before doing such drawing.
  */

  void dirty() override {
    //MIP_PRINT;
    cairo_surface_mark_dirty(MSurface);
  }

  //----------

  void dirty(MIP_FRect ARect) override {
    //MIP_PRINT;
    //cairo_surface_mark_dirty_rectangle(MCairoSurface,ARect.x,ARect.y,ARect.x2(),ARect.y2());
    cairo_surface_mark_dirty_rectangle(MSurface,ARect.x,ARect.y,ARect.w,ARect.h);
  }

  //----------

  /*
    This function finishes the surface and drops all references to external
    resources. For example, for the Xlib backend it means that cairo will no
    longer access the drawable, which can be freed. After calling
    cairo_surface_finish() the only valid operations on a surface are getting
    and setting user, referencing and destroying, and flushing and finishing it.
    Further drawing to the surface will not affect the surface but will instead
    trigger a CAIRO_STATUS_SURFACE_FINISHED error.
    When the last call to cairo_surface_destroy() decreases the reference count
    to zero, cairo will call cairo_surface_finish() if it hasn't been called
    already, before freeing the resources associated with the surface.
  */

  //----------

  void finish() override {
    //MIP_PRINT;
    cairo_surface_finish(MSurface);
  }

  //----------

//------------------------------
public: // clip
//------------------------------


  /*
    Establishes a new clip region by intersecting the current clip region with
    the current path as it would be filled by cairo_fill() and according to the
    current fill rule (see cairo_set_fill_rule()).

    After cairo_clip(), the current path will be cleared from the cairo context.

    The current clip region affects all drawing operations by effectively
    masking out any changes to the surface that are outside the current clip
    region.

    Calling cairo_clip() can only make the clip region smaller, never larger.
    But the current clip is part of the graphics state, so a temporary
    restriction of the clip region can be achieved by calling cairo_clip()
    within a cairo_save()/cairo_restore() pair. The only other means of
    increasing the size of the clip region is cairo_reset_clip().
  */

  /*
  - After cairo_clip(), the current path will be cleared from the cairo context
  - Calling cairo_clip() can only make the clip region smaller, never larger
  */

  /*
    https://www.cairographics.org/FAQ/#clipping_performance

      The cairo_clip function can be used for two different operations. The
    first is to restrict the set of pixels that need to be updated, (imagine
    needing to draw only half of a window that was just uncovered by another
    window). While the second is to modify what is being drawn with some
    non-pixel-aligned shape (imagine a circular clip path, for example).
      These two uses result in very different code paths inside cairo and in
    the underlying window system. The first case can result in faster
    performance compared to unclipped drawing, since the same drawing
    operations can be performed but fewer pixels are updated. The second case
    can actually result in slower performance as an extra compositing step must
    be added to get the clipped result. This is because the pixels on the edge
    of the mask will have different values than they would in the unclipped
    case, (a non-pixel-aligned clip path results in antialiased clipping).
      So there's a bit of a performance trap there as one might add clipping to
    try to speed things up and inadvertently cause things to go slower. The
    trick to get fast clipping is to ensure the path is always aligned with
    integer positions on the device-pixel grid. And the easiest way to do that
    is to use an identity transformation, (cairo_identity_matrix), and
    construct the path by calling cairo_rectangle with integer values.
  */

  //snap to pixels
  //  x = int(x + 0.5)
  //  y = int(x + 0.5)
  //  return ctx.device_to_user(x,y)

  void setClip(MIP_FRect ARect) override {
    //MIP_Print("%.2f,%.2f,%.2f,%.2f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    cairo_reset_clip(MCairo);
    //float x = MIP_Trunc(ARect.x + 0.5);
    //float y = MIP_Trunc(ARect.y + 0.5);
    //float w = MIP_Trunc(ARect.w + 0.5);
    //float h = MIP_Trunc(ARect.h + 0.5);
    float x = ARect.x;
    float y = ARect.y;
    float w = ARect.w;
    float h = ARect.h;
    cairo_rectangle(MCairo,x,y,w,h);
    cairo_clip(MCairo);
  }

  //----------

  /*
    Reset the current clip region to its original, unrestricted state. That is,
    set the clip region to an infinitely large shape containing the target
    surface. Equivalently, if infinity is too hard to grasp, one can imagine
    the clip region being reset to the exact bounds of the target surface.

    Note that code meant to be reusable should not call cairo_reset_clip() as
    it will cause results unexpected by higher-level code which calls
    cairo_clip(). Consider using cairo_save() and cairo_restore() around
    cairo_clip() as a more robust means of temporarily restricting the clip
    region.
  */

  void resetClip() override {
    //MIP_Trace("RESET CLIP\n");
    cairo_reset_clip(MCairo);
  }

//------------------------------
public: // get
//------------------------------

  float getTextWidth(const char* AText) override {
    cairo_text_extents_t e;
    cairo_text_extents(MCairo,AText,&e);
    return e.width;
  }

  //----------

  float getTextHeight(const char* AText) override {
    cairo_text_extents_t e;
    cairo_text_extents(MCairo,AText,&e);
    return e.height;
  }

//------------------------------
public: // set
//------------------------------

  void setColor(MIP_Color AColor) override {
    cairo_set_source_rgba(MCairo,AColor.r,AColor.g,AColor.b,AColor.a);
  }

  //----------

  void setLineWidth(float ASize) override {
    cairo_set_line_width(MCairo,ASize);
  }

  //----------

  void setLineDash(double* ADashes, uint32_t ANumDashes, float AOffset) override {
    cairo_set_dash(MCairo,ADashes,ANumDashes,AOffset);
  }

  //----------

  void setLineCap(uint32_t ALineCap) override {
    cairo_set_line_cap(MCairo,mip_line_cap[ALineCap]);
  }

  //----------

  void setLineJoin(uint32_t ALineJoin) override {
    cairo_set_line_join(MCairo,mip_line_join[ALineJoin]);
  }

  //----------

  /*
    If text is drawn without a call to cairo_set_font_size(), the default font
    size is 10.0.
  */

  void setFontSize(float ASize) override {
    //if (!MIP_AlmostEqual(MFontSize,ASize,0.001)) {
    if (ASize != MFontSize) {
      MFontSize = ASize;
      cairo_set_font_size(MCairo,ASize);
    }
  }

  //----------

  /*
    Replaces the current cairo_font_face_t object in the cairo_t with font_face.
    The replaced font face in the cairo_t will be destroyed if there are no
    other references to it.
  */

  void setFontFace(const char* AName, uint32_t ASlant, uint32_t AWeight) override {
    cairo_select_font_face(MCairo,AName,mip_font_slant[ASlant],mip_font_weight[AWeight]);
  }

//------------------------------
public: // path
//------------------------------

  void strokePath(bool APreserve=false) override {
    if (APreserve) cairo_stroke_preserve(MCairo);
    else cairo_stroke(MCairo);
  }

  //----------

  void fillPath(bool APreserve=false) override {
    if (APreserve) cairo_fill_preserve(MCairo);
    else cairo_fill(MCairo);
  }

  //----------

  void fillPathGradient(float AX1, float AY1, float AX2, float AY2, MIP_Color AColor1, MIP_Color AColor2, bool AVertical, bool APreserve=false) override {
    cairo_pattern_t *pat;
    if (AVertical) pat = cairo_pattern_create_linear( AX1,AY1, AX1,AY2 );
    else pat = cairo_pattern_create_linear( AX1,AY1, AX2,AY1 );
    cairo_pattern_add_color_stop_rgba( pat, 0, AColor1.r, AColor1.g, AColor1.b, AColor1.a );
    cairo_pattern_add_color_stop_rgba( pat, 1, AColor2.r, AColor2.g, AColor2.b, AColor2.a );
    cairo_set_source(MCairo,pat);
    if (APreserve) cairo_fill_preserve(MCairo);
    else cairo_fill(MCairo);
    cairo_pattern_destroy(pat);
  }

//------------------------------
public:
//------------------------------

  void moveTo(float AX, float AY) override {
    cairo_move_to(MCairo,AX,AY);
  }

  //----------

  void lineTo(float AX, float AY) override {
    cairo_line_to(MCairo,AX,AY);
  }

  //----------

  void curveTo(float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) override {
    cairo_curve_to(MCairo,AX2,AY2,AX3,AY3,AX4,AY4);
  }

  //----------

  /*
    "How do I draw a sharp, single-pixel-wide line?"
    http://www.cairographics.org/FAQ/#sharp_lines
  */

  void horizLine(float AX1, float AY1, float AX2) override {
    cairo_move_to(MCairo,AX1,AY1);
    cairo_line_to(MCairo,AX2,AY1);
    //cairo_stroke(MCairo);
  }

  //----------

  void vertLine(float AX1, float AY1, float AY2) override {
    cairo_move_to(MCairo,AX1,AY1);
    cairo_line_to(MCairo,AX1,AY2);
    //cairo_stroke(MCairo);
  }

  //----------

  void rectangle(MIP_FRect ARect) override {
    cairo_rectangle(MCairo,ARect.x,ARect.y,ARect.w,ARect.h);
  }

  //----------

  // cairo draws 41
  //             32

  //void roundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners) {
  void roundedRectangle(MIP_FRect ARect, float ARadius, uint32_t ACorners, uint32_t AEdges) override {
    int32_t x = ARect.x;
    int32_t y = ARect.y;
    int32_t w = ARect.w;//+1;
    int32_t h = ARect.h;//+1;
    int32_t r = ARadius;
    float degrees = M_PI / 180.0;

    bool left         = (AEdges & MIP_EDGE_LEFT);
    bool right        = (AEdges & MIP_EDGE_RIGHT);
    bool top          = (AEdges & MIP_EDGE_TOP);
    bool bottom       = (AEdges & MIP_EDGE_BOTTOM);
    bool left_top     = (ACorners & MIP_CORNER_LEFT_TOP);
    bool left_bottom  = (ACorners & MIP_CORNER_LEFT_BOTTOM);
    bool right_top    = (ACorners & MIP_CORNER_RIGHT_TOP);
    bool right_bottom = (ACorners & MIP_CORNER_RIGHT_BOTTOM);

    cairo_new_sub_path(MCairo);

    // .1
    // ..

    // start upper right

    if (right_top) cairo_arc(MCairo, x+w-r-0, y+r, r, -90*degrees, 0*degrees);
    else cairo_move_to(MCairo, x+w-0, y);

    // right top to bottom

    if (right) {
      if (right_bottom) cairo_arc(MCairo, x+w-r-0, y+h-r-0, r, 0*degrees, 90*degrees);
      else cairo_line_to(MCairo, x+w-0, y+h-0 );
    }
    else {
      if (right_bottom) cairo_arc(MCairo, x+w-r-0, y+h-r-0, r, 0*degrees, 90*degrees);
      else cairo_move_to(MCairo, x+w-0, y+h-0 );
    }

    // bottom right to left

    if (bottom) {
      if (left_bottom) cairo_arc(MCairo, x+r, y+h-r-0, r, 90*degrees, 180*degrees);
      else cairo_line_to(MCairo, x, y+h-0);

    }
    else {
      if (left_bottom) cairo_arc(MCairo, x+r, y+h-r-0, r, 90*degrees, 180*degrees);
      else cairo_move_to(MCairo, x, y+h-0);
    }

    // left bottom to top

    if (left) {
      if (left_top) cairo_arc(MCairo, x+r, y+r, r, 180*degrees, 270*degrees);
      else cairo_line_to(MCairo, x, y);
    }
    else {
      if (left_top) cairo_arc(MCairo, x+r, y+r, r, 180*degrees, 270*degrees);
      else cairo_move_to(MCairo, x, y);
    }

    // top let to right

    if (top) {
      if (right_top) cairo_line_to(MCairo, x+w-r, y);
      else cairo_line_to(MCairo, x+w, y);
      //cairo_close_path(MCairo);
    }
    else {
      //cairo_move_to(MCairo, x+w-0, y);
      if (right_top) cairo_move_to(MCairo, x+w-r, y);
      else cairo_move_to(MCairo, x+w, y);
      //cairo_close_path(MCairo);
    }

    //double x,y;
    //cairo_get_current_point(MCairo,&x,&y);
                         //    // ..
    //cairo_close_path(MCairo);

  }

  //----------

  //void ellipse(float AX1, float AY1, float AX2, float AY2) {
  void ellipse(MIP_FRect ARect) override {
    float w2 = ARect.w * 0.5f;
    float h2 = ARect.h * 0.5f;
    cairo_save(MCairo);
    cairo_translate(MCairo,ARect.x+w2,ARect.y+h2);
    cairo_scale(MCairo,w2,h2);
    cairo_new_sub_path(MCairo); // ???
    cairo_arc(MCairo,0,0,1,0,(M_PI*2.0));
    cairo_restore(MCairo);
  }

  //----------

  void arc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) override {
    float w2 = (float)(AX2 - AX1 + 1) * 0.5f;
    float h2 = (float)(AY2 - AY1 + 1) * 0.5f;
    float a1 = (AAngle1+0.75) * (M_PI*2.0);
    float a2 = (AAngle1+AAngle2+0.75) * (M_PI*2.0);
    cairo_move_to(MCairo,AX1+w2,AY1+h2);
    cairo_save(MCairo);
    cairo_translate(MCairo,AX1+w2,AY1+h2);
    cairo_scale(MCairo,w2,h2);
    cairo_new_sub_path(MCairo);
    cairo_arc(MCairo,0,0,1,a1,a2);
    cairo_restore(MCairo);
  }

  //----------

  void triangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) override {
    cairo_move_to(MCairo,AX1,AY1);
    cairo_line_to(MCairo,AX2,AY2);
    cairo_line_to(MCairo,AX3,AY3);
    cairo_line_to(MCairo,AX1,AY1);
  }

  //----------

  void text(float AXpos, float AYpos, const char* AText) override {
    cairo_move_to(MCairo,AXpos,AYpos);
    cairo_show_text(MCairo,AText);
  }

  //----------

//------------------------------
public: // draw
//------------------------------

  void drawPoint(float AX, float AY, MIP_Color AColor) override {
    setColor(AColor);
    ellipse(MIP_FRect(AX-0.5f,AY-0.5f,AX+0.5f,AY+0.5f));
    //_rectangle(AX,AY,AX+1,AY+1);
    fillPath();
  }

  //----------

  void drawLine(float AXpos1, float AYpos1, float AXpos2, float AYpos2, MIP_Color AColor, float AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    moveTo(AXpos1,AYpos1);
    lineTo(AXpos2,AYpos2);
    strokePath();
  }

  //void drawLines(int32 ANum, float* ACoords) {
  //  if (ANum > 0) {
  //    _moveTo(ACoords[0],ACoords[1]);
  //    for (int32 i=1; i<ANum; i++) {
  //      _lineTo(ACoords[i*2],ACoords[i*2+1]);
  //    }
  //    _draw();
  //  }
  //}

  //----------

  void drawRectangle(MIP_FRect ARect, MIP_Color AColor, float AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    rectangle(ARect);
    strokePath();
  }

  //----------

  //void drawRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t AC) {
  void drawRoundedRectangle(MIP_FRect ARect, float ARadius, uint32_t ACorners, uint32_t AEdges, MIP_Color AColor, float AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    //roundedRectangle(AX1,AY1,AX2,AY2,AR,AC);
    roundedRectangle(ARect,ARadius,ACorners,AEdges);
    strokePath();
  }

  //----------

  void drawArc(MIP_FRect ARect, float AAngle1, float AAngle2, MIP_Color AColor, float AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    arc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    strokePath();
  }

  //----------

  void drawEllipse(MIP_FRect ARect, MIP_Color AColor, float AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    ellipse(ARect);
    strokePath();
  }

  //----------

  void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor, float AWidth=1) override {
    setColor(AColor);
    setLineWidth(AWidth);
    triangle(AX1,AY1,AX2,AY2,AX3,AY3);
    strokePath();
  }

  //----------

  void drawCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4, MIP_Color AColor, float AWidth=1) {
    setColor(AColor);
    setLineWidth(AWidth);
    moveTo(AX1,AY1);
    curveTo(AX2,AY2,AX3,AY3,AX4,AY4);
    strokePath();
  }

  //----------


//------------------------------
public: // fill
//------------------------------

  void fillRectangle(MIP_FRect ARect, MIP_Color AColor) override {
    setColor(AColor);
    rectangle(ARect);
    fillPath();
  }

  //----------

  void fillRoundedRectangle(MIP_FRect ARect, float ARadius, uint32_t ACorners, uint32_t AEdges, MIP_Color AColor) override {
    setColor(AColor);
    roundedRectangle(ARect,ARadius,ACorners,AEdges);
    fillPath();
  }

  //----------

  void fillArc(MIP_FRect ARect, float AAngle1, float AAngle2, MIP_Color AColor) override {
    setColor(AColor);
    //float x = AX1 + ((AX2-AX1)*0.5f);
    //float y = AY1 + ((AY2-AY1)*0.5f);
    //_moveTo(x,y);
    arc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    fillPath();
  }

  //----------

  void fillEllipse(MIP_FRect ARect, MIP_Color AColor) override {
    setColor(AColor);
    ellipse(ARect);
    fillPath();
  }

  //----------

  void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, MIP_Color AColor) override {
    setColor(AColor);
    triangle(AX1,AY1,AX2,AY2,AX3,AY3);
    fillPath();
  }

  //----------

  //void fillPolygon(int32 ANum, float* ACoords, MIP_Color AColor) {
  //  if (ANum > 0) {
  //    _moveTo(ACoords[0],ACoords[1]);
  //    for (int32 i=1; i<ANum; i++) {
  //      _lineTo(ACoords[i*2],ACoords[i*2+1]);
  //    }
  //    fillPath();
  //  }
  //}

  //----------

  //void fillCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) {
  //}

  //void fillCurve(KPoint AP1, KPoint AP2, KPoint AP3, KPoint AP4) {
  //}

  //----------

  //void fillRectangleGradient(float AX1, float AY1, float AX2, float AY2, MIP_Color AColor1, MIP_Color AColor2, bool AVertical) {
  //  _rectangle(AX1,AY1,AX2,AY2);
  //  _fillGradient(AX1,AY1,AX2,AY2,AColor1,AColor2,AVertical);
  //}

  //----------

  //void fillRoundedRectangleGradient(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners, MIP_Color AColor1, MIP_Color AColor2, bool AVertical) {
  //  _roundedRectangle(AX1,AY1,AX2,AY2,AR,ACorners);
  //  _fillGradient(AX1,AY1,AX2,AY2,AColor1,AColor2,AVertical);
  //}

//------------------------------
public: // text
//------------------------------

  void drawText(float AXpos, float AYpos, const char* AText/*, MIP_Color AColor*/) override {
    //setColor(AColor);
    cairo_move_to(MCairo,AXpos,AYpos);
    cairo_show_text(MCairo,AText);
  }

  //----------

  void drawText(MIP_FRect ARect, const char* AText, uint32_t AAlignment/*, MIP_Color AColor*/) override {
    //setColor(AColor);
    //MIP_Assert(AText);
    cairo_text_extents_t e;
    float xx,yy;
    float x = ARect.x;
    float y = ARect.y;
    float w = ARect.w; //+1;
    float h = ARect.h; //+1;

    cairo_text_extents(MCairo,AText,&e);
    switch (AAlignment) {
      case MIP_TEXT_ALIGN_LEFT:
        xx = x;
        yy = (y+h/2) - (e.height/2 + e.y_bearing);
        break;
      case MIP_TEXT_ALIGN_RIGHT:
        xx = (x+w-1) - (e.width + e.x_bearing);
        yy = (y+h/2) - (e.height/2 + e.y_bearing);
        break;
      case MIP_TEXT_ALIGN_TOP:
        xx = (x + w/2) - (e.width/2  + e.x_bearing);
        yy = y + e.height;
        break;
      case MIP_TEXT_ALIGN_BOTTOM:
        xx = (x + w/2) - (e.width/2  + e.x_bearing);
        yy = (y+h-1) - (e.height + e.y_bearing);
        break;
      case MIP_TEXT_ALIGN_CENTER:
        xx = (x + w/2) - (e.width/2  + e.x_bearing);
        yy = (y+h/2) - (e.height/2 + e.y_bearing);
        break;
      default:
        xx = ARect.x;
        yy = ARect.y;
    }
    cairo_move_to(MCairo,xx,yy);
    cairo_show_text(MCairo,AText);
  }

//------------------------------
public: // image
//------------------------------

  void uploadBitmap(float AXpos, float AYpos, MIP_Bitmap* ABitmap) override {
    //MIP_PRINT;
    cairo_surface_t* srf = ABitmap->createCairoSurface();
    cairo_save(MCairo);
    cairo_set_source_surface(MCairo,srf,0,0);
    cairo_rectangle(MCairo,AXpos,AYpos,ABitmap->getWidth(),ABitmap->getHeight());
    cairo_fill(MCairo);
    cairo_restore(MCairo);
    cairo_surface_destroy(srf);
  }

  //----------

  void drawImage(float AXpos, float AYpos, MIP_Drawable* ASource) override {
    drawImage(AXpos,AYpos,ASource,MIP_FRect(0,0,ASource->getWidth(),ASource->getHeight()));
  }

  //----------

  void drawImage(float AXpos, float AYpos, MIP_Drawable* ASource, MIP_FRect ASrc) override {
    if (ASource->isCairo()) {
      cairo_surface_t* srf = ASource->getCairoSurface();
      cairo_save(MCairo);
      cairo_set_source_surface(MCairo,srf,AXpos-ASrc.x,AYpos-ASrc.y);
      cairo_rectangle(MCairo,AXpos,AYpos,ASrc.w,ASrc.h);
      cairo_fill(MCairo);
      cairo_restore(MCairo);
    }
  }

  //----------

  /*
    scale applies a scaling to all the following operations. When you do
    scale(a, b), all coordinates you use later will be multiplied by a or b
    respectively. Put differently, it pretends that the surface you are
    painting to only has size 1/a, 1/b. In your code, it makes scales the
    surface so that all coordinates are between 0 and 1 instead of 0 and w or h.
    All that identity_matrix() does is to reset all scaling, translations etc
    and returns to no transformation at all. For some docs, see
    cairographics.org/manual/cairo-Transformations.html
  */

  /*
    - Perform cairo_scale(cr, z, z) before drawing pixbufs
    - For every pixbuf being drawn, add cairo_translate(cr, (xpos*(1-z)/z), (ypos*(1-z)/z)); prior to rendering
    - After rendering cairo_translate(cr, -(xpos*(1-z)/z), -(ypos*(1-z)/z));
  */

  // The resulting pattern can be queried with cairo_get_source() so that these
  // attributes can be modified if desired, (eg. to create a repeating pattern
  // with cairo_pattern_set_extend()).

  void drawImage(MIP_FRect ADst, MIP_Drawable* ASource, MIP_FRect ASrc) override {
    float xscale = ADst.w / ASrc.w;
    float yscale = ADst.h / ASrc.h;
    MIP_Assert(xscale > 0.0);
    MIP_Assert(yscale > 0.0);
    float inv_xscale = 1.0 / xscale;
    float inv_yscale = 1.0 / yscale;
    cairo_surface_t* srf = ASource->getCairoSurface();
    cairo_save(MCairo);
    cairo_scale(MCairo,xscale,yscale);
    cairo_set_source_surface(MCairo,srf,((ADst.x) * inv_xscale) - ASrc.x,((ADst.y) * inv_yscale) - ASrc.y);//ASrc.x,ASrc.y);
    cairo_pattern_t* pattern = cairo_get_source(MCairo);
    cairo_pattern_set_extend(pattern,CAIRO_EXTEND_NONE);
    MIP_FRect dst = ADst;
    dst.x *= inv_xscale;
    dst.y *= inv_yscale;
    dst.w *= inv_xscale;
    dst.h *= inv_yscale;
    cairo_rectangle(MCairo,dst.x,dst.y,dst.w,dst.h);
    cairo_rectangle(MCairo,ADst.x * inv_xscale,ADst.y * inv_yscale,ADst.w,ADst.h);
    cairo_fill(MCairo);
    cairo_restore(MCairo);
  }


};

//----------------------------------------------------------------------
#endif
