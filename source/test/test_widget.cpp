
#define MIP_GUI_XCB
#define MIP_PAINTER_NANOVG
//#define MIP_PAINTER_OPENGL

#include "mip.h"
#include "gui/widgets/mip_widgets.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------

class myWindow
: public MIP_Window {

//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, bool AEmbedded=false)
  : MIP_Window(AWidth,AHeight,AEmbedded) {
    setup_widgets(AWidth,AHeight);
    setWindowFillBackground(false);

  }

//------------------------------
public:
//------------------------------

  void setup_widgets(uint32_t AWidth, uint32_t AHeight) {
    // background
    MIP_ColorWidget* background = new MIP_ColorWidget( MIP_DRect( 0,0,500,400), MIP_COLOR_GRAY );
    appendChildWidget(background);
    background->Layout.alignment = MIP_WIDGET_ALIGN_CLIENT;

    #define H  22
    #define H2 25

    MIP_ColorWidget*      color1      = new MIP_ColorWidget(      MIP_DRect(  10, 10 + (0 * H2), 150, H ), MIP_COLOR_RED );
    MIP_PanelWidget*      panel1      = new MIP_PanelWidget(      MIP_DRect(  10, 10 + (1 * H2), 150, H ) );
    MIP_TextWidget*       text1       = new MIP_TextWidget(       MIP_DRect(  10, 10 + (2 * H2), 150, H ), "Text" );
    MIP_ValueWidget*      value1      = new MIP_ValueWidget(      MIP_DRect(  10, 10 + (3 * H2), 150, H ), "Value", 0.3 );
    MIP_DragValueWidget*  dragvalue1  = new MIP_DragValueWidget(  MIP_DRect(  10, 10 + (4 * H2), 150, H ), "DragValue", 0.3 );
    MIP_SliderWidget*     slider1     = new MIP_SliderWidget(     MIP_DRect(  10, 10 + (5 * H2), 150, H ), "Slider", 0.3 );

    MIP_KnobWidget*       knob1       = new MIP_KnobWidget(       MIP_DRect( 170, 10, 40,40), "Knob", 0.3 );

    background->appendChildWidget(color1);
    background->appendChildWidget(panel1);
    background->appendChildWidget(text1);
    background->appendChildWidget(value1);
    background->appendChildWidget(dragvalue1);
    background->appendChildWidget(slider1);
    background->appendChildWidget(knob1);

    knob1->setFillBackground(false);
    knob1->setDrawBorder(false);

  }

};

//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(640,480);
  //window->setWindowFillBackground(true);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
