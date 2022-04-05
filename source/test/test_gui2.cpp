
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO

//#define MIP_NO_WINDOW_BUFFERING

//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_painter.h"
#include "gui/mip_window.h"
#include "gui/mip_widgets.h"

#include "gui/widgets/mip_panel_widget.h"

//#include "../data/img/knob4_60x60_131.h"

//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

class myWindow
: public MIP_Window
, public MIP_MenuListener {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener=nullptr)
  : MIP_Window(AWidth,AHeight,AListener,false) {

    setFillWindowBackground();

    MIP_PanelWidget1* panel1 = new MIP_PanelWidget1(MIP_FRect(20,20, 30,30));
    panel1->setBorderRoundedCorners(MIP_CORNER_LEFT_TOP);
    appendWidget(panel1);

  }

  //----------

  virtual ~myWindow() {
  }

};

  //----------

//  void draw_button(MIP_Painter* painter, const char* text, float x, float y, MIP_Color c1, MIP_Color c2) {
//    const float button_width = 100;
//    const float button_height = 20;
//    const float button_radius = 4;
//    painter->setColor(0.3);
//    painter->roundedRectangle(MIP_FRect(x,y,button_width,button_height),button_radius,MIP_CORNER_ALL);
//    painter->fillPathGradient( x,y,x+button_width,y+button_height, c1, c2, true, true);
//    painter->setColor(0.25);
//    painter->setLineWidth(1);
//    painter->strokePath();
//    painter->setColor(1.0);
//    painter->setFontSize(11);
//    //painter->setFontFace("Verdana",MIP_FONT_SLANT_NORMAL,MIP_FONT_WEIGHT_NORMAL);
//    float tx = x + (button_width / 2)  - (                 painter->getTextWidth(text  ) / 2);
//    float ty = y + button_height       - ((button_height - painter->getTextHeight(text)) / 2);
//    painter->text(tx,ty,text);
//  }

  //----------

//  void draw_slider(MIP_Painter* painter, const char* text, float x, float y, MIP_Color c1, MIP_Color c2) {
//    const float button_width = 100;
//    const float button_height = 20;
//    const float button_radius = 4;
//
//    painter->setColor(0.3);
//    painter->roundedRectangle(MIP_FRect(x,y,button_width,button_height),button_radius,MIP_CORNER_ALL);
//    //painter->fillPathGradient( x,y,x+button_width,y+button_height, c1, c2, true, true);
//    painter->setColor(0.25);
//    painter->setLineWidth(1);
//    painter->strokePath();
//
//    painter->roundedRectangle(MIP_FRect(x,y,(button_width*0.6),button_height),button_radius,MIP_CORNER_ALL);
//    painter->fillPathGradient( x,y,x+(button_width*0.6),y+button_height, c1, c2, true, true);
//    painter->setColor(0.25);
//    painter->setLineWidth(1);
//    painter->strokePath();
//
//    painter->setColor(1.0);
//    painter->setFontSize(11);
//    //painter->setFontFace("Verdana",MIP_FONT_SLANT_NORMAL,MIP_FONT_WEIGHT_NORMAL);
//    //float tx = x + (button_width / 2)  - (                 painter->getTextWidth(text  ) / 2);
//    float tx = x + 10;
//    float ty = y + button_height       - ((button_height - painter->getTextHeight(text)) / 2);
//    painter->text(tx,ty,text);
//  }

  //----------

//  void draw_group(MIP_Painter* painter, const char* text, float x, float y, MIP_Color c1, MIP_Color c2, bool open) {
//    const float button_width = 200;
//    const float button_height = 20;
//    const float button_radius = 4;
//    uint32_t corners = MIP_CORNER_LEFT_TOP | MIP_CORNER_RIGHT_TOP;
//    if (!open) {
//      corners |= MIP_CORNER_LEFT_BOTTOM | MIP_CORNER_RIGHT_BOTTOM;
//    }
//    painter->roundedRectangle(MIP_FRect(x,y,button_width,button_height),button_radius,corners);
//    painter->setColor(0.3);
//    painter->fillPathGradient( x,y,x+button_width,y+button_height, c1, c2, true, true);
//    painter->setColor(0.25);
//    painter->setLineWidth(1);
//    painter->strokePath();
//    if (open) {
//      float x1 = x + button_width - 15;
//      float y1 = y + button_height - 10;
//      float x2 = x1 + 10;
//      float y2 = y1;
//      float x3 = x1 + 5;
//      float y3 = y1 + 6;
//      painter->setColor(0.8);
//      painter->triangle(x1,y1,x2,y2,x3,y3);
//      painter->fillPath();
//      painter->rectangle( MIP_FRect(x,y+button_height,button_width,100) );
//      painter->setColor(0.6);
//      painter->fillPath(true);
//      painter->setColor(0.3);
//      painter->strokePath();
//    }
//    else {
//      float x1 = x + button_width - 15 + 2;
//      float y1 = y + button_height - 10 - 3 - 2;
//      float x2 = x1 + 6;
//      float y2 = y1 + 5;
//      float x3 = x1;
//      float y3 = y1 + 10;
//      painter->setColor(0.8);
//      painter->triangle(x1,y1,x2,y2,x3,y3);
//      painter->fillPath();
//    }
//    painter->setColor(1.0);
//    painter->setFontSize(11);
//    //painter->setFontFace("Verdana",MIP_FONT_SLANT_NORMAL,MIP_FONT_WEIGHT_NORMAL);
//    float tx = x + (button_width / 2)  - (                 painter->getTextWidth(text  ) / 2);
//    float ty = y + button_height       - ((button_height - painter->getTextHeight(text)) / 2);
//    painter->text(tx,ty,text);
//  }

  //----------

//  void draw_tabs(MIP_Painter* painter, float x, float y, MIP_Color c1, MIP_Color c2, MIP_Color c3, MIP_Color c4) {
//    const float button_width = 70;
//    const float button_height = 20;
//    const float button_radius = 4;
//    uint32_t corners = MIP_CORNER_LEFT_TOP | MIP_CORNER_RIGHT_TOP;
//
//    painter->roundedRectangle(MIP_FRect(x,y,button_width,button_height),button_radius,corners);
//    painter->setColor(0.3);
//    painter->fillPathGradient( x,y,x+button_width,y+button_height, c3, c4, true, true);
//    painter->setColor(0.25);
//    painter->setLineWidth(1);
//    painter->strokePath();
//
//    painter->roundedRectangle(MIP_FRect(x+(button_width*1),y,button_width,button_height),button_radius,corners);
//    painter->setColor(0.3);
//    painter->fillPathGradient( x+(button_width*1),y,x+(button_width*2),y+button_height, c1, c2, true, true);
//    painter->setColor(0.25);
//    painter->setLineWidth(1);
//    painter->strokePath();
//
//    painter->roundedRectangle(MIP_FRect(x+(button_width*2),y,button_width,button_height),button_radius,corners);
//    painter->setColor(0.3);
//    painter->fillPathGradient( x+(button_width*2),y,x+(button_width*3),y+button_height, c1, c2, true, true);
//    painter->setColor(0.25);
//    painter->setLineWidth(1);
//    painter->strokePath();
//
//    painter->rectangle( MIP_FRect(x,y+button_height,(button_width*3),100) );
//    painter->setColor(0.6);
//    painter->fillPath(true);
//    painter->setColor(0.3);
//    painter->strokePath(true);
//
//    const char* text = "tab";
//
//    painter->setColor(1.0);
//    painter->setFontSize(11);
//    //painter->setFontFace("Verdana",MIP_FONT_SLANT_NORMAL,MIP_FONT_WEIGHT_NORMAL);
//    float tx = x + (button_width / 2)  - (                 painter->getTextWidth(text  ) / 2);
//    float ty = y + button_height       - ((button_height - painter->getTextHeight(text)) / 2);
//    painter->text(tx,ty,text);
//  }


  //----------

//  void on_window_paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) final {
//    //MIP_Window::on_window_paint(AXpos,AYpos,AWidth,AHeight);
//    MIP_Painter* painter = getWindowPainter();
//    painter->fillRectangle( MIP_FRect(AXpos,AYpos,AWidth,AHeight), MIP_COLOR_GRAY );
//    draw_button(painter, "Button", 10, 10, MIP_COLOR_LIGHT_GRAY,    MIP_COLOR_GRAY    );
//    draw_button(painter, "Button", 10, 35, MIP_COLOR_LIGHT_RED,     MIP_COLOR_RED     );
//    draw_button(painter, "Button", 10, 60, MIP_COLOR_LIGHT_ORANGE,  MIP_COLOR_ORANGE  );
//    draw_button(painter, "Button", 10, 85, MIP_COLOR_LIGHT_YELLOW,  MIP_COLOR_YELLOW  );
//    draw_button(painter, "Button", 10,110, MIP_COLOR_LIGHT_LIME,    MIP_COLOR_LIME    );
//    draw_button(painter, "Button", 10,135, MIP_COLOR_LIGHT_GREEN,   MIP_COLOR_GREEN   );
//    draw_button(painter, "Button", 10,160, MIP_COLOR_LIGHT_GREEN2,  MIP_COLOR_GREEN2  );
//    draw_button(painter, "Button", 10,185, MIP_COLOR_LIGHT_CYAN,    MIP_COLOR_CYAN    );
//    draw_button(painter, "Button", 10,210, MIP_COLOR_LIGHT_CYAN2,   MIP_COLOR_CYAN2   );
//    draw_button(painter, "Button", 10,235, MIP_COLOR_LIGHT_BLUE1,   MIP_COLOR_BLUE1   );
//    draw_button(painter, "Button", 10,260, MIP_COLOR_LIGHT_BLUE,    MIP_COLOR_BLUE    );
//    draw_button(painter, "Button", 10,285, MIP_COLOR_LIGHT_BLUE2,   MIP_COLOR_BLUE2   );
//    draw_button(painter, "Button", 10,310, MIP_COLOR_LIGHT_MAGENTA, MIP_COLOR_MAGENTA );
//    draw_button(painter, "Button", 10,335, MIP_COLOR_LIGHT_RED2,    MIP_COLOR_RED2    );
//    draw_group(painter, "GroupBox Closed", 120,10, MIP_COLOR_GRAY, MIP_COLOR_DARK_GRAY, false );
//    draw_group(painter, "GroupBox Open",   120,35, MIP_COLOR_LIGHT_GRAY, MIP_COLOR_GRAY,  true );
//    draw_tabs(painter, 330,10, MIP_COLOR_LIGHT_GRAY, MIP_COLOR_GRAY, MIP_COLOR_GRAY, MIP_COLOR_DARK_GRAY);
//    draw_slider(painter, "Slider", 120,190, MIP_COLOR_LIGHT_GREEN2,  MIP_COLOR_GREEN2  );
//  }

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(640,480);
  //window->setFillWindowBackground();
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}

